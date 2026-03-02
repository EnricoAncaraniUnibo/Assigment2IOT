#include "SweepingTask.h"
#include "../kernel/Logger.h"

SweepingTask::SweepingTask(ServoTimer2* servo): servo(servo){
  state=IDLE;
}

void SweepingTask::tick(){
    if(state==IDLE){
      state = CLOSE;
      float coeff = (2250.0-750.0)/180.0;
      servo->write(750+0*coeff);
      setActive(false);
    } else if(state==CLOSE){
      state = OPEN;
      float coeff = (2250.0-750.0)/180.0;
      servo->write(750+180*coeff);
      setActive(false);
    }else if (state==OPEN){
      state=CLOSE;
      float coeff = (2250.0-750.0)/180.0;
      servo->write(750+0*coeff);
      setActive(false);
    }
}