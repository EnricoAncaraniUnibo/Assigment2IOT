#include <Arduino.h>
#include "src/config.h"
#include "src/kernel/Scheduler.h"
#include "src/kernel/Scheduler.h"
#include "src/model/Hangar.h"
#include "src/model/Platform.h"
#include "src/tasks/SweepingTask.h"

Scheduler sched;
Hangar* hangar;
Platform* p;

void setup() {
  sched.init(50);
  p = new Platform();
  hangar = new Hangar();
  Task* servoTask = new SweepingTask(p->getServo());
  servoTask->init();
  sched.addTask(servoTask);
}

void loop() {
  sched.schedule();
}