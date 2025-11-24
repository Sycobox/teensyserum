#include "wavetable_processor.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
wavetable_processor wavetable1;  //xy=122,284
AudioEffectEnvelope      envelope1;      //xy=447,275
AudioOutputI2S           i2s1;           //xy=806,293
AudioConnection          patchCord1(wavetable1, envelope1);
AudioConnection          patchCord2(envelope1, 0, i2s1, 0);
AudioConnection          patchCord3(envelope1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=446,410
// GUItool: end automatically generated code
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
}

void loop() {
  envelope1.attack(1);   // 200 ms for the sound to fade in
  envelope1.decay(0);      // no decay for now
  envelope1.sustain(1.0);  // sustain at full volume
  envelope1.release(1);
  usbMIDI.read();
  int potValue = analogRead(A14);
  int mappedPotValue = map(potValue, 0, 1027, 0, 100);
  wavetable1.addOffset((double)mappedPotValue / 100);
  Serial.println(mappedPotValue);
  
  // put your main code here, to run repeatedly:
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  wavetable1.noteChange(note); // Any Note-On turns on LED
  envelope1.noteOn();
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  //  // Any Note-Off turns off LED
  envelope1.noteOff();
}
