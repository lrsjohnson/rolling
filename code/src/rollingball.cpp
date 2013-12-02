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

  rotation_ = Matrix4f::identity();
  velocity_ = Vector3f(1, 0, 1);

  axis_rotation_ = 0;
};

RollingBall::~RollingBall() {
};

void RollingBall::draw() {
  // Save State
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();

  // Adjust sphere rotation

  glTranslatef(center_[0], center_[1], center_[2]);


  glPushMatrix();
  
  glMultMatrixf(rotation_);
  // Draw Sphere
  //  glShadeMode(SMOOTH);
  //  glutWireSphere(radius_, RENDER_SLICES, RENDER_STACKS);
  glutSolidSphere(radius_, RENDER_SLICES, RENDER_STACKS);


  glPopMatrix();

  // Attempt at shadow
  glTranslatef(0, -radius_, 0);
  glScaled(0.9, 0.02, 0.9);

  GLfloat diffColor[] = {0.0f, 0.0f, 0.0f, 1.f};
  GLfloat specColor[] = {0.1f, 0.1f, 0.1f, 1.f};
  GLfloat shininess[] = {5.0f};

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);    

  glutSolidSphere(radius_, RENDER_SLICES, RENDER_STACKS);  

  // revert colors
  GLfloat diffColor_d[] = {0.4f, 0.4f, 0.4f, 1.f};
  GLfloat specColor_d[] = {0.6f, 0.6f, 0.6f, 1.f};
  GLfloat shininess_d[] = {50.0f};

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColor_d);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor_d);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess_d);    
  
  // Revert state, etc.
  glPopMatrix();
  glPopAttrib();
};
