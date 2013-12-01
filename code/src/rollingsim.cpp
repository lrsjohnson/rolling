#include "rollingsim.h"
#include "Vector3f.h"

#include <iostream> 
#include <vector> 

using std::cout;
using std::endl;
using std::vector;

RollingSimulation::RollingSimulation() {
  world_ = new RollingWorld();
  ball_ = new RollingBall(Vector3f(0,0,0), 1);
  stepper_ = new runge_kutta4<system_state_t>();
};

RollingSimulation::~RollingSimulation() {
  delete world_;
  delete ball_;
  delete stepper_;
};

void RollingSimulation::draw() {
  world_->draw();
  ball_->draw();
};

RollingSimulation::system_state_t RollingSimulation::getState() {
  vector<double> state;
  state.push_back(ball_->rotation);
  state.push_back(3.0);
  return state;
};

void RollingSimulation::setState(system_state_t state) {
  ball_->rotation = state[0];
};

void RollingSimulation::step(float time_step) {
  system_state_t out(2);
  stepper_->do_step(system_func, getState(), 0, out, time_step);
  setState(out);
};

void RollingSimulation::system_func(const system_state_t& x,
				    system_state_t& dxdt,
				    const double t) {
  dxdt[0] = x[1];
  dxdt[1] = -1000; // Force on rotation
};
