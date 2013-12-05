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
    camera_->SetDistance(30);
    camera_->SetCenter(Vector3f(0, 0, 0));
    camera_->SetRotation(Matrix4f::rotation(Vector3f(1, 0, 0), M_PI / 5.0));
    RollingApplication::Instance()->loadCamera(camera_);
    RollingApplication::Instance()->loadView(this);
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
    case FL_PUSH: RollingApplication::Instance()->onMouseClick(eventButton, eventCoordX, eventCoordY); break;
    case FL_DRAG: RollingApplication::Instance()->onMouseDrag(eventCoordX, eventCoordY); break;
    case FL_RELEASE: RollingApplication::Instance()->onMouseRelease(eventCoordX, eventCoordY); break;    
    case FL_KEYDOWN: RollingApplication::Instance()->onKeyDown(Fl::event_key()); break;        
    case FL_KEYUP: RollingApplication::Instance()->onKeyUp(Fl::event_key()); break;
    case FL_FOCUS: break;
    case FL_UNFOCUS: break;        
    default: return 0;
    }
    redraw();
    return 1;
};

void RollingView::update() {
    // update model from UI etc.
};

void RollingView::setupOpenGL() {
    // Setup opengl
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glEnable(GL_COLOR_MATERIAL);    

    camera_->SetDimensions(w(), h());
    camera_->SetViewport(0, 0, w(), h());
    camera_->ApplyViewport();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    camera_->SetPerspective(50.0f);
    glLoadMatrixf(camera_->projectionMatrix());
}

void RollingView::draw() {
    if (!valid()) {
        setupOpenGL();
    }
    RollingApplication::Instance()->draw();
};





