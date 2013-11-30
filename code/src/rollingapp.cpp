#include "rollingapp.h"

#include <iostream>

using std::cout;
using std::endl;

#define FPS (1.0/30.0)

// Singleton starts null
RollingApplication* RollingApplication::instance_ = NULL;

// Rolling Application Class methods

RollingApplication* RollingApplication::Instance() {
    return (instance_) ? (instance_) : (instance_= new RollingApplication());
};

RollingApplication::~RollingApplication() {
    // FLTK handles widget deletion
    delete ui_;
};


void RollingApplication::Init(int argc,
			      char* argv[]) {
    // Do lots of stuff
    ui_ = new RollingUI();

    animating_ = false;
    x = 0;
};

int RollingApplication::Run() {
    Fl::visual(FL_RGB | FL_DOUBLE);
    ui_->show();
    return Fl::run();
};

void RollingApplication::loadCamera(Camera* camera) {
  camera_ref_ = camera;
};

void RollingApplication::loadView(RollingView* view) {
  view_ref_ = view;
};

/* Events from UI */

void RollingApplication::onKeyUp(unsigned key) {
  cout << "Pressed key num " << key << "." << endl;
  if (key == 32) {
    if (!animating_) {
      start_timer();
    } else {
      stop_timer();
    }
  }
};

void RollingApplication::onMouseClick(unsigned button, int x, int y) {
  Camera::Button camera_button;
  switch (button) {
  case FL_LEFT_MOUSE: camera_button = Camera::LEFT; break;
  case FL_MIDDLE_MOUSE: camera_button = Camera::MIDDLE; break;
  case FL_RIGHT_MOUSE: camera_button = Camera::RIGHT; break;
  default: return;
  }
  camera_ref_->MouseClick(camera_button, x, y);
};

void RollingApplication::onMouseDrag(int x, int y) {
  camera_ref_->MouseDrag(x, y);
};

void RollingApplication::onMouseRelease(int x, int y) {
  camera_ref_->MouseRelease(x, y);  
};

/* Draw scene */
void RollingApplication::draw() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Light color
  GLfloat Lt0diff[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat Lt0pos[] = {3.0, 3.0, 5.0, 1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
  glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

  // Default materials
  GLfloat diffColor[] = {0.4f, 0.4f, 0.4f, 1.f};
  GLfloat specColor[] = {0.6f, 0.6f, 0.6f, 1.f};
  GLfloat shininess[] = {50.0f};

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);    
  
  // Load camera view matrix
  glLoadMatrixf(camera_ref_->viewMatrix());
  
  // Draw the scene
  drawAxes();
  
};


void RollingApplication::drawAxes()
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable( GL_LIGHTING );
  glLineWidth(3);
  glPushMatrix();
  glScaled(5.0, 5.0, 5.0);
  
  glBegin( GL_LINES );

  // x, y, z axs
  glColor4f(1,0.5,0.5,1); glVertex3f(0,0,0); glVertex3f(1, 0, 0);
  glColor4f(0.5,1,0.5,1); glVertex3f(0,0,0); glVertex3f(x, 1, 0);
  glColor4f(0.5,0.5,1,1); glVertex3f(0,0,0); glVertex3f(x, 0, 1);
  
  // negative axes
  glColor4f(0.5,0.5,0.5,1);
  glVertex3f(0,0,0); glVertex3f(-1,0,0);
  glVertex3f(0,0,0); glVertex3f(0,-1,0);
  glVertex3f(0,0,0); glVertex3f(0,0,-1);
  
  glEnd();
  glPopMatrix();
  glEnable( GL_LIGHTING );

  glPopAttrib();
};

void RollingApplication::redraw() {
  view_ref_->redraw();
};

void RollingApplication::step() {
  x += 0.01;
  cout << x << endl;
};

void RollingApplication::timer_callback(void* userdata) {
  RollingApplication* o = (RollingApplication*) userdata;
  cout << "timer callback" << endl;
  // Do something
  o->step();  
  o->redraw();

  if(o->animating()) {
    Fl::repeat_timeout(FPS, timer_callback, userdata);
  }
};


void RollingApplication::start_timer() {
  animating_ = true;
  Fl::add_timeout(FPS, timer_callback, (void*)this);
};

void RollingApplication::stop_timer() {
  animating_ = false;
};
