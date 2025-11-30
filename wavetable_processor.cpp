#include "wavetable_processor.h"
#include <SD.h>
#include "AudioSampleTestwav.h"
#include <cmath>

int arraySize = sizeof(AudioSampleTestwav) / sizeof(AudioSampleTestwav[0]);
float pitchMult = 0;
int cycler = 0;


void wavetable_processor::update() {
  block = allocate();
  if (block == NULL) return;
  for (int i = 0; i < 128; i++) {
    block->data[i] = getNextSample();
  }
  transmit(block, 0);
  release(block);
}

void wavetable_processor::noteChange(int midiNote) {
  float notePercent = pow(2, (float)(midiNote - 69) / 12);
  pitchMult = (440 * notePercent) * (float)frameSize / 44100;
}
void wavetable_processor::addOffset(double percent) {
  curPos = (int)(percent * (arraySize-frameSize) / frameSize) * frameSize;
}

int16_t wavetable_processor::getNextSample() {
  int16_t cs = 0;
  __disable_irq();
  curSample = curSample + pitchMult;
  if (curSample > frameSize) {
    curSample = 0;
  }
  cs = AudioSampleTestwav[(int)curSample + curPos];
  return cs;
  __enable_irq();
}

void wavetable_processor::pitchShift(float _semitones) {
  pitchMult = pow(2, _semitones / 12.0);
}

int* wavetable_processor::getFrameArr(int size) {
  int* arr = new int[size];
  // Use a double for the step size to ensure accurate indexing
  double step_size = (double)frameSize / size; 
  for (int i = 0; i < size; i++) { // Loop 'size' times to fill all of arr
    // 1. Calculate the high-resolution input index using the double step_size
    int input_index = (int)(i * step_size) + curPos;
    uint16_t raw_value = (uint16_t)AudioSampleTestwav[input_index]; 
    // 2. Convert unsigned sample to signed (centered at 0)
    int signed_sample = (int)raw_value - 32768;
    // 3. Linear Scaling: Map the signed sample (-32768 to 32767) to pixel range (-32 to 32)
    // We multiply the signed sample by the scale factor to get the signed pixel displacement.
    double scaled_displacement = (double)signed_sample * ((64.0 / 2.0) / 32767.0);
    if(scaled_displacement < 0)
    {
      scaled_displacement += 64;
    }
    // 4. Assign the signed pixel displacement to the array
    // We use std::round() for the best visual representation.
    arr[i] = (int)std::round(scaled_displacement);
  }
  return arr;
}

int wavetable_processor::getOffset()
{
  return curPos;
}
// bool wavetable_processor::findZeroCrossing()
// {
//   int checked = 0;
//   unsigned int middlePoint = AudioSampleTestwav[curPos];
//   int i = curPos;
//   bool zeroCrossed = false;
//   bool passedNeg = false;
//   if (AudioSampleTextwav[curPos] != middlePoint)
//   {
//     return false;
//   }
//   while(!zeroCrossed || i < arraySize)
//   {
//     if(AudioSampleTestwav[i] < middlePoint)
//     {
//       passedNeg = true;
//     }
//     if(passedNeg && AudioSampleTestwav[i] == middlePoint)
//     {
//       zeroCrossed = true;
//     }
//     i++;
//     checked++;
//   }
//   frameSize = checkedFrameSize;
//   return true;
// }

// bool wavetable_processor::loadWavetable()
// {

// }
