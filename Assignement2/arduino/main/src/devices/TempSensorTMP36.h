#ifndef __TEMP_SENSOR_TMP36__
#define __TEMP_SENSOR_TMP36__

class TempSensorTMP36{
public:
  TempSensorTMP36(int p);
  float getTemperature();
private:
  int pin;
};


#endif
