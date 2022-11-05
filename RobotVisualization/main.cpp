#include "headers.h"
#include "trafficcone.h"
#include "drawingfunctions.h"

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;

QT_END_NAMESPACE

class Menu : public QDialog
{
public:
    explicit Menu(QWidget *parent = nullptr);

private slots:
    void StartSimulation();

private:
    QLabel *m_serialPortLabel = nullptr;
    QComboBox *m_serialPortComboBox = nullptr;
    QLabel *m_TrafficConesLabel = nullptr;
    QSpinBox *m_TrafficConesSpinBox = nullptr;
    QPushButton *m_startButton = nullptr;
};



enum cameraType {
    external = 0,
    internal = 1
};

struct {
  float eye_x = 600;
  float eye_y = 0;
  float eye_z = 200;
  cameraType type = external;
}
camera;

struct {
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
  float u1_left = 0; //u(n-1)
  float u2_left = 0; //u(n-2)
  float w1_left = 0; //ω(n-1)
  float w2_left = (a1*u1_left - b1*w1_left)/b2; //ω(n-2)

  //RIGHT WHEEL
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
}
robot;

struct{
    float k = 5.7;
    float T = 0.27;
    float tau = 0.3;
    float Kp = 1.2*T/(tau*k);
    float Ti = 2*tau;
    float Td = 0;
    float dt = 0.1;

    //LEFT WHEEL
    float pre_err_left = 0;
    float pre_int_left = 0;
    float u_left = 0;

    //RIGHT WHEEL
    float pre_err_right = 0;
    float pre_int_right = 0;
    float u_right = 0;
}
PID;

float PID_control(float y_ref,float y, std::string wheel){

    float error = y_ref - y;
    float integral = 0;
    float derivative = 0;

    if(wheel == "left")
    {
        integral = PID.pre_int_left + error * PID.dt;
        PID.pre_int_left = integral;
        derivative = (error - PID.pre_err_left)/PID.dt;
        PID.pre_err_left = error;
    }
    else if(wheel == "right")
    {
        integral = PID.pre_int_right + error * PID.dt;
        PID.pre_int_right = integral;
        derivative = (error - PID.pre_err_right)/PID.dt;
        PID.pre_err_right = error;
    }

    float P = PID.Kp * error;
    float I = (PID.Kp/PID.Ti)*integral;
    float D = PID.Kp * PID.Td * derivative;

    float U = P + I + D;

    if(U>=24){
        return 24.0;
    } else if(U<=-24.0){
        return -24.0;
    } else {
        return U;
    }
}

void object_respond(float u_sterL, float u_sterP, float &yl, float &yp){
     yl = (robot.a1*robot.u1_left + robot.a0*robot.u2_left - robot.b1*robot.w1_left - robot.b0 * robot.w2_left)/robot.b2;
     yp = (robot.a1*robot.u1_right + robot.a0*robot.u2_right - robot.b1*robot.w1_right - robot.b0 * robot.w2_right)/robot.b2;

     robot.w2_left = robot.w1_left;
     robot.w1_left = yl;
     robot.u2_left = robot.u1_left;
     robot.u1_left = u_sterL;

     robot.w2_right = robot.w1_right;
     robot.w1_right = yp;
     robot.u2_right = robot.u1_right;
     robot.u1_right = u_sterP;
}

bool traffic_cone_robot_collisions(TrafficCone trafficcone) {
  if (sqrt(pow(robot.x - trafficcone.pos_x, 2) + pow(robot.y - trafficcone.pos_y, 2)) <= robot.radius + trafficcone.r_down) {
    return true;
  } else {
    return false;
  } 
}

