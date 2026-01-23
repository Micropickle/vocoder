# THE VOCODER

a voice amplifier for use with encumbersome masks such as gas masks
<div style="display: flex; gap: 10px;">
  <img width="30%" height="30%" alt="board CAD" src="photos/CAD designs/KiCad/Screenshot from 2026-01-22 20-45-53.png" />
  <img width="30%" height="30%" alt="prototype showcase" src="photos/prototype version/IMG_20260123_094632.jpg" />
  </div>

## breif description
the vocoder is a device that allows you to alter your voice to your specifications and allows you to amplify it through speakers and bone transducer speakers and is especially useful in situations where wearing a mask can muffle your voice.

## inspiration
the project was first inspired by a fictonal story/world called degenesis where people called chroniclers disguised their voices using vocoders, long tubes went down their throats to gather their voice and was fed into the 'vocoder' which then was distorted and was amplifiable. along with that i own masks and gasmasks that make it hard to communicate with others so the device is meant to gather your voice via a mic or throat mic and output it via speakers or bone transducer speakers placed on the mask itself which is very useful for hobbies such as airsofting.

## gallery
<div style="display: flex; gap: 1px;">
  <img width="45%" height="45%" src="photos/CAD designs/KiCad/Screenshot from 2026-01-22 20-45-09.png">
  <img width="45%" height="45%" src="photos/CAD designs/KiCad/Screenshot from 2026-01-23 09-53-42.png">
<div/>
<div style="display: flex; gap: 1px;">
  <img width="45%" height="45%" src="photos/CAD designs/KiCad/Screenshot from 2026-01-22 20-45-53.png">
  <img width="45%" height="45%" src="photos/CAD designs/KiCad/Screenshot from 2026-01-22 20-46-21.png">
<div/>

## how to use
1. plug in mic of your choice (the standard is the audio jack port found in phones, but the board can be modified for different ports)
2. compile the project on Platformio and flash to the board (this is necessary as in the future i will implement configurations for BLE and initial settings for audio modification such as amplification, distortion, pitch changing, etc.)
3. wire in audio output of your choosing

## bill of materials

included below are some of the larger components and external components needed, the full BOM can be found in the production files in hardware

|quantity|component|part link|
|:------:|:-------:|:--------:|
|1|XIAO ESP32-S3|https://thepihut.com/products/seeed-studio-xiao-esp32s3|
|1|1x05 dip switches|https://www.digikey.co.uk/en/products/detail/cts-electrocomponents/219-5MST/223200
|1|power slide switch|https://www.tehonline.co.uk/products/mini-slide-switch-spdt-2-54mm
|1|buzzer|https://thepihut.com/products/buzzer-5v-breadboard-friendly
|1|female jack|https://shokitech.com/product/3-5-mm-audio-jack-female-trrs-smd-4-pcs/
|1|screw headers|https://uk.farnell.com/camdenboss/ctbp0500-2/tb-wire-to-brd-2pos-16awg/dp/2668617
|1|li polymer single cell battery|https://www.amazon.co.uk/EEMB-2000mAh-103454-Rechargeable-Connector/dp/B08214DJLJ/ref=sr_1_4?sr=8-4

## initial prototype
it started off as a proof of concept through a prototype board using a simple XIAO esp32 c3 to test the idea and the limitations, this taught me alot digital audio, sound systems and BLE as all three are very necessary for this project
<img width="30%" height="30%" alt="prototype board" src="photos/prototype version/IMG_20260123_094607.jpg" />

the issue was everything was powered by the 3v3 pin on the esp, i had to rely on an external amplifier (and external power supply) to make it work and the esp's clock speed was slow. 

## acknowledgement
i want to give my thanks to hackclub for helping me to make this
link to my journal blueprint: https://blueprint.hackclub.com/projects/8772
