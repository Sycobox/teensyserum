#include <Arduino.h>
#include <AudioStream.h>

class wavetable_processor : public AudioStream {
public:
  wavetable_processor()
    : AudioStream(1, inputQueueArray) {
    playing = false;
    offset = 0;
    framesize = 0;
    currPos;
    // any extra initialization
  }
  virtual void update(void);
  void addOffset(double percent);
private:
  bool playing;
  int offset;
  audio_block_t* block;
  audio_block_t* inputQueueArray[1];
  int framesize;
  int currPos;
  
};