void draw_robot() {
  const double from_ground = robot.from_ground;
  const double wheel_width = robot.wheel_width;
  const double wheel_radius = robot.wheel_radius;

  //BOX
  glColor3f(50.f / 255, 50.f / 255, 255.f/ 255);
  draw_cube(robot.radius, 2 / 1.5 * robot.radius, 2 * wheel_radius - from_ground - 1, wheel_radius + from_ground);
  //LEFT WHEEL
  draw_circle(0, -robot.radius / 1.25, wheel_radius, wheel_width, 0.0, 'b'); //int x, int y, double radius, double width, double rot, char color
  //WHEEL AXLE
  draw_circle(0, -robot.radius / 1.25, 2.0, 2 * robot.radius / 1.25, 0.0, 'b', wheel_radius);
  //RIGHT WHEEL
  draw_circle(0, robot.radius / 1.25, wheel_radius, wheel_width, 0.0, 'b');
  //MIDDLE WHEEL
  draw_circle(-robot.radius / 1.5, 0, 3.0, wheel_width, 0.0, 'b');
  //TRANSPARENT
  draw_circle(0, -robot.radius / 1.25 - 7.5, wheel_radius, 10, 0.0, 't', 0.0); //int x, int y, double radius, double width, double rot, char color
  draw_circle(0, robot.radius / 1.25 - 2.5, wheel_radius, robot.radius, 0.0, 't');
  draw_circle(-robot.radius / 1.5, -wheel_width, 4.5, 3 * wheel_width, 0.0, 't');
  //BOTTOM
  draw_circle(0, from_ground, robot.radius, 2.0, 90.0, 'g', -robot.radius); //(int x, int y, double radius, double width)
  //TOP
  draw_circle(0, 2 * wheel_radius + 1, robot.radius, 2.0, 0.0, 'g', -robot.radius);

}


void set_viewport(int width, int height, cameraType cam) {
  const float ar = (float) width / (float) height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-ar, ar, -1.0, 1.0, 2.0, 1000.0);

  if (cam == internal) {
    gluLookAt(robot.x, robot.y, 50, robot.x - width * cos(robot.rot_z * PI / 180), robot.y - width * sin(robot.rot_z * PI / 180), 10, 0, 0, 10);
  } else if (cam == external) {
    gluLookAt(robot.x + 150, camera.eye_y, camera.eye_z, robot.x, 0, 0, 0, 0, 10);
  }
}

void cameraHandling(sf::Clock & clk, float prev_time, cameraType & type, bool changeCamera) {
  const float camera_velocity = 150.0;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
    camera.eye_x += (clk.restart().asSeconds() - prev_time) * camera_velocity;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
    camera.eye_x -= (clk.restart().asSeconds() - prev_time) * camera_velocity;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
    camera.eye_y += (clk.restart().asSeconds() - prev_time) * camera_velocity;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
    camera.eye_y -= (clk.restart().asSeconds() - prev_time) * camera_velocity;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
    camera.eye_z += (clk.restart().asSeconds() - prev_time) * camera_velocity;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
    camera.eye_z -= (clk.restart().asSeconds() - prev_time) * camera_velocity;
  }

  if (changeCamera) {
    if (type == internal) {
      type = external;
      camera.eye_x = 26;
      camera.eye_y = 5;
      camera.eye_z = 120;
    } else if (type == external) {
      type = internal;
      camera.eye_x = 0;
      camera.eye_y = 0;
      camera.eye_z = 0;
    }
  }
}

bool connectBluetooth(QSerialPort * serial,QString serialport) {
  // set bluetooth connection by serial port
  bool ok;
  serial -> setPortName(serialport);
  serial -> setBaudRate(QSerialPort::Baud9600);
  serial -> setDataBits(QSerialPort::Data8);
  serial -> setParity(QSerialPort::NoParity);
  serial -> setFlowControl(QSerialPort::NoFlowControl);
  if (serial -> open(QIODevice::ReadWrite)) {
    ok = true;
  } else {
    //error
    ok = false;
    qDebug() << serial -> errorString();
  }

  return ok;
}

