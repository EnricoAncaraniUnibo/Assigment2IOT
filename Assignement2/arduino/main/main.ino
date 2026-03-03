#include <Arduino.h>
#include "src/config.h"
#include "src/kernel/Scheduler.h"
#include "src/kernel/Scheduler.h"
#include "src/model/Hangar.h"
#include "src/model/Platform.h"
#include "src/tasks/SweepingTask.h"
#include "src/kernel/MsgService.h"
#include "src/kernel/Logger.h"
#include "src/tasks/TakingOffTask.h"
#include "src/tasks/LandingTask.h"

Scheduler sched;
Hangar* hangar;
Platform* p;
Task* servoTask;
TakingOffTask* takingOffTask;
LandingTask* landingTask;

void setup() {
  MsgService.init();
  sched.init(50);
  p = new Platform();
  hangar = new Hangar();
  servoTask = new SweepingTask(p->getServo());
  takingOffTask = new TakingOffTask(p->getSonar());
  landingTask = new LandingTask(p->getSonar());
  servoTask->init();
  sched.addTask(servoTask);
  sched.addTask(takingOffTask);
  sched.addTask(landingTask);
  p->getPir()->calibrate();
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
  checkTasksCompleted();
  checkCommands();
}

void checkCommands(){
  if(MsgService.isMsgAvailable()){
    Msg* msg = MsgService.receiveMsg();
    String content = msg->getContent();
    if (content == "cmd:TAKEOFF" && hangar->getState()==DRONE_INSIDE){
      hangar->setState(TAKING_OFF);
      takingOffTask->setActive(true);
      servoTask->setActive(true);
      p->getLCD()->clear();
      p->getLCD()->setCursor(4,1);
      p->getLCD()->print("TAKE OFF");
    }
    if (content == "cmd:LAND" && hangar->getState()==DRONE_OUT){
      p->getPir()->sync();
      if(p->getPir()->isDetected()){
        hangar->setState(LANDING);
        landingTask->setActive(true);
        servoTask->setActive(true);
        p->getLCD()->clear();
        p->getLCD()->setCursor(4,1);
        p->getLCD()->print("LANDING");
      }
    }
    delete msg;
  }
}
void checkTasksCompleted(){
  if(takingOffTask->isCompleted() && hangar->getState() == TAKING_OFF){
    hangar->setState(DRONE_OUT);
    servoTask->setActive(true);
    p->getLCD()->clear();
    p->getLCD()->setCursor(4,1);
    p->getLCD()->print("DRONE OUT");
  }
  if(landingTask->isCompleted() && hangar->getState() == LANDING){
    hangar->setState(DRONE_INSIDE);
    servoTask->setActive(true); 
    p->getLCD()->clear();
    p->getLCD()->setCursor(4,1);
    p->getLCD()->print("DRONE INSIDE");
  }
}