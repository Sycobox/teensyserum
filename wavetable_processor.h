#include <Arduino.h>
#include <AudioStream.h>

class wavetable_processor : public AudioStream {
public:
  wavetable_processor()
    : AudioStream(1, inputQueueArray) {
    playing = false;
    frameSize = 1200;
    curPos = 0;
    pitchMult = 0;
    curSample = 0;
    // any extra initialization
  }
  virtual void update(void);
  void addOffset(double percent);
  void noteChange(int midiNote);
  void pitchShift(float _semitones);
  int16_t getNextSample();
  int* getFrameArr(int size);
  int getOffset();
  // bool findZeroCrossing();
  // bool wavetable_processor::loadWavetable();
private:
  bool playing;
  audio_block_t* block;
  audio_block_t* inputQueueArray[1];
  int frameSize;
  int curPos;
  float pitchMult;
  float curSample;
};
