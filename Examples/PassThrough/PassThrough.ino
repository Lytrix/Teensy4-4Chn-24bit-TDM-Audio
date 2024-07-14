#include "AudioStream1.h"
#include "output_tdm4.h"
#include "input_tdm4.h"
#include <Wire.h>
#include "control_AK4619VN.h"
#include <FreqCount.h>
// GUItool: begin automatically generated code
AudioInputTDM            tdm1;           //xy=266,244
AudioOutputTDM           tdm2;           //xy=460,251
AudioConnection          patchCord1(tdm1, 0, tdm2, 0);
AudioConnection          patchCord2(tdm1, 1, tdm2, 1);
AudioConnection          patchCord3(tdm1, 2, tdm2, 2);
AudioConnection          patchCord4(tdm1, 3, tdm2, 3);
// AudioConnection          patchCord5(tdm1, 4, tdm2, 4);
// AudioConnection          patchCord6(tdm1, 5, tdm2, 5);
// AudioConnection          patchCord7(tdm1, 6, tdm2, 6);
// AudioConnection          patchCord8(tdm1, 7, tdm2, 7);
// AudioConnection          patchCord9(tdm1, 8, tdm2, 8);
// AudioConnection          patchCord10(tdm1, 9, tdm2, 9);
// AudioConnection          patchCord11(tdm1, 10, tdm2, 10);
// AudioConnection          patchCord12(tdm1, 11, tdm2, 11);
// AudioConnection          patchCord13(tdm1, 12, tdm2, 12);
// AudioConnection          patchCord14(tdm1, 13, tdm2, 13);
// AudioConnection          patchCord15(tdm1, 14, tdm2, 14);
// AudioConnection          patchCord16(tdm1, 15, tdm2, 15);
// AudioControlCS42448      cs42448_1;      //xy=414,425
// GUItool: end automatically generated code

AK4619VN codec(&Wire, AK4619VN_ADDR);

// Use pin9 to test Clock Frequencies on pin 20/21/23
// https://www.pjrc.com/teensy/td_libs_FreqCount.html    
void debugClockFreq() {
  if (FreqCount.available()) {
    unsigned long count = FreqCount.read();
    Serial.print("Clock Frequency: ");
    Serial.print(count/1000);
    Serial.println("kHz");
  }
}

void setup() {
  AudioMemory(50);
  Serial.begin(9600);
  Serial.println("TDM Passthrough");
  pinMode(22, OUTPUT); //PWN on
  digitalWrite(22, HIGH);
  delay(1000);
  
  codec.begin(18, 19); // SDA, SCL for Teensy 4.1
  /*    pin  6 : SDIN
        pin  7 : SDOUT
        pin 23 : MCLK
        pin 21 : BICK
        pin 20 : SYNC */
  
  //Set CODEC to reset state for initialization
  uint8_t error = 0;
  error = codec.setRstState(true); 
  if(error){
    Serial.println("Unable to set codec reset state");
  }
  //Enable PWR of DAC1&2 and ADC1&2
  error = codec.pwrMgm(true, true, true, true); 
  if(error){
    Serial.println("Unable to set codec PWR");
  }

  // Set Slot start (LR (false) or Slot length), BICK Edge falling/rising, SDOut speed slow/fast
  //error = codec.audioFormatMode(AK4619VN::AK_I2S_STEREO, false, false);
  //error = codec.audioFormatMode(AK4619VN::AK_MSB_STEREO, false, false);
//error = codec.audioFormatMode(AK4619VN::AK_TDM256_I2S_32B, false, false);
  error = codec.audioFormatMode(AK4619VN::AK_TDM128_I2S_32B, false, false);
  //error = codec.audioFormatMode(AK4619VN::AK_TDM128_MSB_32B, true, false);
  
    // Set TDM mode and Slot Length for DAC 1&2 and ADC1&2
  error = codec.audioFormatSlotLen(AK4619VN::AK_SLOT, AK4619VN::AK_32BIT, AK4619VN::AK_24BIT);
  if(error){
    Serial.println("Unable to set slot length.");
  }
  
  if(error){
    Serial.println("Unable to set audio format mode.");
  }  
  // Set sample rate to 96kHz
  //error = codec.sysClkSet(AK4619VN::AK_256FS_96KS);
  //error = codec.sysClkSet(AK4619VN::AK_256FS_8_48KS);
  error = codec.sysClkSet(AK4619VN::AK_128FS_192KS);
  if(error){
    Serial.println("Unable to set system clock mode.");
  }
  // Set all 4 ADC gains to 0dB
  error = codec.micGain(AK4619VN::AK_MIC_GAIN_0DB, AK4619VN::AK_MIC_GAIN_0DB, AK4619VN::AK_MIC_GAIN_0DB, AK4619VN::AK_MIC_GAIN_0DB); 
  if(error){
    Serial.println("Unable to set codec mic input gain.");
  }
  // // Set all 4 ADC digital volume to 0dB
  // error = codec.inputGain(AK4619VN::AK_IN_GAIN_0DB, AK4619VN::AK_IN_GAIN_0DB, AK4619VN::AK_IN_GAIN_0DB, AK4619VN::AK_IN_GAIN_0DB); 
  // if(error){
  //   Serial.println("Unable to set codec digital input input gain.");
  // }
  //Set all DAC1 LR gains to 0dB
  error = codec.outputGain(false, AK4619VN::AK_DAC1B, AK4619VN::AK_OUT_GAIN_0DB); 
  if(error){
    Serial.println("Unable to set DAC1 gain.");
  }
  //Set all DAC2 LR gains to 0dB
  error = codec.outputGain(false, AK4619VN::AK_DAC2B, AK4619VN::AK_OUT_GAIN_0DB);
  if(error){ 
    Serial.println("Unable to set DAC2 gain.");
  }
  //Set Input config to Single ended 1 on both ADCs
  error = codec.inputConf(AK4619VN::AK_IN_SE1, AK4619VN::AK_IN_SE1, AK4619VN::AK_IN_SE1, AK4619VN::AK_IN_SE1);
  if(error){
    Serial.println("Unable to set DAC input configuration.");
  }
  //DAC2 to SDOUT2, DAC1 to SDOUT1
  error = codec.outputConf(AK4619VN::AK_OUT_SDIN2, AK4619VN::AK_OUT_SDIN1); 
  //error = codec.outputConf(AK4619VN::AK_OUT_SDOUT1, AK4619VN::AK_OUT_SDOUT1); 
  if(error){
    Serial.println("Unable to set DAC input configuration.");
  }
  
  //Release Reset state
  error = codec.setRstState(false); 
  if(error){
    Serial.println("Unable to clear codec reset state.");
  }
  delay(500);
  Serial.println("Audio Codec Setup completed:");
  delay(500);
  //Verify settings
  codec.printRegs(0x0, 21);

  //Update Time in microseconds
  FreqCount.begin(1000000); 

}

void loop() {
    debugClockFreq();
}
