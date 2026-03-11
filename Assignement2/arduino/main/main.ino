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
SweepingTask* servoTask;
TakingOffTask* takingOffTask;
LandingTask* landingTask;
BlinkingTask* blinkingTask;
CheckingTemperatureTask* checkingTempTask;

void setup() {
  Logger.log("Setup started");
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
  Logger.log("Setup completed");
}

void loop() {
  if(hangar->getState()==ALARM){
    checkReset();
  }else {
    sched.schedule();
    checkTasksCompleted();
    checkCommands();
  }
  
}

void checkReset(){
  if(p->getButton()->isPressed()){
    Logger.log("RESET pressed");
    checkingTempTask->reset();
    HangarState temp = hangar->getState();
    hangar->setState(hangar->getBackUp());
    hangar->setBackUp(temp);
    p->getL3Led()->switchOff();
    p->getLCD()->clear();
    p->getLCD()->setCursor(4,1);
    switch(hangar->getState()){
      case DRONE_INSIDE: p->getLCD()->print("DRONE INSIDE"); break;
      case DRONE_OUT:    p->getLCD()->print("DRONE OUT");   break;
      case TAKING_OFF:   p->getLCD()->print("TAKE OFF");  servoTask->setActive(true); blinkingTask->setActive(true);  break;
      case LANDING:      p->getLCD()->print("LANDING");   servoTask->setActive(true); blinkingTask->setActive(true);  break;
    }
    MsgService.sendMsg("st:" + String(hangar->getState()));
    checkingTempTask->setActive(true);
  }
}

void checkCommands(){
  if(MsgService.isMsgAvailable()){
    Msg* msg = MsgService.receiveMsg();
    String content = msg->getContent();
    Logger.log("Received: " + content);
    if (content == "cmd:TAKEOFF" && hangar->getState()==DRONE_INSIDE){
      Logger.log("TAKEOFF accepted");
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
      Logger.log("PIR detected=" + String(p->getPir()->isDetected()));
      if(p->getPir()->isDetected()){
        Logger.log("LAND accepted");
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
  CheckingTemperatureTask::TemperatureState tempState = checkingTempTask->getStatus();
  if(hangar->getState()==PRE_ALARM){
    if(tempState!=CheckingTemperatureTask::PRE_ALARM && tempState!=CheckingTemperatureTask::ALARM && tempState!=CheckingTemperatureTask::WARMING_PLUS){
      HangarState temp = hangar->getState();
      hangar->setState(hangar->getBackUp());
      hangar->setBackUp(temp);
      MsgService.sendMsg("st:" + String(hangar->getState()));
      p->getL3Led()->switchOff();
    }
  }else {
    if(tempState==CheckingTemperatureTask::PRE_ALARM){
    hangar->setBackUp(hangar->getState());
    hangar->setState(PRE_ALARM);
    MsgService.sendMsg("st:" + String(hangar->getState()));
  }
  }

  if(tempState==CheckingTemperatureTask::ALARM){
    hangar->setState(ALARM);
    p->getL3Led()->switchOn();
    servoTask->close();
    blinkingTask->stop();
    p->getLCD()->clear();
    p->getLCD()->setCursor(4,1);
    p->getLCD()->print("ALARM");
    if(hangar->getBackUp() == DRONE_OUT){
      MsgService.sendMsg("alarm:ALARM");
    }
    MsgService.sendMsg("st:" + String(hangar->getState()));
  }
  
  if(hangar->getState()!=ALARM){
    if(takingOffTask->isCompleted() && (hangar->getState() == TAKING_OFF || hangar->getBackUp() == TAKING_OFF)){
    if(hangar->getState() == TAKING_OFF){
      hangar->setState(DRONE_OUT);
    }else{
      hangar->setBackUp(DRONE_OUT);
    }
    blinkingTask->stop();
    servoTask->setActive(true);
    p->getLCD()->clear();
    p->getLCD()->setCursor(4,1);
    p->getLCD()->print("DRONE OUT");
    MsgService.sendMsg("st:" + String(hangar->getState()));
    MsgService.sendMsg("dr:" + String("DRONE OUT"));
    }
    if(landingTask->isCompleted() && (hangar->getState() == LANDING || hangar->getBackUp() == LANDING)){
      if(hangar->getState() == LANDING){
        hangar->setState(DRONE_INSIDE);
      }else{
        hangar->setBackUp(DRONE_INSIDE);
      }
      blinkingTask->stop();
      servoTask->setActive(true); 
      p->getLCD()->clear();
      p->getLCD()->setCursor(4,1);
      p->getLCD()->print("DRONE INSIDE");
      MsgService.sendMsg("st:" + String(hangar->getState()));
      MsgService.sendMsg("dr:" + String("DRONE INSIDE"));
    }
  }

}