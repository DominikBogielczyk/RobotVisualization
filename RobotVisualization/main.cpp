#include "headers.h"

#define PI 3.14159265
#define port "COM3"

enum cameraType
{
    external = 0,
    internal = 1
};

struct {
    float eye_x = 26;
    float eye_y = 5;
    float eye_z = 120;
    cameraType type = external;
} camera;

struct {
    const double height = 12.0;
    const double radius = 20.0;
    const double from_ground = 3.0;
    const double wheel_width = 1.0;
    const double wheel_radius = height - from_ground;
    const double track_between_wheels = radius * 2;

    float x = 0;
    float y = 0;
    float prev_x = 0;
    float prev_y = 0;
    float rot_z_0_360 = 0;
    float rot_z = 0;

    float left_wheel_velocity = 0.0;
    float right_wheel_velocity = 0.0;


    float angular_velocity = 0.0;
    float linear_velocity = 0.0;
    bool collision_front = 0;
    bool collision_rear = 0;
    bool collision_right = 0;
    bool collision_left = 0;
} robot;

void draw_circle(int x, int y, double radius, double width, double rot, char color, double dz = 0.0) {

  switch (color) {
  case 'g':
    glColor3f(0/255.f, 0/255.f, 0/255.f); //BLACK
    break;
  case 'b':
    glColor3d(0/255.f, 0/255.f, 153/255.f); //BLUE
    break;
  case 't':
    glColor4d(0.5, 0.5, 0.4, 0.0); //TRANSPARENT
    break;
  }

  glPushMatrix();
  glRotated(rot, 1.0, 0.0, 0.0);
  glRotated(0, 0.0, 1.0, 0.0);
  glRotated(0, 0, 0.0, 1.0);

  int num = 40;

  //glPopMatrix();

  glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
  glVertex3f(x, y, radius); // center of circle
  for (int i = 0; i <= num; i++) {
    glVertex3f(x + radius * cos(i * 2 * PI / num), y, radius + radius * sin(i * 2 * PI / num) + dz);
  }
  glEnd(); //END

  glBegin(GL_QUAD_STRIP); //BEGIN CIRCLE
  for (int i = 0; i <= num; i++) {
    glVertex3f(x + radius * cos(i * 2 * PI / num), y, radius + radius * sin(i * 2 * PI / num) + dz);
    glVertex3f(x + radius * cos(i * 2 * PI / num), y + width, radius + radius * sin(i * 2 * PI / num) + dz);
    glVertex3f(x + radius * cos((i + 1) * 2 * PI / num), y, radius + radius * sin((i + 1) * 2 * PI / num) + dz);
    glVertex3f(x + radius * cos((i + 1) * 2 * PI / num), y + width, radius + radius * sin((i + 1) * 2 * PI / num) + dz);
  }
  glEnd(); //END

  glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
  glVertex3f(x, y, radius); // center of circle
  for (int i = 0; i <= num; i++) {
    glVertex3f(x + radius * cos(i * 2 * PI / num), y + width, radius + radius * sin(i * 2 * PI / num) + dz);
  }
  glEnd(); //END
}

void draw_cube(double robot_size_x, double robot_size_y, double robot_size_z, double middle_z) {
  double half_size_x = robot_size_x / 2.0;
  double half_size_y = robot_size_y / 2.0;
  double half_size_z = robot_size_z / 2.0;

  //glColor3d(0.5, 0.5, 0.4); //GREY
  // bottom
  glBegin(GL_POLYGON);
  glVertex3d(-half_size_x, half_size_y, middle_z - half_size_z);
  glVertex3d(half_size_x, half_size_y, middle_z - half_size_z);
  glVertex3d(half_size_x, -half_size_y, middle_z - half_size_z);
  glVertex3d(-half_size_x, -half_size_y, middle_z - half_size_z);
  glEnd();

  // top
  glBegin(GL_POLYGON);
  glVertex3d(-half_size_x, half_size_y, middle_z + half_size_z);
  glVertex3d(half_size_x, half_size_y, middle_z + half_size_z);
  glVertex3d(half_size_x, -half_size_y, middle_z + half_size_z);
  glVertex3d(-half_size_x, -half_size_y, middle_z + half_size_z);
  glEnd();

  // left
  glBegin(GL_POLYGON);
  glVertex3d(-half_size_x, -half_size_y, middle_z + half_size_z);
  glVertex3d(-half_size_x, half_size_y, middle_z + half_size_z);
  glVertex3d(-half_size_x, half_size_y, middle_z - half_size_z);
  glVertex3d(-half_size_x, -half_size_y, middle_z - half_size_z);
  glEnd();

  // right
  glBegin(GL_POLYGON);
  glVertex3d(half_size_x, -half_size_y, middle_z + half_size_z);
  glVertex3d(half_size_x, half_size_y, middle_z + half_size_z);
  glVertex3d(half_size_x, half_size_y, middle_z - half_size_z);
  glVertex3d(half_size_x, -half_size_y, middle_z - half_size_z);
  glEnd();

  // front
  glBegin(GL_POLYGON);
  glVertex3d(-half_size_x, -half_size_y, middle_z + half_size_z);
  glVertex3d(half_size_x, -half_size_y, middle_z + half_size_z);
  glVertex3d(half_size_x, -half_size_y, middle_z - half_size_z);
  glVertex3d(-half_size_x, -half_size_y, middle_z - half_size_z);
  glEnd();

  // back
  glBegin(GL_POLYGON);
  glVertex3d(-half_size_x, half_size_y, middle_z + half_size_z);
  glVertex3d(half_size_x, half_size_y, middle_z + half_size_z);
  glVertex3d(half_size_x, half_size_y, middle_z - half_size_z);
  glVertex3d(-half_size_x, half_size_y, middle_z - half_size_z);
  glEnd();

}

