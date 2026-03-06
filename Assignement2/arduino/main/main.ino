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
#include "src/tasks/BlinkingTask.h"
#include "src/tasks/CheckingTemperatureTask.h"

Scheduler sched;
Hangar* hangar;
Platform* p;
Task* servoTask;
TakingOffTask* takingOffTask;
LandingTask* landingTask;
BlinkingTask* blinkingTask;
CheckingTemperatureTask* checkingTempTask;

void setup() {
  MsgService.init();
  sched.init(50);
  p = new Platform();
  hangar = new Hangar();
  MsgService.sendMsg("st:" + String(hangar->getState()));
  servoTask = new SweepingTask(p->getServo());
  takingOffTask = new TakingOffTask(p->getSonar());
  landingTask = new LandingTask(p->getSonar());
  blinkingTask = new BlinkingTask(p->getL2Led());
  checkingTempTask = new CheckingTemperatureTask(p->getTempSensor());
  checkingTempTask->setActive(true);
  servoTask->init();
  sched.addTask(servoTask);
  sched.addTask(takingOffTask);
  sched.addTask(landingTask);
  sched.addTask(blinkingTask);
  sched.addTask(checkingTempTask);
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
      MsgService.sendMsg("st:" + String(hangar->getState()));
      blinkingTask->setActive(true);
      takingOffTask->init();
      servoTask->setActive(true);
      p->getLCD()->clear();
      p->getLCD()->setCursor(4,1);
      p->getLCD()->print("TAKE OFF");
    }
    if (content == "cmd:LAND" && hangar->getState()==DRONE_OUT){
      p->getPir()->sync();
      if(p->getPir()->isDetected()){
        hangar->setState(LANDING);
        MsgService.sendMsg("st:" + String(hangar->getState()));
        blinkingTask->setActive(true);
        landingTask->init();
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
  if(takingOffTask->isCompleted() && (hangar->getState() == TAKING_OFF || hangar->getBackUp() == TAKING_OFF)){
    if(hangar->getState() == TAKING_OFF){
      hangar->setState(DRONE_OUT);
    }else{
      hangar->setBackUp(DRONE_OUT);
    }
    MsgService.sendMsg("st:" + String(hangar->getState()));
    blinkingTask->stop();
    servoTask->setActive(true);
    p->getLCD()->clear();
    p->getLCD()->setCursor(4,1);
    p->getLCD()->print("DRONE OUT");
  }
  if(landingTask->isCompleted() && (hangar->getState() == LANDING || hangar->getBackUp() == LANDING)){
    if(hangar->getState() == LANDING){
      hangar->setState(DRONE_INSIDE);
    }else{
      hangar->setBackUp(DRONE_INSIDE);
    }
    MsgService.sendMsg("st:" + String(hangar->getState()));
    blinkingTask->stop();
    servoTask->setActive(true); 
    p->getLCD()->clear();
    p->getLCD()->setCursor(4,1);
    p->getLCD()->print("DRONE INSIDE");
  }
}