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
    if (content == "cmd:TAKEOFF" && hangar->getState()==DRONE_INSIDE){
      hangar->setState(TAKING_OFF);
      servoTask->setActive(true);
      p->getLCD()->clear();
      p->getLCD()->setCursor(4,1);
      p->getLCD()->print("TAKE OFF");
    }
    delete msg;
  }
  if(hangar->getState()==TAKING_OFF){
    unsigned long startTime;
    bool start=false;
    bool finish=false;
    startTime=millis();
    while(finish==false){
      float d = p->getSonar()->getDistance();
      Logger.log(String(d));
      if(d>D1){
        if(start==false){
          start=true;
          startTime=millis();
        }
        if(start==true){
          if(millis()-startTime>T1){
            hangar->setState(DRONE_OUT);
            servoTask->setActive(true);
            p->getLCD()->clear();
            p->getLCD()->setCursor(4,1);
            p->getLCD()->print("DRONE_OUT");
            finish=true;
          }
        }
      }else{
        if(start==true){
          start=false;
        }
      }
    }
    
  }
}