void velocity_extraction(std::string text) {
  // extract from bluetooth message velocities of both wheels
  if (text.find("wp") != std::string::npos && text.find("wl") != std::string::npos) {
    std::regex re1(R"((wp)(.*)(wl))");
    std::regex re2(R"((wl)(.*))");
    std::smatch sm1;
    std::smatch sm2;

    std::regex_search(text, sm1, re1);
    std::regex_search(text, sm2, re2);

    robot.prev_left_wheel_velocity = robot.left_wheel_velocity;
    robot.prev_right_wheel_velocity = robot.right_wheel_velocity;
    robot.left_wheel_velocity_ref = std::stof(sm1[2]);
    robot.right_wheel_velocity_ref = std::stof(sm2[2]);

  }
}

void collisions() {
    //convert rotation for 0-360 degrees only
    if(robot.rot_z>=0){
        robot.rot_z_0_360 = (int)robot.rot_z%360;
        } else {
            robot.rot_z_0_360 = abs((int)(360+robot.rot_z)%360);
        }


  // check if robot ride away from wall if true there is no more collision if false there is still collision
  if (robot.collision_front == 1) {
    if (robot.linear_velocity > 0 && (robot.rot_z_0_360 < 90 || robot.rot_z_0_360 > 270)) {
      robot.collision_front = 0;
    } else if (robot.linear_velocity < 0 && robot.rot_z_0_360 > 90 && robot.rot_z_0_360 < 270) {
      robot.collision_front = 0;
    } else {
      robot.x = robot.prev_x;
    }
  }
  if (robot.collision_rear == 1) {
    if (robot.linear_velocity > 0 && robot.rot_z_0_360 > 90 && robot.rot_z_0_360 < 270) {
      robot.collision_rear = 0;
    } else if (robot.linear_velocity < 0 && (robot.rot_z_0_360 < 90 || robot.rot_z_0_360 > 270)) {
      robot.collision_rear = 0;
    } else {
      robot.x = robot.prev_x;
    }
  }
  if (robot.collision_right == 1) {
    if (robot.linear_velocity > 0 && robot.rot_z_0_360 < 180) {
      robot.collision_right = 0;
    } else if (robot.linear_velocity < 0 && robot.rot_z_0_360 > 180) {
      robot.collision_right = 0;
    } else {
      robot.y = robot.prev_y;
    }
  }
  if (robot.collision_left == 1) {
    if (robot.linear_velocity > 0 && robot.rot_z_0_360 > 180) {
      robot.collision_left = 0;
    } else if (robot.linear_velocity < 0 && robot.rot_z_0_360 < 180) {
      robot.collision_left = 0;
    } else {
      robot.y = robot.prev_y;
    }
  }
}

void robot_movement(sf::Clock clk, float prev_time, double room_width, double room_length) {

  // conversion from right and left wheels velocities to angular and linear velocities
  robot.linear_velocity = (robot.right_wheel_velocity + robot.left_wheel_velocity) * robot.wheel_radius / 2;
  robot.angular_velocity = (robot.right_wheel_velocity - robot.left_wheel_velocity) * robot.wheel_radius / robot.track_between_wheels;

  // calculate ratational movement of robot
  robot.rot_z += (clk.restart().asSeconds() - prev_time) * robot.angular_velocity;

  //chech if is collision if not move robot if not signal with which wall is collision
  if (abs(robot.x - (room_width / 2)) > robot.radius && abs(robot.x + (room_width / 2)) > robot.radius && robot.collision_front == 0 && robot.collision_rear == 0) {
    robot.prev_x = robot.x;
    robot.x += (clk.restart().asSeconds() - prev_time) * robot.linear_velocity * cos(robot.rot_z * PI / 180);
  } else {
    if (robot.x > 0) {
      robot.collision_front = 1;
    } else {
      robot.collision_rear = 1;
    }
  }
  if (abs(robot.y - (room_length / 2)) > robot.radius && abs(robot.y + (room_length / 2)) > robot.radius && robot.collision_right == 0 && robot.collision_left == 0) {
    robot.prev_y = robot.y;
    robot.y += (clk.restart().asSeconds() - prev_time) * robot.linear_velocity * sin(robot.rot_z * PI / 180);
  } else {
    if (robot.y > 0) {
      robot.collision_right = 1;
    } else {
      robot.collision_left = 1;
    }
  }
}

