#include "rollingsim.h"
#include "Vector3f.h"
#include "Matrix3f.h"
#include "Matrix4f.h"

#include <iostream> 
#include <vector> 

using std::cout;
using std::endl;
using std::vector;

RollingSimulation::RollingSimulation() {
    world_ = new RollingWorld();
    ball_ = new RollingBall(Vector3f(0,0,0), 1.0);
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
    //  state.push_back(ball_->rotation);
    //state.push_back(3.0);
    return state;
};

void RollingSimulation::setState(system_state_t state) {
    //  ball_->rotation = state[0];
};

const float RollingSimulation::MIN_VELOCITY = 0.0001;

void RollingSimulation::step(float time_step) {
    // 1. update velocities
    ball_->velocity_ *= 0.9;

    // 2. detect collisions after applying velocities; adjust velocities to correct direction

    // 3. move ball, apply rotation

    if (ball_->velocity_.abs() < MIN_VELOCITY) {
        ball_->velocity_ = Vector3f(0,0,0);
        return;
    }

    Vector3f vector_to_contact = Vector3f(0, -1, 0);
    Vector3f torque_vector = -ball_->velocity_;

    Vector3f rotation_axis = Vector3f::cross(vector_to_contact, torque_vector);

    ball_->center_ += ball_->velocity_ * time_step;
  
    float dist_traveled = ball_->velocity_.abs() * time_step;
    float radians_moved = dist_traveled / ball_->radius();

    ball_->rotation_ = Matrix4f::rotation(rotation_axis, radians_moved) * ball_->rotation_;
    // Old, via stepper:
    //  system_state_t out(2);
    //  stepper_->do_step(system_func, getState(), 0, out, time_step);
    //  setState(out);
};

void RollingSimulation::system_func(const system_state_t& x,
				    system_state_t& dxdt,
				    const double t) {
    dxdt[0] = x[1];
    dxdt[1] = -1000; // Force on rotation
};

void RollingSimulation::onLeft() {
    ball_->velocity_ += Vector3f(-1, 0, 0);      
};

void RollingSimulation::onUp() {
    ball_->velocity_ += Vector3f(0, 0, -1);    
};

void RollingSimulation::onRight() {
    ball_->velocity_ += Vector3f(1, 0, 0);  
};

void RollingSimulation::onDown() {
    ball_->velocity_ += Vector3f(0, 0, 1);
};
