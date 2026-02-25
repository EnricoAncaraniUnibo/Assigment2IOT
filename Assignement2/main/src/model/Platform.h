#ifndef __PLATFORM__
#define __PLATFORM__

#include "../config.h"
#include "../devices/ServoTimer2.h"
#include "../devices/Led.h"

class Platform {

  public:
  Platform();
  ServoTimer2* getServo();
  Led* getL1Led();
  Led* getL2Led();
  Led* getL3Led();
  private:
  ServoTimer2* servo;
  Led* l1;
  Led* l2;
  Led* l3;
};

#endif