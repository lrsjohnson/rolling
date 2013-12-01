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

  rotation = 0;
};

RollingBall::~RollingBall() {
};

void RollingBall::draw() {
  // Save State
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();

  // Adjust sphere rotation
  Vector3f rot_vec = Vector3f(1, 0, 0);
  glRotatef(rotation, rot_vec[0], rot_vec[1], rot_vec[2]);
  
  // Draw Sphere
  glutWireSphere(radius_, RENDER_SLICES, RENDER_STACKS);

  // Revert state, etc.
  glPopMatrix();
  glPopAttrib();
};
