#include "pid_position_controller.h"

PID_position_controller::PID_position_controller()
{

}

std::tuple<float,float> PID_position_controller::PID_position_control_3(float y_ref, float x_ref, float y, float x, float rot, float L, float R){
    // CALCULATION OF ROBOT ROTATION IN RADIANS
    float rot_rad = (rot+180) * PI/180;

    // CALCULATATION OF ERRORS FOR POSITION AND ROTATION CONTROLLERS
    float error_rot = atan2(y_ref-y,x_ref-x) - rot_rad;
    float error_s = sqrt(pow(y_ref-y,2)+pow(x_ref-x,2));

    // CALCULATE REFERENCE LINEAR AND ANGULAR VELOCITIES USING LYAPUNOV STABILITY
    float v = K_p * error_s * cos(error_rot);
    float w = K_p * (cos(error_rot)*sin(error_rot))+ K_rot * error_rot;

    // CALCULATE REFERENCE VELOCITIES OF LEFT AND RIGHT WHEELS USING REFERENCE LINEAR AND ANGULAR VELOCITIES
    float w_l = (2*v - L * w)/(2*R);
    float w_p = (2*v + L* w)/(2*R);

    return {w_l,w_p};
}
