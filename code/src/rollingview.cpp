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
};

RollingView::~RollingView() {
    delete camera_;
};

int RollingView::handle( int event )
{
  unsigned eventCoordX = Fl::event_x();
  unsigned eventCoordY = Fl::event_y();
  unsigned eventButton = Fl::event_button();
  unsigned eventState  = Fl::event_state();	
  
  switch( event )
    {
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
    // Draw the view
    drawAxes();
};

void RollingView::drawAxes()
{
  glDisable( GL_LIGHTING );
  glBegin( GL_LINES );
  
  glColor3f( 1, 0, 0 );
  glVertex3f( 0, 0, 0 );
  glVertex3f( 1, 0, 0 );
  
  glColor3f( 0, 1, 0 );
  glVertex3f( 0, 0, 0 );
  glVertex3f( 0, 1, 0 );
  
  glColor3f( 0, 0, 1 );
  glVertex3f( 0, 0, 0 );
  glVertex3f( 0, 0, 1 );
  
  glEnd();
  glEnable( GL_LIGHTING );
};



