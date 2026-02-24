#ifndef __PLATFORM__
#define __PLATFORM__

#include "../config.h"
#include "../devices/ServoTimer2.h"

class Platform {

  public:
  Platform();
  ServoTimer2* getServo();

  private:
  ServoTimer2* servo;
};

#endif