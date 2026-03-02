#include <Arduino.h>
#include "src/config.h"
#include "src/kernel/Scheduler.h"
#include "src/kernel/Scheduler.h"
#include "src/model/Hangar.h"
#include "src/model/Platform.h"
#include "src/tasks/SweepingTask.h"
#include "src/kernel/MsgService.h"
#include "src/kernel/Logger.h"

Scheduler sched;
Hangar* hangar;
Platform* p;
Task* servoTask;

void setup() {
  MsgService.init();
  sched.init(50);
  p = new Platform();
  hangar = new Hangar();
  servoTask = new SweepingTask(p->getServo());
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
  checkCommands();
}

void checkCommands(){
  if(MsgService.isMsgAvailable()){
    Msg* msg = MsgService.receiveMsg();
    String content = msg->getContent();
    Logger.log(content);
    if (content == "cmd:TAKEOFF"){
      servoTask->setActive(true);
      p->getLCD()->clear();
      p->getLCD()->setCursor(4,1);
      p->getLCD()->print("TAKE OFF");
    }
    delete msg;
  }
}