void draw_robot() {
  const double from_ground = robot.from_ground;
  const double wheel_width = robot.wheel_width;
  const double wheel_radius = robot.wheel_radius;

  //BOX
  glColor3f(50.f/255, 50.f/255, 255.f/255);
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

void draw_floor(double width, double length) {
  glBegin(GL_POLYGON);
  glColor3f(255/255, 255/255, 255/255);
  glVertex3d(-width / 2, length / 2, 0);
  glVertex3d(width / 2, length / 2, 0);
  glVertex3d(width / 2, -length / 2, 0);
  glVertex3d(-width / 2, -length / 2, 0);
  glEnd();

  glColor3d(169 / 255.0, 169 / 255.0, 169 / 255.0);

  glLineWidth(4.0);
  glColor3f(50/255, 50/255, 255/255);

  int n = 6;
  for(int i=1; i<n; i++)
  {
     glBegin(GL_LINES);
     glVertex3f(-width/2, -length/2 + i*length/n, 0);
     glVertex3f(width/2, -length/2 + i*length/n, 0);
     glEnd();
  }
  for(int i=1; i<n; i++)
  {
     glBegin(GL_LINES);
     glVertex3f(-width/2 + i*width/n, -length/2, 0);
     glVertex3f(-width/2 + i*width/n, length/2, 0);
     glEnd();
  }


}

void draw_walls(double width, double length, double height) {
  //RIGHT
  glBegin(GL_POLYGON);
  glColor3f(50.f/255, 50.f/255, 255.f/255);
  glVertex3d(-width / 2, length / 2, 0);
  glVertex3d(width / 2, length / 2, 0);
  glVertex3d(width / 2, length / 2, height);
  glVertex3d(-width / 2, length / 2, height);
  glEnd();

  //LEFT
  glBegin(GL_POLYGON);
  glColor3f(50.f/255, 50.f/255, 255.f/255);
  glVertex3d(-width / 2, -length / 2, 0);
  glVertex3d(width / 2, -length / 2, 0);
  glVertex3d(width / 2, -length / 2, height);
  glVertex3d(-width / 2, -length / 2, height);
  glEnd();

  //REAR
  glBegin(GL_POLYGON);
  glColor3f(51.f/255, 153.f/255, 255.f/255);
  glVertex3d(-width / 2, -length / 2, 0);
  glVertex3d(-width / 2, length / 2, 0);
  glVertex3d(-width / 2, length / 2, height);
  glVertex3d(-width / 2, -length / 2, height);
  glEnd();

  //FRONT
  glBegin(GL_POLYGON);
  glColor3f(51.f/255, 153.f/255, 255.f/255);
  glVertex3d(width / 2, -length / 2, 0);
  glVertex3d(width / 2, length / 2, 0);
  glVertex3d(width / 2, length / 2, height);
  glVertex3d(width / 2, -length / 2, height);
  glEnd();
}

void draw_doors(double height, double width, double position){
    glBegin(GL_POLYGON);
    glColor3d(1.0, 1.0, 255.0 / 255);
    glVertex3d(-position / 2+1, -width / 2, 0);
    glVertex3d(-position / 2+1, width / 2, 0);
    glVertex3d(-position / 2+1, width / 2, height);
    glVertex3d(-position / 2+1, -width / 2, height);
    glEnd();
}

void set_viewport(int width, int height, cameraType cam) {
  const float ar = (float) width / (float) height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-ar, ar, -1.0, 1.0, 2.0, 1000.0);

  if(cam == internal)
  {
      gluLookAt(robot.x, robot.y, 50, robot.x - width * cos(robot.rot_z * PI / 180), robot.y - width * sin(robot.rot_z * PI / 180), 10, 0, 0, 10);
  }
  else if(cam == external)
  {
      gluLookAt(camera.eye_x, camera.eye_y, camera.eye_z, robot.x, robot.y, 0, 0, 0, 10);
  }
}

