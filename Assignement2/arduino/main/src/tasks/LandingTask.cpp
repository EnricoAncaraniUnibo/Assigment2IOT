#include "LandingTask.h"
#include "../config.h"

LandingTask::LandingTask(Sonar* sonar): sonar(sonar){
  state = WAITING;
  startTime = 0;
  completed = false;
}

bool LandingTask::isCompleted(){
  return completed;
}

void LandingTask::init(){
  completed = false;
  state = WAITING;
  startTime = 0;
  setActive(true);
}

void LandingTask::tick(){
  float d = sonar->getDistance();

  if(state == WAITING){
    if(d < D2){
      state = COUNTING;
      startTime = millis();
    }
  } else if(state == COUNTING){
    if(d < D2){
      if(millis() - startTime > T2){
        completed = true;
        state = WAITING;
        setActive(false);
      }
    } else {
      state = WAITING;
    }
  }
}