#ifndef ROLLINGAPP_H
#define ROLLINGAPP_H

#include "rollingview.h"
#include "rollingui.h"

/**
 * RollingApplication is a singleton.
 */
class RollingApplication {
 public:
    ~RollingApplication();

    static RollingApplication* Instance();

    void Init(int argc, char* argv[]);

    int Run();


 private:
    static RollingApplication* instance_;

    friend class RollingUI;

    RollingUI* ui_;
};
 

#endif // ifndef ROLLINGAPP_H
