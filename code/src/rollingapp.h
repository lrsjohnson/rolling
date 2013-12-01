#ifndef ROLLINGAPP_H
#define ROLLINGAPP_H

#include "camera.h"
#include "rollingview.h"
#include "rollingui.h"
#include "rollingsim.h"

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
    void onMouseClick(unsigned button, int x, int y);
    void onMouseDrag(int x, int y);    
    void onMouseRelease(int x, int y);

    // Get references
    void loadCamera(Camera* camera);
    void loadView(RollingView* view);

    // Timer callbacks
    static void timer_callback(void* userdata);

    void step();
    void start_timer();
    void stop_timer();

    inline bool animating() { return animating_; };


 private:
    // Singleton instance
    static RollingApplication* instance_;

    // Simulation reference
    RollingSimulation* rolling_sim_;
    
    // User Interface Members
    friend class RollingUI;
    RollingUI* ui_;
    void drawAxes();

    bool animating_;
    
    // RollingApplication does not own camera_ref_ or view_ref_
    Camera* camera_ref_;
    RollingView* view_ref_;

    float x;
};
 
#endif // ifndef ROLLINGAPP_H