void cameraHandling(sf::Clock & clk, float prev_time, cameraType &type, bool changeCamera) {
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

  if(changeCamera)
  {
      if(type == internal)
      {
          type = external;
          camera.eye_x = 26;
          camera.eye_y = 5;
          camera.eye_z = 120;
      }
      else if(type == external)
      {
          type = internal;
          camera.eye_x = 0;
          camera.eye_y = 0;
          camera.eye_z = 0;
      }
  }
}


bool connectBluetooth(QSerialPort *serial)
{
    // set bluetooth connection by serial port
    bool ok;
    serial -> setPortName(port);
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

void velocity_extraction(std::string text){
    // extract from bluetooth message velocities of both wheels
    if(text.find("wp") != std::string::npos && text.find("wl") != std::string::npos){
        std::regex re1(R"((wp)(.*)(wl))");
        std::regex re2(R"((wl)(.*))");
        std::smatch sm1;
        std::smatch sm2;

        std::regex_search(text,sm1,re1);
        std::regex_search(text,sm2,re2);

        robot.right_wheel_velocity = std::stof(sm1[2]);
        robot.left_wheel_velocity = std::stof(sm2[2]);

    }
}

void collisions(){
    // check if robot ride away from wall if true there is no more collision if false there is still collision
    if (robot.collision_front == 1){
        if(robot.linear_velocity > 0 && (robot.rot_z_0_360 < 90 || robot.rot_z_0_360 > 270 )){
            robot.collision_front = 0;
        } else if(robot.linear_velocity < 0 && robot.rot_z_0_360 > 90 && robot.rot_z_0_360 < 270) {
            robot.collision_front = 0;
        } else {
            robot.x = robot.prev_x;
        }
    }
    if(robot.collision_rear == 1){
        if(robot.linear_velocity > 0 && robot.rot_z_0_360 > 90 && robot.rot_z_0_360 < 270){
            robot.collision_rear = 0;
        } else if(robot.linear_velocity < 0  && (robot.rot_z_0_360 < 90 || robot.rot_z_0_360 > 270 )) {
            robot.collision_rear = 0;
        } else {
            robot.x = robot.prev_x;
        }
    }
    if(robot.collision_right == 1){
        if(robot.linear_velocity > 0 && robot.rot_z_0_360 < 180){
            robot.collision_right = 0;
        } else if(robot.linear_velocity < 0 && robot.rot_z_0_360 > 180) {
            robot.collision_right = 0;
        } else {
            robot.y = robot.prev_y;
        }
    }
    if(robot.collision_left == 1){
        if(robot.linear_velocity > 0 && robot.rot_z_0_360 > 180){
            robot.collision_left = 0;
        } else if(robot.linear_velocity < 0 && robot.rot_z_0_360 < 180) {
            robot.collision_left = 0;
        } else {
            robot.y = robot.prev_y;
        }
    }
}

void robot_movement(sf::Clock clk,float prev_time,double room_width,double room_length){

        // conversion from right and left wheels velocities to angular and linear velocities
        robot.linear_velocity = (robot.right_wheel_velocity+robot.left_wheel_velocity)*5*robot.wheel_radius/2;
        robot.angular_velocity = (robot.right_wheel_velocity-robot.left_wheel_velocity)*10*robot.wheel_radius/robot.track_between_wheels;

        // calculate ratational movement of robot
        robot.rot_z += (clk.restart().asSeconds() - prev_time) * robot.angular_velocity;

        //chech if is collision if not move robot if not signal with which wall is collision
        if (abs(robot.x - (room_width / 2)) > robot.radius && abs(robot.x + (room_width / 2)) > robot.radius && robot.collision_front == 0 && robot.collision_rear == 0) {
          robot.prev_x = robot.x;
          robot.x += (clk.restart().asSeconds() - prev_time) * robot.linear_velocity * cos(robot.rot_z * PI / 180);
        } else {
            if(robot.x > 0){
                robot.collision_front = 1;
            } else {
                robot.collision_rear = 1;
            }
        }
        if (abs(robot.y - (room_length / 2)) > robot.radius && abs(robot.y + (room_length / 2)) > robot.radius && robot.collision_right == 0 && robot.collision_left == 0) {
          robot.prev_y = robot.y;
          robot.y += (clk.restart().asSeconds() - prev_time) * robot.linear_velocity * sin(robot.rot_z * PI / 180);
        } else {
            if(robot.y > 0){
                robot.collision_right = 1;
            } else {
                robot.collision_left = 1;
            }
        }
}


void play() {
  // create the window
  sf::Window window(sf::VideoMode(1500, 768), "Robot Visualization", sf::Style::Default, sf::ContextSettings(32));
  window.setVerticalSyncEnabled(true); //limit the number of frames

  // activate the window
  window.setActive(true);

  float prev_time = 0;

  //1200cm x 800cm x 250cm
  const double room_width = 1200.0;
  const double room_length = 800.0;
  const double room_height = 250.0;

  const double doors_height = 200.0;
  const double doors_width  = 100.0;
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
  running = connectBluetooth(serial);

  while (running) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        running = false;
      }
      if (event.type == sf::Event::KeyPressed)
      {
          if (event.key.code == sf::Keyboard::C)
          {
              cameraHandling(clk, prev_time, camera.type, true);
          }
          // keyboard robot control
          if (event.key.code == sf::Keyboard::Up)
          {
              robot.right_wheel_velocity = 5.0;
              robot.left_wheel_velocity = 5.0;
          }
          if (event.key.code == sf::Keyboard::Down)
          {
              robot.right_wheel_velocity = -5.0;
              robot.left_wheel_velocity = -5.0;
          }
          if (event.key.code == sf::Keyboard::Right)
          {
              robot.right_wheel_velocity = 5.0;
              robot.left_wheel_velocity = -5.0;
          }
          if (event.key.code == sf::Keyboard::Left)
          {
              robot.right_wheel_velocity = -5.0;
              robot.left_wheel_velocity = 5.0;
          }
      }

      if(event.type == sf::Event::KeyReleased)
      {
            if(event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left)
            {
                robot.right_wheel_velocity = 0.0;
                robot.left_wheel_velocity = 0.0;
            }
      }


    }

    // clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // draw stuff
    glPushMatrix();

    //keyboard_robot_control();

    robot_movement(clk,prev_time,room_width,room_length);

    //convert rotation for 0-360 degrees only
    if(robot.rot_z>=0){
        robot.rot_z_0_360 = (int)robot.rot_z%360;
    } else {
        robot.rot_z_0_360 = abs((int)(360+robot.rot_z)%360);
    }

    //collisions case
    collisions();


    if(robot.collision_front || robot.collision_rear || robot.collision_right || robot.collision_left){
        output = "collision";
        serial->write(output);
    }

    //camera movement
    cameraHandling(clk, prev_time, camera.type, false);

    set_viewport(window.getSize().x, window.getSize().y, camera.type);

    draw_floor(room_width, room_length);
    draw_walls(room_width, room_length, room_height);
    draw_doors(doors_height,doors_width,doors_position);

    glTranslated(robot.x, robot.y, 0.0);
    glRotated(0, 1.0, 0.0, 0.0);
    glRotated(0, 0.0, 1.0, 0.0);
    glRotated(robot.rot_z, 0, 0.0, 1.0);

    //ROBOT
    draw_robot();

    glPopMatrix();

    window.display();

    readData = serial -> readAll();
    serial -> waitForReadyRead(10);

    auto receiveTime = std::chrono::duration_cast < std::chrono::milliseconds > (std::chrono::system_clock::now().time_since_epoch()).count();

    if (readData.toStdString().length() > 0) {
        posOfsep = 0;
        input = readData.toStdString();
      //RECEIVED COMMAND "CAMERA" - CHANGE CAMERA
      if(input.find("camera") != std::string::npos)
      {
          cameraHandling(clk, prev_time, camera.type, true);
      }

      dataToCut = input;

      while ((posOfsep = dataToCut.find(separation_sign)) != std::string::npos) {
        data.push_back(dataToCut.substr(0, posOfsep));
        dataToCut.erase(0, posOfsep + separation_sign.length());
      }

      output =QString::fromStdString(data[1]).toUtf8();
      serial -> write(output);
      std::cout<<data[0]<<std::endl;
      control = data[0];

      velocity_extraction(control);

      data.clear();

    }

    prev_time = clk.restart().asSeconds();
  }

  serial -> close();

}

int main() {
  play();
  return 0;
}
