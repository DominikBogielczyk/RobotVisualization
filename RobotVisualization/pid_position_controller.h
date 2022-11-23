#ifndef PID_POSITION_CONTROLLER_H
#define PID_POSITION_CONTROLLER_H
#include <headers.h>

class PID_position_controller
{
public:
    float K_p = 5;
    float K_rot = 10;
    float prev_err = -1;
    bool stop = 0;
    int position_counter_x = 1;
    int position_counter_y = 1;
    int max_position_counter = 10;
    float start_x = 450;
    float start_y = 0;


    PID_position_controller();
    std::tuple<float,float> PID_position_control_3(float y_ref, float x_ref, float y, float x, float rot, float L, float R);
};

#endif // PID_POSITION_CONTROLLER_H
