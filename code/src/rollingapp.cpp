#include "rollingapp.h"

// Singleton starts null
RollingApplication* RollingApplication::instance_ = NULL;

// Rolling Application Class methods

RollingApplication* RollingApplication::Instance() {
    return (instance_) ? (instance_) : (instance_= new RollingApplication());
};

void RollingApplication::Init(int argc,
					     char* argv[]) {
    // Do lots of stuff
    ui_ = new RollingUI();
};
					     

