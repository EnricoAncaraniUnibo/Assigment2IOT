#ifndef __PIR__
#define __PIR__

class Pir {
 
public: 
  Pir(int pin);
  bool isDetected();
  void calibrate();

  void sync();

private:
  int pin;
  bool detected;
};

#endif
