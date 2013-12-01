#include "rollingball.h"
#include "extra.h"

#include <cassert>

const int RollingBall::RENDER_SLICES = 20;
const int RollingBall::RENDER_STACKS = 15;

RollingBall::RollingBall(Vector3f center, float r) {
  assert(center_);
  assert(r > 0);
  center_ = center;
  radius_ = r;
};

RollingBall::~RollingBall() {
};

void RollingBall::draw() {
  // Save State
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  // Draw Sphere
  glutWireSphere(radius_, RENDER_SLICES, RENDER_STACKS);

  // Revert state, etc.
  glPopAttrib();
};
