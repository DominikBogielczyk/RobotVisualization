#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <headers.h>

class PID_controller
{
public:
    float k = 5.7;
    float T = 0.27;
    float tau = 0.3;
    float Kp = 1.2*T/(tau*k);
    float Ti = 2*tau;
    float Td = 0;
    float dt = 0.1;

    //LEFT WHEEL
    float pre_err_left = 0;
    float pre_int_left = 0;
    float u_left = 0;

    //RIGHT WHEEL
    float pre_err_right = 0;
    float pre_int_right = 0;
    float u_right = 0;

    PID_controller();
    float PID_control(float y_ref,float y, std::string wheel);
};

#endif // PID_CONTROLLER_H
