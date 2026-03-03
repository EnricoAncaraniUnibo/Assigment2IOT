#include "BlinkingTask.h"

BlinkingTask::BlinkingTask(Led* led): led(led){
  state=OFF;
  lastTimeBlink=0;
}

void BlinkingTask::tick(){
  if(millis()-lastTimeBlink>=500){
    if(state==ON){
    led->switchOff();
    state=OFF;
    } else {
      led->switchOn();
      state=ON;
    }
    lastTimeBlink=millis();
  }
}

void BlinkingTask::stop(){
  state=OFF;
  led->switchOff();
  setActive(false);
}