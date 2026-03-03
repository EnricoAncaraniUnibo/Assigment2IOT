#include "CheckingTemperatureTask.h"

CheckingTemperatureTask::CheckingTemperatureTask(TempSensorTMP36* sensor): sensor(sensor){
  state=IDLE;
  startTime=0;
}

void CheckingTemperatureTask::tick(){
  temp=sensor->getTemperature();
  if(state == IDLE){
    if(temp > Temp1){
      state = WARMING;
      startTime = millis();
    }
  }
}

void CheckingTemperatureTask::reset(){
  state=IDLE;
}