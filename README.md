# THE VOCODER

a voice amplifier for use with encumbersome masks such as gas masks
<div style="display: flex; gap: 10px;">
  <img width="30%" height="30%" alt="board CAD" src="photos/CAD designs/KiCad/Screenshot from 2026-03-05 19-23-45.png" />
  <img width="30%" height="30%" alt="prototype showcase" src="photos/prototype version/IMG_20260123_094632.jpg" />
  </div>

## breif description
the vocoder is a device that allows you to alter your voice to your specifications and allows you to amplify it through speakers and bone transducer speakers and is especially useful in situations where wearing a mask can muffle your voice.

## inspiration
the project was first inspired by a fictonal story/world called degenesis where people called chroniclers disguised their voices using vocoders, long tubes went down their throats to gather their voice and was fed into the 'vocoder' which then was distorted and was amplifiable. along with that i own masks and gasmasks that make it hard to communicate with others so the device is meant to gather your voice via a mic or throat mic and output it via speakers or bone transducer speakers placed on the mask itself which is very useful for hobbies such as airsofting.

## gallery
<div style="display: flex; gap: 1px;">
  <img width="45%" height="45%" src="photos/CAD designs/KiCad/Screenshot from 2026-03-05 19-23-30.png">
  <img width="45%" height="45%" src="photos/CAD designs/KiCad/Screenshot from 2026-03-05 19-23-13.png">
<div/>
<div style="display: flex; gap: 1px;">
  <img width="45%" height="45%" src="photos/CAD designs/KiCad/Screenshot from 2026-03-05 19-23-45.png">
  <img width="45%" height="45%" src="photos/CAD designs/KiCad/Screenshot from 2026-03-05 19-23-59.png">
<div/>

## how to use
1. plug in mic of your choice (the standard is the audio jack port found in phones, but the board can be modified for different ports)
2. compile the project on Platformio and flash to the board (this is necessary as in the future i will implement configurations for BLE and initial settings for audio modification such as amplification, distortion, pitch changing, etc.)
3. wire in audio output of your choosing

## bill of materials

| Designator         | Footprint                                                     | Quantity | Value                       | LCSC Part # |
|:------------------:|:-------------------------------------------------------------:|:--------:|:---------------------------:|:-----------:|
| BZ1                | PinHeader_1x02_P2.54mm_Vertical                               | 1        | Buzzer                      |             |
| C1, C2, C4, C7, C8 | 0603                                                          | 5        | 10u                         |             |
| C10, C11           | 0603                                                          | 2        | 0.1u                        |             |
| C12, C13, C14      | 0603                                                          | 3        | 1u                          |             |
| C3, C5, C6         | 0603                                                          | 3        | 100n                        |             |
| D1, D2             | 1206                                                          | 2        | LED                         |             |
| J1                 | Jack_3.5mm_PJ320D_Horizontal                                  | 1        | AudioJack4                  |             |
| J2                 | PinHeader_1x02_P2.54mm_Vertical                               | 1        | Conn_01x02_Socket           |             |
| J3                 | TerminalBlock_Phoenix_MKDS-1,5-2-5.08_1x02_P5.08mm_Horizontal | 1        | Conn_01x02_Pin              |             |
| J4                 | USB_C_Receptacle_GCT_USB4105-xx-A_16P_TopMnt_Horizontal       | 1        | USB_C_Receptacle_USB2.0_16P |             |
| Q1, Q2, Q3         | SOT-23                                                        | 3        | Q_NPN_BCE                   |             |
| R1, R2             | 0603                                                          | 2        | 100                         |             |
| R10, R9            | 0603                                                          | 2        | 5.1k                        |             |
| R3, R5, R6         | 0603                                                          | 3        | 10k                         |             |
| R4                 | 0603                                                          | 1        | 100k                        |             |
| R7                 | 0603                                                          | 1        | 1.2k                        |             |
| SW1                | SW_Slide-03_Wuerth-WS-SLTV_10x2.5x6.4_P2.54mm                 | 1        | SW_SPDT                     |             |
| SW2, SW3           | SW_Push_SPST_NO_Alps_SKRK                                     | 2        | SW_Push                     |             |
| U1                 | ESP32-S2-MINI-1                                               | 1        | ESP32-S3-MINI-1             |             |
| U2                 | SOIC-8-1EP_3.9x4.9mm_P1.27mm_EP2.41x3.3mm_ThermalVias         | 1        | TP4056-42-ESOP8             |             |
| U3                 | SOT-23-5                                                      | 1        | TPS73633DBV                 |             |
| U5                 | LFCSP-32-1EP_5x5mm_P0.5mm_EP3.5x3.5mm_ThermalVias             | 1        | ADAU1761                    |             |



## general buying section

for common components (and components that i have listed to be apart of the general buying section) the common place to aquire these would be JLCPCB along with the board your ordering, Digikey and similar shops as they sell many common components in bulk, amazon with different resistor and capacitor kits.

## initial prototype
it started off as a proof of concept through a prototype board using a simple XIAO esp32 c3 to test the idea and the limitations, this taught me alot digital audio, sound systems and BLE as all three are very necessary for this project
<img width="30%" height="30%" alt="prototype board" src="photos/prototype version/IMG_20260123_094607.jpg" />

the issue was everything was powered by the 3v3 pin on the esp, i had to rely on an external amplifier (and external power supply) to make it work and the esp's clock speed was slow. 

## acknowledgement
i want to give my thanks to hackclub for helping me to make this
link to my journal blueprint: https://blueprint.hackclub.com/projects/8772