//void Screendump(char *tga_file, short W, short H) {
// FILE   *out = fopen(tga_file, "w");
// char   pixel_data[3*W*H];
// short  TGAhead[] = {0, 2, 0, 0, 0, 0, W, H, 24};

// glReadBuffer(GL_FRONT);
// glReadPixels(0, 0, W, H, GL_BGR, GL_UNSIGNED_BYTE, pixel_data);
// fwrite(&TGAhead, sizeof(TGAhead), 1, out);
// fwrite(pixel_data, 3*W*H, 1, out);
// fclose(out); }

void play(int number_of_traffic_cones,QString serialport) {
  // create the window
  sf::Window window(sf::VideoMode(1500, 768), "Robot Visualization", sf::Style::Default, sf::ContextSettings(32));
  window.setVerticalSyncEnabled(true); //limit the number of frames

  // activate the window
  window.setActive(true);

  window.setFramerateLimit(60);

  float prev_time = 0;

  sf::Clock plot_clock;
  sf::Clock update_clock;
  int from_prev_plot = 0;
  int from_prev_update = 0;

  float collision_delay = 0;

  std::ofstream myfile;

  //1200cm x 800cm x 250cm
  const double room_width = 1200.0;
  const double room_length = 800.0;
  const double room_height = 250.0;

  const double doors_height = 200.0;
  const double doors_width = 100.0;
  const double doors_position = 1200.0;

  glClearColor(0, 0, 0, 1);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_SMOOTH);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);

  // setup lights

  GLfloat light_position[] = {
    camera.eye_x,
    camera.eye_y,
    camera.eye_z,
    1.0
  };
  GLfloat light_ambient[] = {
    0.4,
    0.4,
    0.4,
    1.0
  }; //otoczenie
  GLfloat light_diffuse[] = {
    0.4,
    0.4,
    0.4,
    1.0
  }; //rozproszone
  GLfloat light_specular[] = {
    0.0,
    0.0,
    0.0,
    1.0
  }; //lustrzane

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  GLfloat global_ambient[] = {
    0.5,
    0.5,
    0.5,
    0.1
  };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

  // load resources, initialize the OpenGL states, ...

  // run the main loop
  bool running = true;

  sf::Clock clk;

  std::string input;
  QByteArray output;
  QByteArray readData;
  std::vector < std::string > data {};

  size_t posOfsep = 0;
  std::string separation_sign = ";";
  std::string dataToCut;
  std::string control;

  QSerialPort * serial = new QSerialPort();
  running = connectBluetooth(serial,serialport);

  std::vector < TrafficCone > trafficCones;

  for (int i = 0; i < number_of_traffic_cones; i++) {
    trafficCones.push_back(TrafficCone(300 - 150*i, 0));
  }

