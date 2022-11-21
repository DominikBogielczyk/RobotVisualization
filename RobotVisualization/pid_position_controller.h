#ifndef PID_POSITION_CONTROLLER_H
#define PID_POSITION_CONTROLLER_H
#include <headers.h>

class PID_position_controller
{
public:
    float k = 0.57;
    float T = 0.5;
    float tau = 10;
    //float Kp = 1.2*T/(tau*k);
    //float Ti = 4*tau;
    //float Td = 0;
    float Kp = 1;
    float Ti = 1;
    float Td = 1;
    //float dt = 0.015;

    //POSITION ERROR PARAMETERS
    float pre_err_s = 0;
    float pre_int_s = 0;

    //ROTATION ERROR PARAMETERS
    float pre_err_rot = 0;
    float pre_int_rot = 0;

    float error_rot = 0;
    float error_rot_1 = 0;
    float error_rot_2 = 0;

    PID_position_controller();
    std::tuple<float,float> PID_position_control(float y_ref, float x_ref, float y, float x,float rot, std::string wheel,float dt);
    std::tuple<float,float> PID_position_control_2(float y_ref, float x_ref, float y, float x,float rot,float x0,float y0,float rot0, std::string wheel,float dt);
};

#endif // PID_POSITION_CONTROLLER_H
