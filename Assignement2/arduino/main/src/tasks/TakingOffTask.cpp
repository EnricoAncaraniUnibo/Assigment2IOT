#include "TakingOffTask.h"
#include "../config.h"

TakingOffTask::TakingOffTask(Sonar* sonar): sonar(sonar){
  state = WAITING;
  startTime = 0;
  completed = false;
}

bool TakingOffTask::isCompleted(){
  return completed;
}

void TakingOffTask::init(){
  completed = false;
  state = WAITING;
  startTime = 0;
  setActive(true);
}

void TakingOffTask::tick(){
  float d = sonar->getDistance();
  MsgService.sendMsg("dist:" + String(d));
  if(state == WAITING){
    if(d > D1){
      state = COUNTING;
      startTime = millis();
    }
  } else if(state == COUNTING){
    if(d > D1){
      if(millis() - startTime > T1){
        completed = true;
        state = WAITING;
        setActive(false);
      }
    } else {
      state = WAITING;
    }
  }
}