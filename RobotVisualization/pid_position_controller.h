#ifndef PID_POSITION_CONTROLLER_H
#define PID_POSITION_CONTROLLER_H
#include <headers.h>

class PID_position_controller
{
public:
    float K_p = 5;
    float K_rot = 10;

    PID_position_controller();
    std::tuple<float,float> PID_position_control_3(float y_ref, float x_ref, float y, float x, float rot, float L, float R);
};

#endif // PID_POSITION_CONTROLLER_H
