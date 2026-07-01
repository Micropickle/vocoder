# THE VOCODER

a voice amplifier for use with encumbersome masks such as gas masks

## breif description
the vocoder is a device that allows you to alter your voice to your specifications and allows you to amplify it through speakers and bone transducer speakers and is especially useful in situations where wearing a mask can muffle your voice.

## inspiration
the project was first inspired by a fictonal story/world called degenesis where people called chroniclers disguised their voices using vocoders, long tubes went down their throats to gather their voice and was fed into the 'vocoder' which then was distorted and was amplifiable. along with that i own masks and gasmasks that make it hard to communicate with others so the device is meant to gather your voice via a mic or throat mic and output it via speakers or bone transducer speakers placed on the mask itself which is very useful for hobbies such as airsofting or cosplaying. the device also has professional use cases such as when having to wear heavy clothing and gasmasks such as PPE, this device will make you clearly audible to others when working

## gallery

CAD designs
photos/CAD designs/KiCad

initial prototype photos
photos/prototype version

## how to use
1. plug in mic of your choice (the standard is the audio jack port found in phones, but the board can be modified for different ports) and wire in an external speaker that meets the specifications of the amplifier chip
2. compile the project on Platformio and flash to the board (this is necessary as in the future i will implement configurations for BLE and initial settings for audio modification such as amplification, distortion, pitch changing, etc.)
3. wire in audio output of your choosing

## bill of materials

hardware/production/bom.csv

## initial prototype
it started off as a proof of concept through a prototype board using a simple XIAO esp32 c3 to test the idea and the limitations, this taught me alot digital audio, sound systems and BLE as all three are very necessary for this project
<img width="30%" height="30%" alt="prototype board" src="photos/prototype version/IMG_20260123_094607.jpg" />

the issue was everything was powered by the 3v3 pin on the esp, i had to rely on an external amplifier (and external power supply) to make it work, furthermore the esp's clock speed was too slow and brought about glitches in the audio output. 

## description
the circuit board went through many design stages primarily due to three things, errors, expense and having to include extra components to make the circuit work properly.

the main parts of the circuit are as followed
* the esp32-s3 board was chosen for its high clockspeed, familiarity and extensive functionality, the esp32-s3 WROOM board had included wifi, bluetooth, BLE and usb pins reducing external circuits needed and opening up options in terms of customisation
* the ADAU1761 is a codec that includes a ADC, DAC and built in processor to make changes to the incoming and outcoming audio, this was chosen due to its specialty in audio processing which will reduce the processing power from the esp32 and its ease of programming due to the manufacturers having dedicated graphical software for the chip, the only problem is custom drivers need to be written for the esp32 to upload the code for the ADAU1761
* the MAX98357A was chosen due to its bilt in i2s interfacw which is compatable with the esp32 and the adau1761 as both can be interfaced with the i2s bus, an amplifier was included so that the audio output can be directed to a speaker or bone transucer without an external amplifier or power supply 




