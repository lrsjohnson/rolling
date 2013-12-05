#ifndef ROLLINGAPP_H
#define ROLLINGAPP_H

#include "camera.h"
#include "rollingview.h"
#include "rollingui.h"
#include "rollingsim.h"

#include <sys/time.h>

/**
 * RollingApplication is a singleton.
 */
class RollingApplication {
 public:
    ~RollingApplication();

    static RollingApplication* Instance();

    void Init(int argc, char* argv[]);
    int Run();

    void draw();
    // Trigger redrawing in the view
    void redraw();
    
    void onKeyUp(unsigned key);
    void onKeyDown(unsigned key);    
    void onMouseClick(unsigned button, int x, int y);
    void onMouseDrag(int x, int y);    
    void onMouseRelease(int x, int y);

    // Get references
    void loadCamera(Camera* camera);
    void loadView(RollingView* view);

    // Timer callbacks
    static void timer_callback(void* userdata);


    void updateMode();
    
    void onTick();
    void startTimer();
    void stopTimer();

    inline bool timer_running() { return timer_running_; };

    Vector3f delta_for_camera;

 private:
    bool camera_mode_;

    // Singleton instance
    static RollingApplication* instance_;

    // Simulation reference
    RollingSimulation* rolling_sim_;
    
    // User Interface Members
    friend class RollingUI;
    RollingUI* ui_;
    void drawAxes();

    // Timer
    bool timer_running_;
    timeval last_tick_tv_;
    
    // RollingApplication does not own camera_ref_ or view_ref_
    Camera* camera_ref_;
    RollingView* view_ref_;

    float x;
};
 
#endif // ifndef ROLLINGAPP_H
