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
  pitchMult = (440 * notePercent) * (float)frameSize/44100;
}
void wavetable_processor::addOffset(double percent) {
  curPos = (int)(percent * (arraySize)/frameSize)*frameSize;
}

int16_t wavetable_processor::getNextSample() {
  int16_t cs = 0;
  __disable_irq();
  curSample = curSample + pitchMult;
  if(curSample > frameSize)
  {
    curSample = 0;
  }
  cs = AudioSampleTestwav[(int)curSample + curPos];


return cs;
  __enable_irq();
}



void wavetable_processor::pitchShift(float _semitones)
{
    pitchMult = pow(2, _semitones / 12.0);
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