//  uint8_t *pixels =new uint8_t[3*window.getSize().x*window.getSize().y];

  while (running) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        running = false;
        printf("\n");
      }      
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::C) {
          cameraHandling(clk, prev_time, camera.type, true);
        }
        // keyboard robot control
        if (event.key.code == sf::Keyboard::Up) {
          robot.right_wheel_velocity_ref = 25.0;
          robot.left_wheel_velocity_ref = 25.0;
        }
        if (event.key.code == sf::Keyboard::Down) {
          robot.right_wheel_velocity_ref = -25.0;
          robot.left_wheel_velocity_ref = -25.0;
        }
        if (event.key.code == sf::Keyboard::Right) {
          robot.right_wheel_velocity_ref = 25.0;
          robot.left_wheel_velocity_ref = -25.0;
        }
        if (event.key.code == sf::Keyboard::Left) {
          robot.right_wheel_velocity_ref = -25.0;
          robot.left_wheel_velocity_ref = 25.0;
        }
      }

      if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left) {
          robot.right_wheel_velocity_ref = 0.0;
          robot.left_wheel_velocity_ref = 0.0;
        }
      }

    }
    //ROBOT VELOCITY REGULATION
    PID.u_left = PID_control(robot.left_wheel_velocity_ref,robot.left_wheel_velocity,"left");
    PID.u_right = PID_control(robot.right_wheel_velocity_ref,robot.right_wheel_velocity,"right");


    //ROBOT REAL VELOCITITES SIMULATION
    object_respond(PID.u_left, PID.u_right, robot.left_wheel_velocity, robot.right_wheel_velocity);

    // clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // draw stuff
    glPushMatrix();

    //std::cout<<"left wheel velocity: "<<robot.left_wheel_velocity<<" right wheel velocity: "<<robot.right_wheel_velocity<<std::endl;

    //ROBOT MOVEMENT
    robot_movement(clk, prev_time, room_width, room_length);

    //collisions case
    collisions();

    for (size_t i = 0; i < trafficCones.size(); i++) {
      if (traffic_cone_robot_collisions(trafficCones[i])) {
        trafficCones[i].pos_x += (clk.restart().asSeconds() - prev_time) * robot.linear_velocity * cos(robot.rot_z * PI / 180);
        trafficCones[i].pos_y += (clk.restart().asSeconds() - prev_time) * robot.linear_velocity * sin(robot.rot_z * PI / 180);
        std::cout<<"Collision with traffic cone!"<<std::endl;
        robot.traffic_cones_collision = 1;
      }
    }

    //send message if is collision or not

    if (robot.collision_front || robot.collision_rear || robot.collision_right || robot.collision_left) {
      robot.collision = 1;
    } else {
      robot.collision = 0;
    }

    if (robot.collision == 1 && robot.last_collision == 0) {
      output = "collision";
      serial -> write(output);
      std::cout << output.toStdString() << std::endl;
      robot.last_collision = 1;
    } else if (robot.collision == 0 && robot.last_collision == 1) {
      output = "ok";
      serial -> write(output);
      std::cout << output.toStdString() << std::endl;
      robot.last_collision = 0;
    }

    //RESTART POSISION AFTER COLLISION WITH TRAFFIC CONE

    if(robot.traffic_cones_collision){
        collision_delay += prev_time;
        if(collision_delay>=2){
            robot.traffic_cones_collision = 0;
            collision_delay = 0;
            robot.x = robot.start_x;
            robot.y = robot.start_y;
            robot.rot_z = 0;
            for (size_t i = 0; i < trafficCones.size(); i++) {
                trafficCones[i].pos_x = 300 - 150*static_cast<int>(i);
                trafficCones[i].pos_y = 0;
            }
        }
    }

    //camera movement
    cameraHandling(clk, prev_time, camera.type, false);

    set_viewport(window.getSize().x, window.getSize().y, camera.type);


    from_prev_plot += plot_clock.restart().asMilliseconds();
    from_prev_update += update_clock.restart().asMilliseconds();
    if (from_prev_update >= 100) {

        for(size_t i=0; i<100-1; i++)
        {
            robot.x_tab[i] = robot.x_tab[i+1];
            robot.y_tab[i] = robot.y_tab[i+1];
            robot.v_tab[i] = robot.v_tab[i+1];
            robot.w_tab[i] = robot.w_tab[i+1];
            robot.wl_tab[i] = robot.wl_tab[i+1];
            robot.wp_tab[i] = robot.wp_tab[i+1];
        }
        robot.x_tab[99] = robot.x;
        robot.y_tab[99] = robot.y;
        robot.v_tab[99] = robot.linear_velocity;
        robot.w_tab[99] = robot.angular_velocity;
        robot.wl_tab[99] = robot.left_wheel_velocity;
        robot.wp_tab[99] = robot.right_wheel_velocity;

        from_prev_update = 0;
    }
    //new data to plot every 1000ms
    if(from_prev_plot >= 1000)
    {
        myfile.open("robot_data.txt", std::ios::out);
        myfile << "x;y;v,ω;ωL;ωP\n";
        for(size_t i=0; i<100; i++)
        {
          myfile << robot.x_tab[i] << ";" << robot.y_tab[i] << ";" << robot.v_tab[i] << ";" << robot.w_tab[i] << ";" << robot.wl_tab[i] << ";" << robot.wp_tab[i] << "\n";
        }
        myfile.close();

        from_prev_plot = 0;

    }

    draw_floor(room_width, room_length);
    draw_walls(room_width, room_length, room_height);
    draw_doors(doors_height, doors_width, doors_position);

    for (size_t i = 0; i < trafficCones.size(); i++) {
      trafficCones[i].draw_traffic_cone();
    }

    glTranslated(robot.x, robot.y, 0.0);
    glRotated(0, 1.0, 0.0, 0.0);
    glRotated(0, 0.0, 1.0, 0.0);
    glRotated(robot.rot_z, 0, 0.0, 1.0);

    //ROBOT
    draw_robot();

    glPopMatrix();

    window.display();

