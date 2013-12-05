#include "rollingsim.h"
#include "Vector3f.h"
#include "Matrix3f.h"
#include "Matrix4f.h"

#include <iostream>
#include <vector>
#include <stdlib.h>

using std::cout;
using std::endl;
using std::vector;

RollingSimulation::RollingSimulation() {
    world_ = new RollingWorld();
    ball_ = new RollingBall(Vector3f(0, 2.0, 0), 1.0);
    stepper_ = new runge_kutta4<system_state_t>();

    gravity_ = Vector3f(0, -40, 0);
};

RollingSimulation::~RollingSimulation() {
    delete world_;
    delete ball_;
    delete stepper_;
};

void RollingSimulation::draw(Vector3f clickedPoint) {
    world_->sphere_c = ball_->center_;
    world_->draw();
    ball_->draw();

    // Drawing a cube at last clicked point.
    glPushMatrix();
    glTranslatef(clickedPoint[0], clickedPoint[1], clickedPoint[2]);
    glutSolidCube(1);
    glPopMatrix();
    world_->handleClick(clickedPoint);
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

const float RollingSimulation::MIN_VELOCITY = 0.28;

void RollingSimulation::step(float time_step) {

    // 1. detect collisions; adjust/project velocities to correct direction
    vector<Vector3f> collision_points;
    world_->getCollisions(ball_, &collision_points);

    ball_->collision_points = collision_points;

    // 2. update velocities

    // dampening/friction
    ball_->velocity_ *= 0.95;
    ball_->velocity_ += time_step * gravity_;

    ball_->velocity_ += time_step * 30 * Vector3f(projected_external_vel[0], 0, projected_external_vel[2]);
    if (collision_points.size() > 0) {
        // Jump
        ball_->velocity_ += time_step * 500 * Vector3f(0, projected_external_vel[1], 0);
        Vector3f avg_collision = Vector3f::ZERO;
        int num_collisions = collision_points.size();
        float max_penetration = 0;
        Vector3f max_penetration_vec(0, 0, 0);
        int num_collisions_in_v_direction = 0;
        for (int i = 0; i < num_collisions; i++) {
            Vector3f v_center_to_collision = (collision_points[i] - ball_->center_);
            if (Vector3f::dot(v_center_to_collision, ball_->velocity_) > 0) {
                avg_collision += collision_points[i];
                num_collisions_in_v_direction++;
                float penetration_dist = ball_->radius()*1.1 - v_center_to_collision.abs();
                if (penetration_dist > max_penetration) {
                    max_penetration = penetration_dist;
                    max_penetration_vec = v_center_to_collision;
                }
            }
        }
        avg_collision =  avg_collision / (1.0f * num_collisions_in_v_direction);
        ball_->avg_collision = avg_collision - ball_->center_;

        // 3. Account for collision by moving ball up
        Vector3f collision_unit_normal = (avg_collision - ball_->center_).normalized();

        float vel_dot = Vector3f::dot(ball_->velocity_, collision_unit_normal);
        if (vel_dot > 0) {
            ball_->n_vel_ = vel_dot * collision_unit_normal;
            ball_->t_vel_ = ball_->velocity_ - ball_->n_vel_;
        } else {
            ball_->n_vel_ = Vector3f::ZERO;
            ball_->t_vel_ = ball_->velocity_;
        }
        if (max_penetration > 0.1 * ball_->radius()) {
            ball_->t_vel_ -= 10.0 * (max_penetration_vec + collision_unit_normal) * (max_penetration * max_penetration * max_penetration);
        }
        if (ball_->n_vel_.abs() > 3) {
            ball_->t_vel_ -= min(ball_->n_vel_.abs() - 3.0, 5.0) * ball_->n_vel_.normalized();
        }
    } else {
        ball_->avg_collision = Vector3f::ZERO;
        ball_->n_vel_ = Vector3f::ZERO;
        ball_->t_vel_ = ball_->velocity_;
    }

        
    // 4. move ball, apply rotation
    if (ball_->t_vel_.abs() < MIN_VELOCITY) {
        ball_->t_vel_ = ball_->t_vel_ * 0.2;
        ball_->velocity_ = ball_->t_vel_;
        return;
    }

    //    ball_->t_vel_ = ball_->t_vel_.normalized() * ball_->velocity_.abs();
    ball_->velocity_ = ball_->t_vel_;

    Vector3f vector_to_contact = ball_->avg_collision;
    Vector3f torque_vector = -ball_->t_vel_;

    Vector3f rotation_axis = Vector3f::cross(vector_to_contact, torque_vector);

    ball_->center_ += ball_->t_vel_ * time_step;
  
    float dist_traveled = ball_->t_vel_.abs() * time_step;
    float radians_moved = dist_traveled / ball_->radius();

    if (rotation_axis.abs() > 0) {
        Matrix4f rotation_delta = Matrix4f::rotation(rotation_axis, radians_moved);
        ball_->rotation_ = rotation_delta * ball_->rotation_;
        ball_->last_rotation_delta = rotation_delta;
    } else {
        ball_->rotation_ = ball_->last_rotation_delta * ball_->rotation_;
    }
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
    ball_->velocity_ += Vector3f(-3, 0, 0);      
};

void RollingSimulation::onUp() {
    ball_->velocity_ += Vector3f(0, 0, -3);    
};

void RollingSimulation::onRight() {
    ball_->velocity_ += Vector3f(3, 0, 0);  
};

void RollingSimulation::onDown() {
    ball_->velocity_ += Vector3f(0, 0, 3);
};

void RollingSimulation::onUpward() {
    ball_->velocity_ += Vector3f(0, 1, 0);
};

void RollingSimulation::onDownward() {
    ball_->velocity_ += Vector3f(0, -1, 0);
};

Vector3f RollingSimulation::onReset() {
    float new_height = world_->height_at_xz(0, 0) + 20.0;
    cout << new_height << endl;
    ball_->center_ = Vector3f(0, new_height, 0);
    ball_->velocity_ = Vector3f(0, 0, 0);
    return ball_->center_;
};
