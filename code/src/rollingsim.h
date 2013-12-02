#ifndef ROLLING_SIM_H
#define ROLLING_SIM_H

#include "rollingworld.h"
#include "rollingball.h"

#include <boost/numeric/odeint.hpp> 
#include <vector> 

using std::vector;
using boost::numeric::odeint::runge_kutta4;

class RollingSimulation {
 public:
    RollingSimulation();
    ~RollingSimulation();
  
    typedef vector<double> system_state_t;
  
    void draw();
    void step(float time_step);

    system_state_t getState();
    void setState(system_state_t);  

    void onLeft();
    void onUp();
    void onRight();
    void onDown();  
  
    // Call for the boost runge-kutta stepper
    static void system_func(const system_state_t& x,
                            system_state_t& dxdt,
                            const double t);
  
 private:
    RollingWorld* world_;
    RollingBall* ball_;
    runge_kutta4<system_state_t>* stepper_;
};

#endif // ifndef ROLLING_SIM_H
