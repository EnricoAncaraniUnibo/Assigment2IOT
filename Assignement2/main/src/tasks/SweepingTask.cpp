#include "SweepingTask.h"

SweepingTask::SweepingTask(ServoTimer2* servo): servo(servo){
  state=IDLE;
}

void SweepingTask::tick(){
  switch(state){
    case IDLE: {
      state = CLOSE;
      int coeff = (2250-750)/180;
      servo->write(750+0*coeff);
    }
  }
}