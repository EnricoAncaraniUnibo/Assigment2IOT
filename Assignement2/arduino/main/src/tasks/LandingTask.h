#ifndef __LANDING_TASK__
#define __LANDING_TASK__

#include "../kernel/Task.h"
#include <Arduino.h>
#include "../devices/Sonar.h"

class LandingTask : public Task {

  enum LandingState { WAITING, COUNTING };

public:
  LandingTask(Sonar* sonar);
  bool isCompleted();
  void tick();
  void init();
private:
  LandingState state;
  Sonar* sonar;
  unsigned long startTime;
  bool completed;
};

#endif