#include "rollingapp.h"

#include <iostream>
#include <sys/time.h>

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


void RollingApplication::Init(int argc, char* argv[]) {
    // Initialize UI
    ui_ = new RollingUI();
    timer_running_ = true;
    x = 0;
    gettimeofday(&last_tick_tv_, NULL);

    // Initialize simulation
    rolling_sim_ = new RollingSimulation();
    startTimer();
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
    cout << "Released key num " << key << "." << endl;
    if (key == 32) {
        rolling_sim_->external_vel[1] = 0; // jump
    } else if (key == 65361 || key == 'a') { // LEFT KEY
        rolling_sim_->external_vel[0] = 0;
    } else if (key == 65362 || key == 'w') { // UP KEY
        rolling_sim_->external_vel[2] = 0;
    } else if (key == 65363 || key == 'd') { // RIGHT KEY
        rolling_sim_->external_vel[0] = 0;
    } else if (key == 65364 || key == 's') { // DOWN KEY
        rolling_sim_->external_vel[2] = 0;
    } else if (key == 'r') {
        rolling_sim_->onReset();
    }
};

void RollingApplication::onKeyDown(unsigned key) {
    cout << "Pressed key num " << key << "." << endl;
    if (key == 32) {
        rolling_sim_->external_vel[1] = 1;
    } else if (key == 65361 || key == 'a') { // LEFT KEY
        rolling_sim_->external_vel[0] = -1;
    } else if (key == 65362 || key == 'w') { // UP KEY
        rolling_sim_->external_vel[2] = -1;
    } else if (key == 65363 || key == 'd') { // RIGHT KEY
        rolling_sim_->external_vel[0] = 1;
    } else if (key == 65364 || key == 's') { // DOWN KEY
        rolling_sim_->external_vel[2] = 1;
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
    rolling_sim_->draw();

    // Maybe draw the axes
    //  drawAxes();
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

void timeval_subtract (timeval* result, timeval* x, timeval* y) {
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }
    /* Compute the time remaining to wait.
       tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;
};
 
void RollingApplication::onTick() {
    /*
      timeval now_tv;
      gettimeofday(&now_tv, NULL);
      timeval diff_tv;
      timeval_subtract(&diff_tv, &now_tv, &last_tick_tv_);
      float time_step = diff_tv.tv_sec + diff_tv.tv_usec / 1000000.0;
      last_tick_tv_ = now_tv;
    */

    
    // Update external velocity based on camera direction
    Matrix4f camera_rotation = camera_ref_->GetRotation().inverse();
    Vector4f external_vel = Vector4f(rolling_sim_->external_vel, 0);
    external_vel[1] = 0;
    Vector3f rotated_vel = (camera_rotation * external_vel).xyz();
    rotated_vel[1] = 0; // Ignore vertical velocity
    if (rotated_vel.abs() > 0) {
        rotated_vel = rotated_vel.normalized();
    }
    rolling_sim_->projected_external_vel = rotated_vel;
    rolling_sim_->projected_external_vel[1] = rolling_sim_->external_vel[1];

    // Step the simulation

    Vector3f old_ball_center = rolling_sim_->ball_->center_;
    
    rolling_sim_->step(FPS);

    Vector3f ball_delta = rolling_sim_->ball_->center_ - old_ball_center;
    // Only do x translation
    ball_delta[1] *= 0.5;
    delta_for_camera += ball_delta;
    camera_ref_->SetCenter(camera_ref_->GetCenter() + delta_for_camera * 0.25);
    delta_for_camera = 0.75 * delta_for_camera;
};

void RollingApplication::timer_callback(void* userdata) { // static
    RollingApplication* o = (RollingApplication*) userdata;
    // Do something
    o->onTick();  
    o->redraw();
    if(o->timer_running()) {
        Fl::repeat_timeout(FPS, timer_callback, userdata);
    }
};


void RollingApplication::startTimer() {
    timer_running_ = true;
    gettimeofday(&last_tick_tv_, NULL);  
    Fl::add_timeout(FPS, timer_callback, (void*)this);
};

void RollingApplication::stopTimer() {
    timer_running_ = false;
};
