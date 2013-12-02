#ifndef ROLLING_BALL_H
#define ROLLING_BALL_H

#include "Vector3f.h"
#include "Matrix4f.h"

class RollingBall {
 public:
    RollingBall(Vector3f center, float r);
    ~RollingBall();

    void draw();

    float radius() { return radius_; };

    Matrix4f rotation_;
    float axis_rotation_;
    Vector3f velocity_;
    Vector3f center_;
  
 private:
    float radius_;


  
    static const int RENDER_SLICES;
    static const int RENDER_STACKS;   
};

#endif // ROLLING_BALL_H
