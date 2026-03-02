#include "SweepingTask.h"
#include "../kernel/Logger.h"

SweepingTask::SweepingTask(ServoTimer2* servo): servo(servo){
  state="P";
}

void SweepingTask::tick(){
  Logger.log("Sono dentro");
    if(state=="P"){
      state = "C";
      int coeff = (2250-750)/180;
      servo->write(750+0*coeff);
      setActive(false);
      Logger.log("Ho chiuso");
    } else if(state=="C"){
      state = "O";
      int coeff = (2250-750)/180;
      servo->write(750+180*coeff);
      setActive(false);
      Logger.log("Ho aperto");
    }
}