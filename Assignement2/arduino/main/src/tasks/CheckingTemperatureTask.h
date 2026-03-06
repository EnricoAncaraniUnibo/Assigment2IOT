#ifndef __CHECKINGTEMPERATURE_TASK__
#define __CHECKINGTEMPERATURE_TASK__

#include "../kernel/Task.h"
#include <Arduino.h>
#include "../devices/TempSensorTMP36.h"
#include "../config.h"

class CheckingTemperatureTask : public Task {

enum TemperatureState { IDLE,WARMING, PREALARM, ALARM };

public:
  CheckingTemperatureTask(TempSensorTMP36* sensor);
  void tick();
  void reset();

private:
  TemperatureState state;
  TempSensorTMP36* sensor;
  float temp;
  unsigned long startTime;
};


#endif