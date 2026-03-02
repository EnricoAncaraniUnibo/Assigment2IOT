#ifndef __TASK__
#define __TASK__

class Task {

public:
  Task(){
    active = false;
  }

  virtual void init(){
    active = true;
  }

  virtual void tick() = 0;

  bool isActive(){
    return active;
  }

  void setActive(bool active){
    this->active = active;
  }
  
private:
  bool active;
};

#endif
