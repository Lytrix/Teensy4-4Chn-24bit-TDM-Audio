# Teensy Audio 4 channel in/out TDM 24 bit 192kHz

This repo is a modification of the [Teensy 4 Audio TDM Library](http://github.com/paulstoffregen/audio) for the 4 channel in/out AK4619VN codec and the Audiostream of the [Teensy Cores Library](https://github.com/PaulStoffregen/cores/tree/master/teensy4) to be able to handle 24bit audio 192kHz as input and pass it through to the output using the Audio patchcords.

Pin setup:
```  
  pin  6 : SDIN
  pin  7 : SDOUT
  pin  9 : Test Clock speed pin
  pin 18 : SDA
  pin 19 : SCL
  pin 20 : SYNC
  pin 21 : BICK
  pin 22 : PWN Power/Reset to program codec
  pin 23 : MCLK
  

```
To work with TDM 24bit 96 or higher both MCLCK and BCK must be running at 24.546Mhz (=128*192000Hz or 256*96000Hz) 

**For 24 bit 96kHz**
uncomment row 108 and row 122 to run at 256: 
```
codec.audioFormatMode(AK4619VN::AK_TDM256_I2S_32B, false, false);

codec.sysClkSet(AK4619VN::AK_256FS_96KS);
```

The [AK4619VN Codec library](https://github.com/Lytrix/ak4619vn) is forked from the code created by Trimmenz for ESP32 boards and modified to work with a Teensy.

A barebones version without using the Audio library can be found here:
https://github.com/Lytrix/Teensy4-i2s-TDM

Todo:
Modify other Audio library 16bit oriented code to handle 24 bit audio like playing Wave files from SD card and recording.
