#ifndef __TAKINGOFF_TASK__
#define __TAKINGOFF_TASK__

#include "../kernel/Task.h"
#include <Arduino.h>
#include "../devices/Sonar.h"
#include "../kernel/MsgService.h"

class TakingOffTask : public Task {

enum TakingOffState { WAITING, COUNTING };

public:
  TakingOffTask(Sonar* sonar);
  bool isCompleted();
  void tick();
  void init();
private:
  TakingOffState state;
  Sonar* sonar;
  unsigned long startTime;
  bool completed;
};

#endif