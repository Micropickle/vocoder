#include <Arduino.h>
#include <driver/i2s.h>
#include <esp_timer.h>
#include <string.h>
#include <math.h>


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

#define SAMPLE_RATE   16000 //8000 gives telephone quality, 16000 offers decent voice transmittion, 44100 offers cd quality music, 48000 gives overall good quality used for music, DVD playing and video
#define BIT_DEPTH     8 // 8 bit audio gives choppy sound, 16 bit offers audio quality compared to a smartphone
#define ADC_BIAS    2230
#define NOISE_THRESHOLD 30
#define BUFFER_COUNT 2
#define BUFFER_SIZE  512 //latency increases with buffer size, 64 offers almost instant playback, 512 offers half a second delay (all dependant on buffer count)

int16_t input_buffer[BUFFER_SIZE];
int16_t output_buffer[BUFFER_SIZE];
int buffer_pos = 0;
// if CPU cannot keep up then audible pops can be heard, if the case then increase buffer size

// an increase in buffer size decreases the frequency of CPU interruptions but increases latency and uses more memory
// an increase in buffer count decreases glitching but also uses more memory

// large buffer lengths but little buffer count = high latency
// low buffer length but many buffers = high interrupt load

// ============================================================================
// AUDIO FUNCTIONS
// ============================================================================

void setup_adc() {
  analogSetPinAttenuation(PIN_ADC_MIC, ADC_11db);
  analogReadResolution(BIT_DEPTH);
}

void input_sample() {
  uint16_t adc_val = analogRead(PIN_ADC_MIC);

  int16_t centred_sample = (int16_t)(adc_val - ADC_BIAS);
  if (abs(centred_sample) < NOISE_THRESHOLD) {
    centred_sample = 0;
  }
  
  input_buffer[buffer_pos] = centred_sample * 64;
}

void process_sample() {
  int16_t processed_sample = input_buffer[buffer_pos];
  // voice changer effects here
  output_buffer[buffer_pos] = processed_sample;
  buffer_pos++;
}

void output_sample() {
  if (buffer_pos >= BUFFER_SIZE) {
    size_t bytes_written = 0;
    i2s_write(I2S_NUM_0, output_buffer, BUFFER_SIZE * sizeof(int16_t), &bytes_written, portMAX_DELAY);
    buffer_pos = 0;
  }
}
void accurate_delays() {
  static uint32_t last_time = 0;
  uint32_t current_time = esp_timer_get_time();
  uint32_t sample_period_us = 1000000 / SAMPLE_RATE;

  while ((current_time - last_time) < sample_period_us) {
    current_time = esp_timer_get_time();
    ets_delay_us(1);
  }
  last_time = current_time;
}

void audio_handler(void* parameter) {
  while (1) {
    input_sample();
    process_sample();
    output_sample();
    accurate_delays();
  }
}

void audio_init() {
  setup_adc();

  xTaskCreate(
    audio_handler,
    "audio_handler",
    1000,
    NULL,
    3,
    NULL
  );
}

// ============================================================================
// I2S SETUP
// ============================================================================

void setup_i2s() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = BUFFER_COUNT,
    .dma_buf_len = BUFFER_SIZE,
    .use_apll = true,
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
  i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
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

#define BT_ADV_POWER ESP_PWR_LVL_N12  // -12dBm (low power)
#define BT_CONN_POWER ESP_PWR_LVL_P9  // +9dBm (high power when connected)


bool deviceConnected = false;
bool oldDeviceConnected = false;
bool advertising = false;
std::string input;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    led_buzzer(2, 50, 100, 5000);
    deviceConnected = true;
    advertising = false;
  };

  void onDisconnect(BLEServer *pServer) {
    led_buzzer(1, 200, 0, 5000);
    if (!advertising) {
      btServer->startAdvertising();
      deviceConnected = false;
      advertising = true;
    }
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
  btAdvertising->setMinInterval(0x0064);  // 100ms in 0.625ms units
  btAdvertising->setMaxInterval(0x00C8);  // 200ms in 0.625ms units
  if (!advertising) {
    btServer->startAdvertising();
    advertising = true;
  }

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
  audio_init();
}

// ============================================================================
// MAIN LOOP
// ============================================================================


void loop() {
  vTaskDelay(10 / portTICK_PERIOD_MS);
}