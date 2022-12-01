#include "pid_position_controller.h"

PID_position_controller::PID_position_controller()
{

}

std::tuple<float,float> PID_position_controller::PID_position_control_3(float y_ref, float x_ref, float y, float x, float rot, float L, float R){

    // trajectory generate
    if(stop==0){
    y_ref = this->start_y + (y_ref - this->start_y) * this->position_counter_y / this->max_position_counter;
    x_ref = this->start_x + (x_ref - this->start_x) * this->position_counter_x / this->max_position_counter;

    if(abs(y_ref-y) < 10 and this->position_counter_y < this->max_position_counter)
        this->position_counter_y ++;
    if(abs(x_ref-x) < 10 and this->position_counter_x < this->max_position_counter)
        this->position_counter_x ++;
    }

    // robot rotation in radians
    float rot_rad = (rot+180) * PI/180;

    // orientation and position errors for position control
    float error_rot = (atan2(y_ref-y,x_ref-x) - rot_rad);
    float error_s = sqrt(pow(y_ref-y,2)+pow(x_ref-x,2));

    // calculate reference linear and angular velocities using laypunov stability
    float v = K_p * error_s * cos(error_rot);
    float w = K_p * (cos(error_rot)*sin(error_rot))+ K_rot * error_rot;

    // calculate left and right wheel referance angular velocities
    float w_l = (2*v - L/2 * w)/(2*R);
    float w_p = (2*v + L/2 * w)/(2*R);

    // stop robot when it reach destination point
    if((abs(error_s)<1 && abs(prev_err-error_s)<0.1) || stop == 1){
        this -> position_counter_x = 1;
        this -> position_counter_y = 1;
        stop = 1;
        w_l = 0;
        w_p = 0;
        prev_err = -1;
    } else {
        prev_err = error_s;
    }

    // return angular velocities for wheels
    return {w_l,w_p};
}
