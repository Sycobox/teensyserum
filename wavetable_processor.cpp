#include "wavetable_processor.h"
#include <SD.h>

void wavetable_processor::update() {

  block = allocate();
  if (block == NULL) return;

  for (int i = 0; i < 128; i++) {
    if (i < 64) block->data[i] = 32768;
    else block->data[i] = 0;
  }

  transmit(block, 0);
  release(block);
}