#include "wavetable_processor.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// GUItool: begin automatically generated code
wavetable_processor wavetable1;  //xy=122,284
AudioEffectEnvelope envelope1;   //xy=447,275
AudioOutputI2S i2s1;             //xy=806,293
AudioConnection patchCord1(wavetable1, envelope1);
AudioConnection patchCord2(envelope1, 0, i2s1, 0);
AudioConnection patchCord3(envelope1, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=446,410
// GUItool: end automatically generated code
Encoder enc(29, 30);
int buttonPin = 31;
bool lbs = false;
bool bs = false;
int lastpos = 0;
int pos = 0;
float sus = 1.0;
float release = 1;
float attack = 1;
float decay = 0;
int mode = -1;
int oldEncPos = -1;



void setup() {
  Serial.begin(9600);
  pinMode(31, INPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.display();
  delay(200);
  drawCurrFrame();
  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
}

void loop() {
  setEnv();
  usbMIDI.read();
  changePos();
  changeMode();
  checkMode();
  // put your main code here, to run repeatedly:
}

void setEnv() {
  envelope1.attack(attack);
  envelope1.decay(decay);
  envelope1.sustain(sus);
  envelope1.release(release);
}

void changeMode() {
  lbs = bs;
  bs = digitalRead(buttonPin);
  if (lbs == LOW and bs == HIGH) {
    mode++;
    if (mode > 0) {
      textDisplay();
    }
    if (mode > 4) {
      mode = 0;
    }
    Serial.println(mode);
    delay(5);
  } else {
  }
}

void checkMode() {
  if (mode == 0) {
    tableView();
  } else if (mode == 1) {
    pos = -1;
    changeAttack();
  } else if (mode == 2) {
    changeRelease();
  } else if (mode == 3) {
    changeSustain();
  } else if (mode == 4) {
    changeDecay();
  }
}

void changePos() {
  int potValue = analogRead(A14);
  int mappedPotValue = map(potValue, 0, 1027, 0, 100);
  wavetable1.addOffset((double)mappedPotValue / 100);
}

void tableView() {
  lastpos = pos;
  pos = wavetable1.getOffset();
  if (lastpos != pos) {
    drawCurrFrame();
  }
}

void changeAttack() {
  int newEncPos = enc.read();
  if (newEncPos - oldEncPos >= 4) {
    oldEncPos = newEncPos;  // update the encoder's position
    attack += 5;
    textDisplay();
  }
  if (newEncPos - oldEncPos <= -4) {
    oldEncPos = newEncPos;
    attack -= 5;
    if (attack < 1) {
      attack = 1;
    }
    textDisplay();
  }
}

void changeRelease() {
  int newEncPos = enc.read();
  if (newEncPos - oldEncPos >= 4) {
    oldEncPos = newEncPos;  // update the encoder's position
    release += 5;
    textDisplay();
  }
  if (newEncPos - oldEncPos <= -4) {
    oldEncPos = newEncPos;
    release -= 5;
    if (release < 1) {
      release = 1;
    }
    textDisplay();
  }
}

void changeSustain() {
  int newEncPos = enc.read();
  if (newEncPos - oldEncPos >= 4) {
    oldEncPos = newEncPos;  // update the encoder's position
    sus += 0.05;
    if (sus > 1) {
      sus = 1;
    }
    textDisplay();
  }
  if (newEncPos - oldEncPos <= -4) {
    oldEncPos = newEncPos;
    sus -= 0.05;
    if (sus < 0) {
      sus = 0;
    }
    textDisplay();
  }
}

void changeDecay() {
  int newEncPos = enc.read();
  if (newEncPos - oldEncPos >= 4) {
    oldEncPos = newEncPos;  // update the encoder's position
    decay += 5;
    textDisplay();
  }
  if (newEncPos - oldEncPos <= -4) {
    oldEncPos = newEncPos;
    decay -= 5;
    if (decay < 0) {
      decay = 0;
    }
    textDisplay();
  }
}

void textDisplay() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  if (mode == 1) {
    display.println(F("attack: "));
    display.print((int)attack);
    display.println(" ms");
  }
  if (mode == 2) {
    display.println(F("release: "));
    display.print((int)release);
    display.println(" ms");
  }
  if (mode == 3) {
    display.println(F("sustain: "));
    display.print((int)(sus * 100));
    display.println("%");
  }
  if (mode == 4) {
    display.println(F("decay: "));
    display.print((int)decay);
    display.println(" ms");
  }
  display.display();
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  wavetable1.noteChange(note);
  envelope1.noteOn();
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  envelope1.noteOff();
}

void drawCurrFrame() {
  display.clearDisplay();
  int* waveFrame = wavetable1.getFrameArr(display.width());
  for (int i = 0; i < display.width(); i++) {
    display.drawPixel(i, waveFrame[i], SSD1306_WHITE);
    Serial.println(waveFrame[i]);
  }
  display.display();
  delete[] waveFrame;
}
