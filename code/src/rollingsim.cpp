#include "rollingsim.h"

#include "Vector3f.h"

#include <iostream>

using std::cout;
using std::endl;

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

void RollingSimulation::step(float time_step) {
  ball_->rotation += 360 * time_step / 3.0;
};