//    glReadPixels(0,0,window.getSize().x,window.getSize().y,GL_RGB,GL_UNSIGNED_BYTE,pixels);

//    std::cout<<int(pixels[0])<<" "<<int(pixels[1])<<std::endl;



    //READ DATA FROM CONNECTED BLUETOOTH DEVICE
    readData = serial -> readAll();
    serial -> waitForReadyRead(10);

    if (readData.toStdString().length() > 0) {
      input = readData.toStdString();

      //RECEIVED COMMAND "CAMERA" - CHANGE CAMERA
      if (input.find("camera") != std::string::npos) {
        cameraHandling(clk, prev_time, camera.type, true);
      }

      dataToCut = input;

      //SEPERATE DATA FROM CONNECTED BLUETOOTH DEVICE
      while ((posOfsep = dataToCut.find(separation_sign)) != std::string::npos) {
        data.push_back(dataToCut.substr(0, posOfsep));
        dataToCut.erase(0, posOfsep + separation_sign.length());
      }


      //SEND RESPOND THAT DATA CAME SUCCESFULLY
      output = QString::fromStdString(data[1]).toUtf8();
      serial -> write(output);

      //TRANSFORM VELOCITY DATA FROM CONNECTED DEVICE TO VELOCITIES OF ROBOT
      control = data[0];
      velocity_extraction(control);

      data.clear();

    }

    prev_time = clk.restart().asSeconds();
  }

  //CLOSE SERIAL PORT AFTER CLOSING APPLICATION
  serial -> close();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    Menu menu;
    menu.show();
    return app.exec();
}

Menu::Menu(QWidget *parent):
    QDialog(parent),
    m_serialPortLabel(new QLabel(tr("Serial port:"))),
    m_serialPortComboBox(new QComboBox),
    m_TrafficConesLabel(new QLabel(tr("How many traffic cones:"))),
    m_TrafficConesSpinBox(new QSpinBox),
    m_startButton(new QPushButton(tr("Start")))
{
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        m_serialPortComboBox->addItem(info.portName());

    m_TrafficConesSpinBox->setRange(0,5);
    m_TrafficConesSpinBox->setValue(0);

    auto mainLayout = new QGridLayout;
    mainLayout->addWidget(m_serialPortLabel, 0, 0);
    mainLayout->addWidget(m_serialPortComboBox, 0, 1);
    mainLayout->addWidget(m_TrafficConesLabel, 1, 0);
    mainLayout->addWidget(m_TrafficConesSpinBox, 1, 1);
    mainLayout->addWidget(m_startButton, 0, 2, 2, 1);
    setLayout(mainLayout);

    setWindowTitle(tr("Robot Visualization"));
    m_serialPortComboBox->setFocus();

    connect(m_startButton, &QPushButton::clicked, this, &Menu::StartSimulation);
}

void Menu::StartSimulation(){
    robot.x = robot.start_x;
    robot.y = robot.start_y;
    robot.rot_z = 0;
    play(m_TrafficConesSpinBox->value(),m_serialPortComboBox->currentText());
}
