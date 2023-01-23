#ifndef PID_POSITION_CONTROLLER_H
#define PID_POSITION_CONTROLLER_H
#include <headers.h>

class Position_controller
{
public:
    // parameters for position controller
    float K_p = 5;
    float K_rot = 12;
    float prev_err = -1;
    bool stop = 0;
    int position_counter_x = 1;
    int position_counter_y = 1;
    const int max_position_counter = 10;
    float start_x = 450;
    float start_y = 0;

    Position_controller();

    // parameters which return wheels referance angular velocities from position regulator
    std::tuple<float,float> position_control(float y_ref, float x_ref, float y, float x, float rot, float L, float R);
};

#endif // PID_POSITION_CONTROLLER_H
