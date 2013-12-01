#include "rollingsim.h"

#include "Vector3f.h"

RollingSimulation::RollingSimulation() {
  world_ = new RollingWorld();
  ball_ = new RollingBall(Vector3f(0,0,0), 1);
};

RollingSimulation::~RollingSimulation() {
  delete world_;
};

void RollingSimulation::draw() {
  world_->draw();
  ball_->draw();
};
