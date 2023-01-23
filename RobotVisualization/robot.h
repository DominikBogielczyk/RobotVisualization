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
    // parameters of robot size
    const double height = 12.0;
    const double radius = 20.0;
    const double from_ground = 3.0;
    const double wheel_width = 1.0;
    const double wheel_radius = height - from_ground;
    const double track_between_wheels = 10.0;

    // parameters of robot position and orientation
    float start_x = 450;
    float start_y = 0;
    float x = start_x;
    float y = start_y;
    float prev_x = x;
    float prev_y = y;
    float rot_z_0_360 = 0;
    float rot_z = 0;
    float x_ref = 450;
    float y_ref = 0;

    // parameters of robot velocities
    float left_wheel_velocity = 0.0;
    float right_wheel_velocity = 0.0;
    float prev_left_wheel_velocity = 0.0;
    float prev_right_wheel_velocity = 0.0;
    float left_wheel_velocity_ref = 0.0;
    float right_wheel_velocity_ref = 0.0;
    float angular_velocity = 0.0;
    float linear_velocity = 0.0;

    // signals for collision
    bool collision_front = 0;
    bool collision_rear = 0;
    bool collision_right = 0;
    bool collision_left = 0;
    bool collision = 0;
    bool last_collision = 0;
    bool traffic_cones_collision = 0;

    // motors parameters
    float Kt = 0.062;
    float Kb = 0.062;
    float J = 0.0551;
    float B =0.188;
    float Re = 0.56;
    float Le = 0.97 * 0.001;

    // transfer function parameters
    float a0 = 0.0246;
    float a1 = 1.668;
    float b0 = 0.0;
    float b1 = -0.7021;
    float b2 = 1;

    // left wheel
    float u_left = 0;
    float u1_left = 0; //u(n-1)
    float u2_left = 0; //u(n-2)
    float w1_left = 0; //ω(n-1)
    float w2_left = (a1*u1_left - b1*w1_left)/b2; //ω(n-2)

    // right wheel
    float u_right = 0;
    float u1_right = 0;
    float u2_right = 0;
    float w1_right = 0;
    float w2_right = (a1*u1_right - b1*w1_right)/b2;

    // parameters to plot
    float wl_tab[200] = {0.0};
    float wp_tab[200] = {0.0};
    float wlref_tab[200] = {0.0};
    float wpref_tab[200] = {0.0};
    float uL_tab[200] = {0.0};
    float uR_tab[200] = {0.0};
    float x_tab[200] = {0.0};
    float y_tab[200] = {0.0};
    float xref_tab[200] = {0.0};
    float yref_tab[200] = {0.0};

    Robot();

    // function with robot kinematic
    void object_respond();

    // function which check collision between robot and traffic cone
    bool traffic_cone_robot_collisions(TrafficCone trafficcone);

    // function which draw robot
    void draw_robot();

    // function which reset robot position to start position
    void reset_robot_position();

    // function which extrude robot referance velocity or position
    void input_extraction(std::string text,bool pos_control);

    // function which check collision between robot and walls
    void collisions();

    // function which move robot
    void robot_movement(sf::Clock clk, float prev_time, double room_width, double room_length);

    // function which signal if robot reach destination point
    bool finish_point_reach(Room room);

};

#endif // ROBOT_H
