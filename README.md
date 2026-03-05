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



## general buying section

for common components (and components that i have listed to be apart of the general buying section) the common place to aquire these would be JLCPCB along with the board your ordering, Digikey and similar shops as they sell many common components in bulk, amazon with different resistor and capacitor kits.

## initial prototype
it started off as a proof of concept through a prototype board using a simple XIAO esp32 c3 to test the idea and the limitations, this taught me alot digital audio, sound systems and BLE as all three are very necessary for this project
<img width="30%" height="30%" alt="prototype board" src="photos/prototype version/IMG_20260123_094607.jpg" />

the issue was everything was powered by the 3v3 pin on the esp, i had to rely on an external amplifier (and external power supply) to make it work and the esp's clock speed was slow. 

## acknowledgement
i want to give my thanks to hackclub for helping me to make this
link to my journal blueprint: https://blueprint.hackclub.com/projects/8772


