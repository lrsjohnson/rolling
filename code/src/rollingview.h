#ifndef ROLLINGVIEW_H
#define ROLLINGVIEW_H

#include <FL/Fl_Gl_Window.H>
#include "extra.h"

class Camera;

class RollingView : public Fl_Gl_Window {
 public:
    RollingView(int x, int y, int w, int h, const char* label = 0);

    virtual ~RollingView();

    virtual int handle(int event);
    virtual void update();
    virtual void draw();

 private:
    Camera* camera_;

    void drawAxes();
};
#endif // ifndef ROLLINGVIEW_H
