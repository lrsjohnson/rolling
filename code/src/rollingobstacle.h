#ifndef ROLLING_OBSTACLE_H
#define ROLLING_OBSTACLE_H

#include "Vector3f.h"
#include "Vector4f.h"
#include "Matrix4f.h"
#include "extra.h"
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

#include "rollingball.h"

class RollingObstacle {
public:
    RollingObstacle();
    
    virtual void getCollisionsForObstacle(RollingBall* ball, vector<Vector3f>* collision_points) {};
    virtual void draw() {};

    Vector3f closestPtOnTriangle(Vector3f& p, Vector3f& a, Vector3f& b, Vector3f&);
    void getCollisionsForTriangle(Vector3f& p1,
                                  Vector3f& p2,
                                  Vector3f& p3,
                                  RollingBall* ball,
                                  vector<Vector3f>* collision_points);
    
    Vector4f color_;
    Vector4f paint_color_;

};

class BoxObstacle : public RollingObstacle {
 public:
    BoxObstacle(Vector3f center_vec, float w_x, float w_y, float w_z);
    void getCollisionsForObstacle(RollingBall* ball, vector<Vector3f>* collision_points);
    void draw();

    Vector3f center;
    float w_x_;
    float w_y_;
    float w_z_;

    float enclosing_radius;
 private:
    void getCollisionsForFace(RollingBall* ball, vector<Vector3f>* collision_points,
                              Vector3f& center,
                              Vector3f& u_dir, float u,
                              Vector3f& v_dir, float v);
};

#endif // ROLLING_OBSTACLE_H
