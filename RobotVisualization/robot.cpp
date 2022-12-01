#include "robot.h"

Robot::Robot()
{

}

void Robot::object_respond(){
     // object respond for wheels angular velocities
     left_wheel_velocity = (a1*u1_left + a0*u2_left - b1*w1_left - b0 * w2_left)/b2;
     right_wheel_velocity = (a1*u1_right + a0*u2_right - b1*w1_right - b0 * w2_right)/b2;

     // write past states
     w2_left = w1_left;
     w1_left = left_wheel_velocity;
     u2_left = u1_left;
     u1_left = u_left;

     w2_right = w1_right;
     w1_right = right_wheel_velocity;
     u2_right = u1_right;
     u1_right = u_right;
}

bool Robot::traffic_cone_robot_collisions(TrafficCone trafficcone) {
  // chceck coolision between robot and traffic cone
  if (sqrt(pow(x - trafficcone.pos_x, 2) + pow(y - trafficcone.pos_y, 2)) <= radius + trafficcone.r_down) {
    return true;
  } else {
    return false;
  }
}

void Robot::draw_robot() {

  // box
  glColor3f(50.f / 255, 50.f / 255, 255.f/ 255);
  draw_cube(radius, 2 / 1.5 * radius, 2 * wheel_radius - from_ground - 1, wheel_radius + from_ground);
  // left wheel
  draw_circle(0, -radius / 1.25, wheel_radius, wheel_width, 0.0, 'b'); //int x, int y, double radius, double width, double rot, char color
  // wheel axes
  draw_circle(0, -radius / 1.25, 2.0, 2 * radius / 1.25, 0.0, 'b', wheel_radius);
  // right wheel
  draw_circle(0, radius / 1.25, wheel_radius, wheel_width, 0.0, 'b');
  // middle wheel
  draw_circle(-radius / 1.5, 0, 3.0, wheel_width, 0.0, 'b');
  // transparent
  draw_circle(0, -radius / 1.25 - 7.5, wheel_radius, 10, 0.0, 't', 0.0); //int x, int y, double radius, double width, double rot, char color
  draw_circle(0, radius / 1.25 - 2.5, wheel_radius, radius, 0.0, 't');
  draw_circle(-radius / 1.5, -wheel_width, 4.5, 3 * wheel_width, 0.0, 't');
  // bottom
  draw_circle(0, from_ground, radius, 2.0, 90.0, 'g', -radius); //(int x, int y, double radius, double width)
  // top
  draw_circle(0, 2 * wheel_radius + 1, radius, 2.0, 0.0, 'g', -radius);
  glPopMatrix();

}


void Robot::reset_robot_position(){
    // restart robot velocities, position and voltages to start state
    x = start_x;
    y = start_y;
    rot_z = 0;
    prev_x = x;
    prev_y = y;
    left_wheel_velocity = 0.0;
    right_wheel_velocity = 0.0;
    prev_left_wheel_velocity = 0.0;
    prev_right_wheel_velocity = 0.0;
    left_wheel_velocity_ref = 0.0;
    right_wheel_velocity_ref = 0.0;
    u_left = 0;
    u_right = 0;
}


void Robot::input_extraction(std::string text,bool pos_control) {
  // extract from bluetooth message velocities of both wheels
  if (text.find("setA") != std::string::npos && text.find("setB") != std::string::npos) {
    std::regex re1(R"((setA)(.*)(setB))");
    std::regex re2(R"((setB)(.*))");
    std::smatch sm1;
    std::smatch sm2;

    std::regex_search(text, sm1, re1);
    std::regex_search(text, sm2, re2);

    prev_left_wheel_velocity = left_wheel_velocity;
    prev_right_wheel_velocity = right_wheel_velocity;

    // referance position extraction if position control or referance velocity extraction if velocity control
    if(pos_control == 0){
        left_wheel_velocity_ref = std::stof(sm1[2]);
        right_wheel_velocity_ref = std::stof(sm2[2]);
    } else if(pos_control == 1){
        left_wheel_velocity_ref = 0;
        right_wheel_velocity_ref = 0;
        x_ref = std::stof(sm1[2]);
        y_ref = std::stof(sm2[2]);

    }
  }
}

void Robot::collisions() {
    // convert rotation for 0-360 degrees only
    if(rot_z>=0){
        rot_z_0_360 = (int)rot_z%360;
        } else {
            rot_z_0_360 = abs((int)(360+rot_z)%360);
        }


  // check if robot ride away from wall if true there is no more collision if false there is still collision
  if (collision_front == 1) {
    if (linear_velocity > 0 && (rot_z_0_360 < 90 || rot_z_0_360 > 270)) {
      collision_front = 0;
    } else if (linear_velocity < 0 && rot_z_0_360 > 90 && rot_z_0_360 < 270) {
      collision_front = 0;
    } else {
      x = prev_x;
    }
  }
  if (collision_rear == 1) {
    if (linear_velocity > 0 && rot_z_0_360 > 90 && rot_z_0_360 < 270) {
      collision_rear = 0;
    } else if (linear_velocity < 0 && (rot_z_0_360 < 90 || rot_z_0_360 > 270)) {
      collision_rear = 0;
    } else {
      x = prev_x;
    }
  }
  if (collision_right == 1) {
    if (linear_velocity > 0 && rot_z_0_360 < 180) {
      collision_right = 0;
    } else if (linear_velocity < 0 && rot_z_0_360 > 180) {
      collision_right = 0;
    } else {
      y = prev_y;
    }
  }
  if (collision_left == 1) {
    if (linear_velocity > 0 && rot_z_0_360 > 180) {
      collision_left = 0;
    } else if (linear_velocity < 0 && rot_z_0_360 < 180) {
      collision_left = 0;
    } else {
      y = prev_y;
    }
  }
}

void Robot::robot_movement(sf::Clock clk, float prev_time, double room_width, double room_length) {

  // conversion from right and left wheels velocities to angular and linear velocities
  linear_velocity = (right_wheel_velocity + left_wheel_velocity) * wheel_radius / 2;
  angular_velocity = (right_wheel_velocity - left_wheel_velocity) * wheel_radius / track_between_wheels;

  // calculate ratational movement of robot
  rot_z += (clk.restart().asSeconds() - prev_time) * angular_velocity;

  //chech if is collision if not move robot if not signal with which wall is collision
  if (abs(x - (room_width / 2)) > radius && abs(x + (room_width / 2)) > radius && collision_front == 0 && collision_rear == 0) {
    prev_x = x;
    x += (clk.restart().asSeconds() - prev_time) * linear_velocity * cos(rot_z*PI/180);
  } else {
    if (x > 0) {
      collision_front = 1;
    } else {
      collision_rear = 1;
    }
  }
  if (abs(y - (room_length / 2)) > radius && abs(y + (room_length / 2)) > radius && collision_right == 0 && collision_left == 0) {
    prev_y = y;
    y += (clk.restart().asSeconds() - prev_time) * linear_velocity * sin(rot_z*PI/180);
  } else {
    if (y > 0) {
      collision_right = 1;
    } else {
      collision_left = 1;
    }
  }
}

bool Robot::finish_point_reach(Room room){
    // check if is finish point reach by robot
    if (sqrt(pow(x - room.fp_x_position, 2) + pow(y - room.fp_y_position, 2)) <= radius + room.fp_radius) {
      return true;
    } else {
      return false;
    }
}
