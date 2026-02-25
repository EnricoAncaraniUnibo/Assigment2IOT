#include "Hangar.h"

Hangar::Hangar(){
  currentState = DRONE_INSIDE;
}

HangarState Hangar::getState(){
  return this->currentState;
}

void Hangar::setState(HangarState s){
  currentState = s;
}