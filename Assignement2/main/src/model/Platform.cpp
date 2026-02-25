#include "Platform.h"

Platform::Platform(){
  servo = new ServoTimer2();
  servo->attach(MOTOR_PIN);
  l1 = new Led(L1_PIN);
  l2 = new Led(L2_PIN);
  l3 = new Led(L3_PIN);
  lcd = new LiquidCrystal_I2C(0x27, 20, 4);
}

ServoTimer2* Platform::getServo(){
  return this->servo;
}

Led* Platform::getL1Led(){
  return this->l1;
}

Led* Platform::getL2Led(){
  return this->l2;
}

Led* Platform::getL3Led(){
  return this->l3;
}

LiquidCrystal_I2C* Platform::getLCD(){
  return this->lcd;
}