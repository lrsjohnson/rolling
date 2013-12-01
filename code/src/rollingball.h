#ifndef ROLLING_BALL_H
#define ROLLING_BALL_H

#include "Vector3f.h"

class RollingBall {
 public:
  RollingBall(Vector3f center, float r);
  ~RollingBall();

  void draw();
 private:
  Vector3f center_;
  float radius_;

  static const int RENDER_SLICES;
  static const int RENDER_STACKS;   
};

#endif // ROLLING_BALL_H
