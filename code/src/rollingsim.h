#ifndef ROLLING_SIM_H
#define ROLLING_SIM_H

#include "rollingworld.h"
#include "rollingball.h"

class RollingSimulation {
 public:
  RollingSimulation();
  ~RollingSimulation();

  void draw();
  void step(float time_step);
  
 private:
  RollingWorld* world_;
  RollingBall* ball_;
};

#endif // ifndef ROLLING_SIM_H
