#include "PID_controller.h"

PID_controller::PID_controller()
{

}

float PID_controller::PID_wheel_control(float y_ref,float y, std::string wheel){

    float error = y_ref - y;
    float integral = 0;
    float derivative = 0;

    if(wheel == "left")
    {
        integral = pre_int_left + error * dt;
        pre_int_left = integral;
        derivative = (error - pre_err_left)/dt;
        pre_err_left = error;
    }
    else if(wheel == "right")
    {
        integral = pre_int_right + error * dt;
        pre_int_right = integral;
        derivative = (error - pre_err_right)/dt;
        pre_err_right = error;
    }

    float P = Kp * error;
    float I = (Kp/Ti)*integral;
    float D = Kp * Td * derivative;

    float U = P + I + D;

    if(U>=24){
        return 24.0;
    } else if(U<=-24.0){
        return -24.0;
    } else {
        return U;
    }
}

