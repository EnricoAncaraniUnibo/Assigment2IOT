#include "Sonar.h"
#include "Arduino.h"

Sonar::Sonar(int trigPin, int echoPin, float temp){
  this->trigPin = trigPin;
  this->echoPin = echoPin;
  float temperature = temp;
  vs = 331.45 + 0.62 * temperature;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

float Sonar::getDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  float tUS = pulseIn(echoPin, HIGH);
  if (tUS > 0){
    float t = tUS / 1000.0 / 1000.0 / 2;
    return t * vs;
  } else {
    return NO_OBJECTS;
  }
}