#include <Arduino.h>
#include <AudioStream.h>

class wavetable_processor : public AudioStream {
public:
  wavetable_processor()
    : AudioStream(1, inputQueueArray) {
    playing = false;
    // any extra initialization
  }
  virtual void update(void);
private:
  bool playing;
  audio_block_t* block;
  audio_block_t* inputQueueArray[1];
};



