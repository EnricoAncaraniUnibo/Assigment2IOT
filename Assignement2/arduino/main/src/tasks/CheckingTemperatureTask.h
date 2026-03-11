#ifndef __CHECKINGTEMPERATURE_TASK__
#define __CHECKINGTEMPERATURE_TASK__

#include "../kernel/Task.h"
#include <Arduino.h>
#include "../devices/TempSensorTMP36.h"
#include "../config.h"
#include "../kernel/Logger.h"

class CheckingTemperatureTask : public Task {



public:
  enum TemperatureState { IDLE,WARMING, PRE_ALARM,WARMING_PLUS, ALARM };
  CheckingTemperatureTask(TempSensorTMP36* sensor);
  void tick();
  void reset();
  TemperatureState getStatus();

private:
  TemperatureState state;
  TempSensorTMP36* sensor;
  float temp;
  unsigned long startTime;
};


#endif