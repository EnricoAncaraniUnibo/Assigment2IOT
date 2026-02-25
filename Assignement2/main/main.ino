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
  p->getL1Led()->switchOn();
  p->getL2Led()->switchOff();
  p->getL3Led()->switchOff();
  p->getLCD()->init();
  p->getLCD()->backlight();
  p->getLCD()->setCursor(4,1);
  p->getLCD()->print("DRONE INSIDE");
}

void loop() {
  sched.schedule();
}