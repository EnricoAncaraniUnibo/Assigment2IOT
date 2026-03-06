#ifndef __SONAR__
#define __SONAR__

#define NO_OBJECTS -1

class Sonar {
public:
  Sonar(int trigPin, int echoPin, float temperature);
  float getDistance();

private:
  int trigPin;
  int echoPin;
  float vs;
};

#endif