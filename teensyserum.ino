#include "wavetable_processor.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
wavetable_processor wavetable1;  //xy=122,284
AudioOutputI2S i2s1;           //xy=406,284
AudioConnection patchCord1(wavetable1, 0, i2s1, 0);
AudioConnection patchCord2(wavetable1, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=283,332
// GUItool: end automatically generated code
void setup() {
  // put your setup code here, to run once:
  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  
  //currently massive wip
}

void loop() {
  // put your main code here, to run repeatedly:

}
