#ifndef __HANGAR__
#define __HANGAR__

enum HangarState {
  DRONE_INSIDE,
  TAKING_OFF,
  DRONE_OUT,
  LANDING,
  PRE_ALARM,
  ALARM
};

class Hangar {

public:
  Hangar();

  HangarState getState();
  void setState(HangarState s);

private:

  HangarState currentState;
};

#endif
