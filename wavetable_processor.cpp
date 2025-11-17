#include "wavetable_processor.h"
#include <SD.h>
#include "AudioSampleTestwav.h"

int arraySize = sizeof(AudioSampleTestwav) / sizeof(AudioSampleTestwav[0]);


void wavetable_processor::update() {

  block = allocate();
  if (block == NULL) return;

  for (int i = 0; i < 128; i++) {
    block->data[i] = AudioSampleTestwav[i*10 + offset];
  }

  transmit(block, 0);
  release(block);
}

void wavetable_processor::addOffset(double percent) {
  offset = (int)(percent * (arraySize - 1280));
}

bool wavetable_processor::findZeroCrossing()
{
  checkedFrameSize = 0;
  int middlePoint = 2147483647;
  int i = currPos;
  bool zeroCrossed = false;
  bool passedNeg = false;
  if (AudioSampleTextwav[currPos] != middlePoint)
  {
    return false;
  }
  while(!zeroCrossed || i < arraySize)
  {
    if(AudioSampleTestwav[i] < middlePoint)
    {
      passedNeg = true;
    }
    if(passedNeg && AudioSampleTestwav[i] == middlePoint)
    {
      zeroCrossed = true;
    }
    i++;
    checkedFrameSize++;
  }
  framesize = checkedFrameSize;
  return true;
}