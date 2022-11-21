#include "pid_position_controller.h"

PID_position_controller::PID_position_controller()
{

}

std::tuple<float,float> PID_position_controller::PID_position_control(float y_ref, float x_ref, float y, float x,float rot, std::string wheel,float dt){

    // CALCULATION OF ROBOT ROTATION IN RADIANS
    float rot_rad = (rot+180) * PI/180;

    // CALCULATATION OF ERRORS FOR POSITION AND ROTATION CONTROLLERS
    float error_rot = atan2(y_ref-y,x_ref-x) - rot_rad;
    float error_s = sqrt(pow(y_ref-y,2)+pow(x_ref-x,2)) * cos(error_rot);

    // PID FOR POSITION CONTROL
    float integral_s = 0;
    float derivative_s = 0;

    integral_s = pre_int_s + error_s * dt;
    pre_int_s = integral_s;
    derivative_s = (error_s - pre_err_s)/dt;
    pre_err_s = error_s;

    float P_s = 1.2*Kp * error_s;
    float I_s = 0*(Kp/Ti)*integral_s;
    float D_s = 0.15*Kp * Td * derivative_s;

    float w_s = P_s + I_s + D_s;

    // PID FOR ROTATION CONTROL
    float integral_rot = 0;
    float derivative_rot = 0;

    integral_rot = pre_int_rot + error_rot * dt;
    pre_int_rot = integral_rot;
    derivative_rot = (error_rot - pre_err_rot)/dt;
    pre_err_rot = error_rot;

    float P_rot = 4*Kp * error_rot;
    float I_rot = 0*(Kp/Ti)*integral_rot;
    float D_rot = 0.32*Kp * Td * derivative_rot;

    float w_rot = P_rot + I_rot + D_rot;

    float U = 0;

    // CALCULATE VOLTAGE OF WHEELS
    float U_left = w_s - w_rot;
    float U_right = w_s + w_rot;

        if(U_left>=24){
            U_left = 24.0;
        } else if(U_left<=-24.0){
            U_left = -24.0;
        }

        if(U_right>=24){
            U_right = 24.0;
        } else if(U_right<=-24.0){
            U_right = -24.0;
        }

        return {U_left,U_right};
}

int sgn(float v) {
  return (v > 0) - (v < 0);
}

std::tuple<float,float> PID_position_controller::PID_position_control_2(float y_ref, float x_ref, float y, float x,float rot,float x0,float y0,float rot0, std::string wheel,float dt){

   float rot_ref = atan2(y_ref-y0,x_ref-x0);
   rot = rot + 180;
   error_rot_2 = error_rot_1;
   error_rot_1 = error_rot;
   error_rot = rot_ref - (rot*PI/180);
   float error_s = sqrt(pow(y_ref-y,2)+pow(x_ref-x,2))*sgn(atan2(y_ref - y,x_ref-x));
   float u_left = 0;
   float u_right = 0;

   std::cout<<error_rot<<" "<<error_s<<std::endl;
   if(abs(error_rot)>=0.00001){// || abs(error_rot_1)>=0.00001 || abs(error_rot_2)>=0.00001){
       float integral_rot = 0;
       float derivative_rot = 0;

       integral_rot = pre_int_rot + error_rot * dt;
       pre_int_rot = integral_rot;
       derivative_rot = (error_rot - pre_err_rot)/dt;
       pre_err_rot = error_rot;

       float P_rot = Kp * error_rot;
       float I_rot = (Kp/Ti)*integral_rot;
       float D_rot = Kp * Td * derivative_rot;

       float w_rot = P_rot + I_rot + D_rot;

       u_left = w_rot;
       u_right = -w_rot;
   } else if(abs(error_rot)<0.00001 ){//&& abs(error_rot_1)<0.00001 && abs(error_rot)<0.00001){
       float integral_s = 0;
       float derivative_s = 0;

       integral_s = pre_int_s + error_s * dt;
       pre_int_s = integral_s;
       derivative_s = (error_s - pre_err_s)/dt;
       pre_err_s = error_s;

       float P_s = Kp * error_s;
       float I_s = (Kp/Ti)*integral_s;
       float D_s = Kp * Td * derivative_s;

       float w_s = P_s + I_s + D_s;
       u_left = 0.01*w_s;
       u_right = 0.0*w_s;
   }

   if(u_left>=24){
       u_left = 24.0;
   } else if(u_left<=-24.0){
       u_left = -24.0;
   }

   if(u_right>=24){
       u_right = 24.0;
   } else if(u_right<=-24.0){
       u_right = -24.0;
   }

   return {u_left,u_right};
}
