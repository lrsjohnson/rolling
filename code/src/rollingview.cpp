#include "rollingview.h"
#include "camera.h"
#include "rollingapp.h"

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include "extra.h"

RollingView::RollingView(int x,
			 int y,
			 int w,
			 int h,
			 const char* label) : Fl_Gl_Window(x, y, w, h, label) {
    camera_ = new Camera();

    // Setup camera!
    camera_->SetDimensions(w, h);
    camera_->SetDistance(2);
    camera_->SetCenter(Vector3f(0, 0, 0));
};

RollingView::~RollingView() {
    delete camera_;
};

int RollingView::handle( int event ) {
  unsigned eventCoordX = Fl::event_x();
  unsigned eventCoordY = Fl::event_y();
  unsigned eventButton = Fl::event_button();
  unsigned eventState  = Fl::event_state();	
  
  switch( event ) {
  case FL_PUSH:
    {
      switch (eventButton)
	{
	case FL_LEFT_MOUSE:
	  camera_->MouseClick( Camera::LEFT, eventCoordX, eventCoordY );
	  break;
	  
	case FL_MIDDLE_MOUSE:
	  camera_->MouseClick( Camera::MIDDLE, eventCoordX, eventCoordY );
	    break;
	    
	case FL_RIGHT_MOUSE:
	  camera_->MouseClick( Camera::RIGHT, eventCoordX, eventCoordY );
	  break;
	}
    }
    break;
    
  case FL_DRAG:
    {
      camera_->MouseDrag(eventCoordX, eventCoordY);
    }
    break;
    
  case FL_RELEASE:
    {
      camera_->MouseRelease(eventCoordX, eventCoordY);
    }
    break;	
    
  case FL_KEYUP:
    {/*
       unsigned key = Fl::event_key();
       
       if( key == 'a' )
       {
       m_drawAxes = !m_drawAxes;
       cout << "drawAxes is now: " << m_drawAxes << endl;
       }
       else if( key == 's' )
       {
       m_drawSkeleton = !m_drawSkeleton;
       cout << "drawSkeleton is now: " << m_drawSkeleton << endl;
       }
       else if( key == 'f' )
       {
       m_drawFloor = !m_drawFloor;
       cout << "drawFloor is now: " << m_drawFloor << endl;
       }
     */}
    break;
    
  default:
    return 0;
  }
  
  redraw();
  
  return 1;
};

void RollingView::update() {
  // update model from UI eetc.
};

void RollingView::draw() {
  if (!valid()) {
    // Setup opengl
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    camera_->SetDimensions(w(), h());
    camera_->SetViewport(0, 0, w(), h());
    camera_->ApplyViewport();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    camera_->SetPerspective(50.0f);
    glLoadMatrixf(camera_->projectionMatrix());
  }

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
  glLoadMatrixf(camera_->viewMatrix());
  
  // Draw the view
  drawAxes();
};

void RollingView::drawAxes()
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable( GL_LIGHTING );
  glLineWidth(3);
  glPushMatrix();
  glScaled(5.0, 5.0, 5.0);
  
  glBegin( GL_LINES );

  // x, y, z axs
  glColor4f(1,0.5,0.5,1); glVertex3f(0,0,0); glVertex3f(1,0,0);
  glColor4f(0.5,1,0.5,1); glVertex3f(0,0,0); glVertex3f(0,1,0);
  glColor4f(0.5,0.5,1,1); glVertex3f(0,0,0); glVertex3f(0,0,1);
  
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



