#ifndef __PLATFORM__
#define __PLATFORM__

#include "../config.h"
#include "../devices/ServoTimer2.h"
#include "../devices/Led.h"
#include <LiquidCrystal_I2C.h>
#include "../devices/Sonar.h"
#include "../devices/Pir.h"

class Platform {

  public:
  Platform();
  ServoTimer2* getServo();
  Led* getL1Led();
  Led* getL2Led();
  Led* getL3Led();
  Sonar* getSonar();
  LiquidCrystal_I2C* getLCD();
  Pir* getPir();
  private:
  ServoTimer2* servo;
  Led* l1;
  Led* l2;
  Led* l3;
  LiquidCrystal_I2C* lcd;
  Sonar* s;
  Pir* pi;
};

#endif