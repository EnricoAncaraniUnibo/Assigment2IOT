#include "CheckingTemperatureTask.h"

CheckingTemperatureTask::CheckingTemperatureTask(TempSensorTMP36* sensor, Hangar* han, Platform* p): sensor(sensor),hangar(han),p(p){
  state=IDLE;
  startTime=0;
}

void CheckingTemperatureTask::tick(){
  temp=sensor->getTemperature();
  if(state==WARMINGPLUS){
    if(temp>Temp2){
      if(millis()-startTime>T4){
        state=ALARM;
        hangar->setState(ALARM);
        setActive(false);
      }
    }else{
      state=PRE_ALARM;
    }
  }

  if(state==PRE_ALARM){
    if(temp<Temp1){
      hangar->setState(hangar->getBackUp());
      state=IDLE;
    }
    if(temp>Temp2){
      startTime=millis();
      state=WARMING_PLUS;
    }
  }
  
  if(state == IDLE){
    if(temp > Temp1){
      state = WARMING;
      startTime = millis();
    }
  } else if(state==WARMING){
    if(temp>Temp1){
      if(millis()-startTime>T3){
        state=PRE_ALARM;
        hangar->setBackUp(hangar->getState());
        hangar->setState(PRE_ALARM);
      }
    }else{
      state=IDLE;
    }
  }
}

void CheckingTemperatureTask::reset(){
  hangar->setState(hangar->getBackUp());
  state=IDLE;
}