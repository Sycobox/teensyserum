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

//initializing all the public variables
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
  //for printing (serial monitor output)
  Serial.begin(9600);
  //sets up the rotery encoder button
  pinMode(31, INPUT);
  //diplay setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  //test start (splash screen)
  display.display();
  delay(200);
  //draws current table display
  drawCurrFrame();
  //sets up audio library
  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  //sets up midi input handler
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
}

//not much is happening in this loop function, mainly calls other functions
void loop() {
  setEnv();
  usbMIDI.read();
  changePos();
  changeMode();
  checkMode();
}

//intializes the envelope values using the values set later
void setEnv() {
  envelope1.attack(attack);
  envelope1.decay(decay);
  envelope1.sustain(sus);
  envelope1.release(release);
}

//using the rotery encoder button, switches modes by changing a variable value
void changeMode() {
  lbs = bs;
  bs = digitalRead(buttonPin);
  if (lbs == LOW and bs == HIGH) {
    //change mode
    mode++;
    //if the mode is not to show the wavetable, display the text parameters for the envelope
    if (mode > 0) {
      textDisplay();
    }
    //if mode is beyond the bounds of modes, return back to 0
    if (mode > 4) {
      mode = 0;
    }
    //just to check which mode you are in
    Serial.println(mode);
    //to allow headroom for button bounce back triggers
    delay(5);
  }
}

//actually sets the mode to change function control
void checkMode() {
  if (mode == 0) {
    //changes to the wavetable view
    tableView();
  } else if (mode == 1) {
    //changes pos to negative value so that when it switches back to wavetable view it actually changes
    pos = -1;
    //allows you to change attack
    changeAttack();
  } else if (mode == 2) {
    //allows you to change release
    changeRelease();
  } else if (mode == 3) {
    //allows you to change sustain
    changeSustain();
  } else if (mode == 4) {
    //allows you to change decay
    changeDecay();
  }
}

//Using a pot value, it will change the position of the wavetable (changes the frame)
void changePos() {
  //read analog pot value
  int potValue = analogRead(A14);
  //map pot value between 0 and 100
  int mappedPotValue = map(potValue, 0, 1027, 0, 100);
  //take the mapped pot value and turn it into a percentage to be used for the addOffset function
  wavetable1.addOffset((double)mappedPotValue / 100);
}

//Makes sure that it only updates the screen whenever the position of the wavetable changes
void tableView() {
  lastpos = pos;
  pos = wavetable1.getOffset();
  if (lastpos != pos) {
    //if pot value changes, redraw the wave frame
    drawCurrFrame();
  }
}

//Changes the attack value and updates the screen only if the rotery encoder moves
void changeAttack() {
  int newEncPos = enc.read();
  if (newEncPos - oldEncPos >= 4) {
    oldEncPos = newEncPos;  // update the encoder's position
    attack += 5; //adjust attack by 5ms
    textDisplay(); //update the text display
  }
  if (newEncPos - oldEncPos <= -4) {
    oldEncPos = newEncPos;
    attack -= 5; //adjust attack by 5ms
    //restrict attack to 1ms as the lowest
    if (attack < 1) {
      attack = 1;
    }
    textDisplay(); //update the text display
  }
}

//Changes the release value and updates the screen only if the rotery encoder moves
void changeRelease() {
  int newEncPos = enc.read();
  if (newEncPos - oldEncPos >= 4) {
    oldEncPos = newEncPos;  // update the encoder's position
    release += 5;//adjust release by 5ms
    textDisplay();//update the text display
  }
  if (newEncPos - oldEncPos <= -4) {
    oldEncPos = newEncPos;
    release -= 5; //adjust release by 5ms
    //restrict release to 1ms as the lowest
    if (release < 1) {
      release = 1;
    }
    textDisplay(); //update the text display
  }
}

//Changes the sustain value and updates the screen only if the rotery encoder moves
void changeSustain() {
  int newEncPos = enc.read();
  if (newEncPos - oldEncPos >= 4) {
    oldEncPos = newEncPos;  // update the encoder's position
    sus += 0.05; //adjust sustain by 5%
    //restrict sus to 100% as max
    if (sus > 1) {
      sus = 1;
    }
    textDisplay(); //update the text display
  }
  if (newEncPos - oldEncPos <= -4) {
    oldEncPos = newEncPos;  // update the encoder's position
    sus -= 0.05; //adjust sustain by 5%
    //restrict sus to 0% as lowest
    if (sus < 0) {
      sus = 0;
    }
    textDisplay(); //update the text display
  }
}

//Changes the decay value and updates the screen only if the rotery encoder moves
void changeDecay() {
  int newEncPos = enc.read();
  if (newEncPos - oldEncPos >= 4) {
    oldEncPos = newEncPos;  // update the encoder's position 
    decay += 5; //adjust decay by 5ms
    textDisplay(); //update the text display
  }
  if (newEncPos - oldEncPos <= -4) {
    oldEncPos = newEncPos;  // update the encoder's position
    decay -= 5; //adjust decay by 5ms
    //restrict decay to 0ms as the lowest
    if (decay < 0) {
      decay = 0;
    }
    textDisplay(); //update the text display
  }
}

//Updates the display for text based on the mode
void textDisplay() {
  //clears display to allow for new display
  display.clearDisplay();
  //sets up text to start with
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  //attack mode
  if (mode == 1) {
    display.println(F("attack: "));
    display.print((int)attack);
    display.println(" ms");
  }
  //release mode
  if (mode == 2) {
    display.println(F("release: "));
    display.print((int)release);
    display.println(" ms");
  }
  //sustain mode
  if (mode == 3) {
    display.println(F("sustain: "));
    display.print((int)(sus * 100));
    display.println("%");
  }
  //decay mode
  if (mode == 4) {
    display.println(F("decay: "));
    display.print((int)decay);
    display.println(" ms");
  }
  display.display(); //update display
}

//handles the note on from DAW
void OnNoteOn(byte channel, byte note, byte velocity) {
  //calls note change function based on midi note
  wavetable1.noteChange(note);
  envelope1.noteOn();
}

//handles the note off from DAW
void OnNoteOff(byte channel, byte note, byte velocity) {
  envelope1.noteOff();
}

//calculates and draws the current frame based on the values given from the getFrameArr() method
void drawCurrFrame() {
  //clears display to allow for new display
  display.clearDisplay();
  //sets up a wavetable pixel height arr (pointer)
  int* waveFrame = wavetable1.getFrameArr(display.width());
  //sets the pixels based on the width
  for (int i = 0; i < display.width(); i++) {
    //sets the pixel to be on using the wavetable amp
    display.drawPixel(i, waveFrame[i], SSD1306_WHITE);
    //print wavetable pixel values
    Serial.println(waveFrame[i]);
  }
  display.display(); //refresh display
  delete[] waveFrame; //delete the array to free up memory
}
