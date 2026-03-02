#ifndef __SWEEPING_TASK__
#define __SWEEPING_TASK__

#include "../kernel/Task.h"
#include <Arduino.h>
#include "../devices/ServoTimer2.h"

class SweepingTask: public Task{

  public:
    SweepingTask(ServoTimer2* servo);
    void tick();
  private:
    char state;
    ServoTimer2* servo;
};

#endif