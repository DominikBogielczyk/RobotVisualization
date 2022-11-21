#ifndef ROBOT_H
#define ROBOT_H
#include <headers.h>
#include <trafficcone.h>
#include <PID_controller.h>
#include <drawingfunctions.h>
#include <room.h>

class Robot
{
public:
    const double height = 12.0;
    const double radius = 20.0;
    const double from_ground = 3.0;
    const double wheel_width = 1.0;
    const double wheel_radius = height - from_ground;
    const double track_between_wheels = 10.0;

    float start_x = 450;
    float start_y = 0;
    float x = start_x;
    float y = start_y;
    float prev_x = x;
    float prev_y = y;
    float rot_z_0_360 = 0;
    float rot_z = 0;

    float left_wheel_velocity = 0.0;
    float right_wheel_velocity = 0.0;
    float prev_left_wheel_velocity = 0.0;
    float prev_right_wheel_velocity = 0.0;
    float left_wheel_velocity_ref = 0.0;
    float right_wheel_velocity_ref = 0.0;

    float angular_velocity = 0.0;
    float linear_velocity = 0.0;
    bool collision_front = 0;
    bool collision_rear = 0;
    bool collision_right = 0;
    bool collision_left = 0;
    bool collision = 0;
    bool last_collision = 0;
    bool traffic_cones_collision = 0;

    //MOTORS PARAMETERS
    float Kt = 0.062;
    float Kb = 0.062;
    float J = 0.0551;
    float B =0.188;
    float Re = 0.56;
    float Le = 0.97 * 0.001;

    //TRANFER FUNCTION PARAMETERES
    float a0 = 0.0246;
    float a1 = 1.668;
    float b0 = 0.0;
    float b1 = -0.7021;
    float b2 = 1;

    //LEFT WHEEL
    float u_left = 0;
    float u1_left = 0; //u(n-1)
    float u2_left = 0; //u(n-2)
    float w1_left = 0; //ω(n-1)
    float w2_left = (a1*u1_left - b1*w1_left)/b2; //ω(n-2)

    //RIGHT WHEEL
    float u_right = 0;
    float u1_right = 0;
    float u2_right = 0;
    float w1_right = 0;
    float w2_right = (a1*u1_right - b1*w1_right)/b2;

    float wl_tab[100];
    float wp_tab[100];
    float w_tab[100];
    float v_tab[100];
    float x_tab[100];
    float y_tab[100];

    Robot();
    void object_respond();
    bool traffic_cone_robot_collisions(TrafficCone trafficcone);
    void draw_robot();
    void reset_robot_position();
    void velocity_extraction(std::string text);
    void collisions();
    void robot_movement(sf::Clock clk, float prev_time, double room_width, double room_length);
    bool finish_point_reach(Room room);

};

#endif // ROBOT_H
