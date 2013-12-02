#include "rollingworld.h"

#include "Vector3f.h"
#include "extra.h"

int s = 8;

void RollingWorld::draw() {
  // Save State
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();

  // Adjust sphere rotation

  glTranslatef(0, -1.0, 0);

  glBegin(GL_TRIANGLES);

  glNormal(Vector3f(0, 1, 0));
  glVertex(Vector3f(s, 0, s));

  glNormal(Vector3f(0, 1, 0));
  glVertex(Vector3f(-s, 0, s));

  glNormal(Vector3f(0, 1, 0));
  glVertex(Vector3f(-s, 0, -s));


  glNormal(Vector3f(0, 1, 0));
  glVertex(Vector3f(-s, 0, -s));

  glNormal(Vector3f(0, 1, 0));
  glVertex(Vector3f(s, 0, -s));

  glNormal(Vector3f(0, 1, 0));
  glVertex(Vector3f(s, 0, s));


  glEnd();

  // Revert state, etc.
  glPopMatrix();
  glPopAttrib();
};
