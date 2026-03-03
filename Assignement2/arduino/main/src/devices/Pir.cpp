#include "Pir.h"
#include "Arduino.h"

Pir::Pir(int pin){
  this->pin = pin;
  pinMode(pin, INPUT);     
} 
  
void Pir::sync(){
  detected = digitalRead(pin) == HIGH;  
}

bool Pir::isDetected(){
  return detected;
}

void Pir::calibrate(){
	delay(10000);
}


