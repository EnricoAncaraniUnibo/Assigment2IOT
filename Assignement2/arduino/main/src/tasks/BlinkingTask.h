#ifndef __BLINKING_TASK__
#define __BLINKING_TASK__

#include "../kernel/Task.h"
#include <Arduino.h>
#include "../devices/Led.h"

class BlinkingTask : public Task {

enum LedState { ON, OFF };

public:
  BlinkingTask(Led* led);
  void tick();
  void stop();

private:
  LedState state;
  Led* led;
  unsigned long lastTimeBlink;
};

#endif