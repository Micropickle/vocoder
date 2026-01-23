#include <Arduino.h>
#include <driver/i2s.h>
#include <esp_timer.h>
#include <string.h>


// ============================================================================
// PIN CONFIGURATION
// ============================================================================

#define PIN_I2S_BCLK  8
#define PIN_I2S_WORD  10
#define PIN_I2S_DATA  9
#define PIN_I2S_MCLK  7

#define PIN_ADC_MIC   4

#define PIN_LED       3
#define PIN_BUZZER    2

// ============================================================================
// AUDIO CONFIGURATION
// ============================================================================

#define SAMPLE_RATE   20000
#define BIT_DEPTH     16

#define ADC_BIAS    2230    

#define BUFFER_SIZE   32

// ============================================================================
// AUDIO EFFECTS
// ============================================================================


// ============================================================================
// I2S SETUP
// ============================================================================

void setup_i2s() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = BUFFER_SIZE,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };
  i2s_pin_config_t pin_config = {
    .mck_io_num = PIN_I2S_MCLK,
    .bck_io_num = PIN_I2S_BCLK,
    .ws_io_num = PIN_I2S_WORD,
    .data_out_num = PIN_I2S_DATA,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);
}

// ============================================================================
// EXTRAS
// ============================================================================

uint8_t default_pitch = 255;

void buzzer_beep(size_t amount = 1, uint32_t onTime = 100, uint32_t offTime = 100, uint8_t pitch = default_pitch) {
  for (int i = 0; i < amount; i++) {
    analogWrite(PIN_BUZZER, pitch);
    delay(onTime);
    analogWrite(PIN_BUZZER, 0);
    delay(offTime);
  }
  return;
}

int default_freq = 5000;

void buzzer_beep_freq(size_t amount = 1, uint32_t onTime = 100, uint32_t offTime = 100, int freq = default_freq) {
  for (int i = 0; i < amount; i++) {
    tone(PIN_BUZZER, freq, onTime);
    delay(offTime);
  }
  return;
}

uint8_t default_brightness = 255;

void led_flicker(size_t amount = 1, uint32_t onTime = 50, uint32_t offTime = 300, uint8_t brightness = default_brightness) {
  for (int i = 0; i < amount; i++) {
    analogWrite(PIN_LED, brightness);
    delay(onTime);
    analogWrite(PIN_LED, 0);
    delay(offTime);
  }
  return;
}

void led_buzzer(size_t amount = 1, uint32_t onTime = 50, uint32_t offTime = 300, int freq = default_freq, uint8_t brightness = default_brightness) {
  for (int i = 0; i < amount; i++) {
    analogWrite(PIN_LED, brightness);
    tone(PIN_BUZZER, freq, onTime);
    delay(onTime);
    analogWrite(PIN_LED, 0);
    delay(offTime);
  }
  return;
}

// ============================================================================
// BLUETOOTH COMMANDS
// ============================================================================

//heirarchy:    profile(server)->service->characteristic->properties+value+descriptor

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEUUID.h>
#include <BLESecurity.h>

#define SERVICE_TEST_UUID        "c4f1f458-f41b-45c9-8636-7e098e715259"
#define CHARACTERISTIC_TEST_UUID "6829258f-513c-47c2-b6ac-328ce39ddd94"

BLEServer *btServer;
BLECharacteristic *btCharacteristics;
BLESecurity *btSecurity;

#define BTNAME "vocoder"

bool deviceConnected = false;
bool oldDeviceConnected = false;
std::string input;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    led_buzzer(2, 50, 100, 5000);
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer) {
    led_buzzer(1, 200, 0, 5000);
    btServer->startAdvertising();
    deviceConnected = false;
  }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    input = pCharacteristic->getValue();
    Serial.print("message: "); Serial.print(input.c_str()); Serial.println();
  }
  void onRead(BLECharacteristic* pChar) {
    Serial.println("characteristic read");
  }
};

void bt_init() {
  BLEDevice::init(BTNAME);
  btServer = BLEDevice::createServer();
  btServer->setCallbacks(new MyServerCallbacks);


  BLEService *btService = btServer->createService(SERVICE_TEST_UUID);

  btCharacteristics = btService->createCharacteristic(
                      CHARACTERISTIC_TEST_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  btCharacteristics->setCallbacks(new MyCharacteristicCallbacks);

  btCharacteristics->setValue("testing");

  btService->start();


  BLEAdvertising *btAdvertising = btServer->getAdvertising();
  btAdvertising->addServiceUUID(SERVICE_TEST_UUID);
  btAdvertising->setScanResponse(true);
  btServer->startAdvertising();

  return;
}

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  bt_init();
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  setup_i2s();
}

// ============================================================================
// MAIN LOOP
// ============================================================================


void loop() {
  static int16_t audio_buffer[BUFFER_SIZE * 2];
  static size_t samples_collected = 0;
  
  // Wait for I2S to be ready (it will tell us when it needs data)
  size_t bytes_written = 0;
  
  // Only process when we have a full buffer
  if (samples_collected < BUFFER_SIZE) {
    // Collect samples as fast as possible
    int raw_adc = analogRead(PIN_ADC_MIC);
    int16_t sample = (raw_adc - ADC_BIAS) << 2;
    
    // Clip
    if (sample > 32767) sample = 32767;
    if (sample < -32768) sample = -32768;
    
    // Store in stereo
    int stereo_index = samples_collected * 2;
    audio_buffer[stereo_index] = sample;
    audio_buffer[stereo_index + 1] = sample;
    
    samples_collected++;
  } else {
    // Send when buffer is full
    i2s_write(I2S_NUM_0, audio_buffer, sizeof(audio_buffer), &bytes_written, portMAX_DELAY);
    samples_collected = 0;
  }
}