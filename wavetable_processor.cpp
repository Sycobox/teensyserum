#include "wavetable_processor.h"
#include <SD.h>
#include "AudioSampleTestwav.h"
#include <cmath>

int arraySize = sizeof(AudioSampleTestwav) / sizeof(AudioSampleTestwav[0]);

/*
essential for ANY audio object, this is what actually sets and plays back the audio
*/
void wavetable_processor::update() {
  //1. allocates memory in the block space
  block = allocate();
  //2. if the block is empty, will not doing anything (avoids null pointer exception) 
  if (block == NULL) return;
  //fill up the 128 sample buffer
  for (int i = 0; i < 128; i++) {
    //3. calls for the next sample from the array using a specific method
    block->data[i] = getNextSample();
  }
  //4. Plays the waveform
  transmit(block, 0);
  release(block);
}

/*
Based ona midi value, calculate the interval to iterate through the array to speed up or slow down the wave
*/
void wavetable_processor::noteChange(int midiNote) {
  //1. This finds based on midi note, the percentage based on A as a starting point
  float notePercent = pow(2, (float)(midiNote - 69) / 12);
  //2. Finds the interval at which the array should skip so that it speeds up
  pitchMult = (440 * notePercent) * (float)frameSize / 44100;
}

/*
Based on a percentage of the entire waveform, it finds where to move the position
*/
void wavetable_processor::addOffset(double percent) {
  //Changes the posoiton of the based on percent value of the table
  //Its dividing and remultiplying so that it moves based on the frame size interval
  curPos = (int)(percent * (arraySize - frameSize) / frameSize) * frameSize;
}

/*
Calculates and finds the array value to play
@returns the next array value
*/
int16_t wavetable_processor::getNextSample() {
  //1. initializes a method variable to set to the new sample
  int16_t cs = 0;
  //2. some random harware interrupt stuff dw bout this... (waits for the calculuation to go through forcing hardware to wait)
  __disable_irq();
  //3. skips through the interval using the pitch mult we calculated earlier
  curSample = curSample + pitchMult;
  //4. if the cur sample is beyond the frame size, reset back to 0
  if (curSample > frameSize) {
    curSample = 0;
  }
  //cs is set to the actual array value using the index we calculated earlier
  cs = AudioSampleTestwav[(int)curSample + curPos];
  return cs;
  //5. allows the calculuation to finish and reenables interrupt
  __enable_irq();
}

/*
Was originally going to be used for pitchbending, however has not been completed LOL
*/
void wavetable_processor::pitchShift(float _semitones) {
  pitchMult = pow(2, _semitones / 12.0);
}

/* 
Converts the values of the waveform to pixel values to display on the screen
Limits the values so that they are inbetween 0 and 63 and centers the waveform at pixel 32
@returns an array that scales down the amplitudes of the wavetable frame to pixel values for the screen value
*/
int* wavetable_processor::getFrameArr(int size) {
  int* arr = new int[size];
  // use a double for the step size to ensure accurate indexing
  double step_size = (double)frameSize / size;
  for (int i = 0; i < size; i++) {  // Loop 'size' times to fill all of arr
    // 1. calculates the high-resolution input index using the double step_size
    int input_index = (int)(i * step_size) + curPos;
    uint16_t raw_value = (uint16_t)AudioSampleTestwav[input_index];
    // 2. convert unsigned sample to signed (centered at 0)
    int signed_sample = (int)raw_value - 32768;
    // 3. linear Scaling: Map the signed sample (-32768 to 32767) to pixel range (-32 to 32)
    // we multiply the signed sample by the scale factor to get the signed pixel displacement.
    double scaled_displacement = (double)signed_sample * ((64.0 / 2.0) / 32767.0);
    //4. converts the negative pixel values back to the middle and centers at 32 pixels as the middle point
    if (scaled_displacement < 0) {
      scaled_displacement += 64;
    }
    // 5. assign the signed pixel displacement to the array
    // we use std::round() for the best visual representation.
    arr[i] = (int)std::round(scaled_displacement);
  }
  return arr;
}

/*
@returns the current position of the wavetable
*/
int wavetable_processor::getOffset() {
  return curPos;
}
