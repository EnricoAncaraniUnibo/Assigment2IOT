#include "Platform.h"

Platform::Platform(){
  servo = new ServoTimer2();
  servo->attach(MOTOR_PIN);
}

ServoTimer2* Platform::getServo(){
  return this->servo;
}