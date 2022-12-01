#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <headers.h>

class PID_controller
{
public:
    // parameters for wheels volicties regulation
    float k = 5.7;
    float T = 0.27;
    float tau = 0.3;
    float Kp = 1.2*T/(tau*k);
    float Ti = 2*tau;
    float Td = 0;
    float dt = 0.015;

    // left wheel PID parameters
    float pre_err_left = 0;
    float pre_int_left = 0;
    float u_left = 0;

    // right wheel PID parameters
    float pre_err_right = 0;
    float pre_int_right = 0;
    float u_right = 0;

    PID_controller();

    // function which return voltages for wheels from PID controller
    float PID_wheel_control(float y_ref,float y, std::string wheel);
};

#endif // PID_CONTROLLER_H
