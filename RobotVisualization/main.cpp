#include "headers.h"
#include "trafficcone.h"
#include "drawingfunctions.h"
#include "room.h"
#include "pid_controller.h"
#include "robot.h"

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
    QLabel *m_ModeLabel = nullptr;
    QComboBox *m_ModeComboBox = nullptr;
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

void set_viewport(int width, int height, cameraType cam,Robot robot) {
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


void play(int number_of_traffic_cones,QString serialport,QString mode) {
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

  Room room;

  PID_controller pid_controller;

  Robot robot;

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
  // run the main loop
  bool running = connectBluetooth(serial,serialport);

  std::vector < TrafficCone > trafficCones;

  for (int i = 0; i < number_of_traffic_cones; i++) {
    trafficCones.push_back(TrafficCone(300 - 150*i, 0));
  }

  //ADD SOUND EFFECT

  sf::SoundBuffer buffer;
  sf::Sound sound;
  bool sound_played = 0;
  if(!buffer.loadFromFile("collision_sound.wav")){
      std::cout<<"Sound can't be load!"<<std::endl;
  } else {
      sound.setBuffer(buffer);
  }

  //VARIABLES USED TO RIDE TIME COUNTING
//  bool count_time = false;
  bool is_finish_point = false;
  bool count_time = 0;
  float ride_time = 0;

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
    robot.u_left = pid_controller.PID_control(robot.left_wheel_velocity_ref,robot.left_wheel_velocity,"left");
    robot.u_right = pid_controller.PID_control(robot.right_wheel_velocity_ref,robot.right_wheel_velocity,"right");

    //ROBOT REAL VELOCITITES SIMULATION
    robot.object_respond(robot.u_left, robot.u_right, robot.left_wheel_velocity, robot.right_wheel_velocity);

    // clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // draw stuff
    glPushMatrix();

    //ROBOT MOVEMENT
    robot.robot_movement(clk, prev_time, room.room_width, room.room_length);

    //collisions case
    robot.collisions();

    for (size_t i = 0; i < trafficCones.size(); i++) {
      if (robot.traffic_cone_robot_collisions(trafficCones[i])) {
        trafficCones[i].pos_x += (clk.restart().asSeconds() - prev_time) * robot.linear_velocity * cos(robot.rot_z * PI / 180);
        trafficCones[i].pos_y += (clk.restart().asSeconds() - prev_time) * robot.linear_velocity * sin(robot.rot_z * PI / 180);
        robot.traffic_cones_collision = 1;
        ride_time = 0;
        count_time = false;
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
      robot.last_collision = 1;
    } else if (robot.collision == 0 && robot.last_collision == 1) {
      output = "ok";
      serial -> write(output);
      robot.last_collision = 0;
    }

    //RESTART POSISION AFTER COLLISION WITH TRAFFIC CONE

    if(robot.traffic_cones_collision){
        if(!sound_played){
        sound.play();
        sound_played = 1;
        }
        collision_delay += prev_time;
        if(collision_delay>=1){
            sound_played = 0;
            robot.traffic_cones_collision = 0;
            collision_delay = 0;
            robot.reset_robot_position();
            for (size_t i = 0; i < trafficCones.size(); i++) {
                trafficCones[i].pos_x = 300 - 150*static_cast<int>(i);
                trafficCones[i].pos_y = 0;
            }
        }
    }

    //camera movement
    cameraHandling(clk, prev_time, camera.type, false);

    set_viewport(window.getSize().x, window.getSize().y, camera.type,robot);

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
          //std::cout<< robot.x_tab[i] << ";" << robot.y_tab[i] << ";" << robot.v_tab[i] << ";" << robot.w_tab[i] << ";" << robot.wl_tab[i] << ";" << robot.wp_tab[i] <<std::endl;
        }
        myfile.close();
        from_prev_plot = 0;

    }

    //DRAW ELEMENTS OF ROOM
    room.draw_floor();
    room.draw_walls();
    room.draw_doors();
    if(mode=="Count time mode"){
    room.draw_finish_point();
    }

    for (size_t i = 0; i < trafficCones.size(); i++) {
      trafficCones[i].draw_traffic_cone();
    }

    glTranslated(robot.x, robot.y, 0.0);
    glRotated(0, 1.0, 0.0, 0.0);
    glRotated(0, 0.0, 1.0, 0.0);
    glRotated(robot.rot_z, 0, 0.0, 1.0);

    //ROBOT
    robot.draw_robot();

    glPopMatrix();

    window.display();

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
      robot.velocity_extraction(control);

      data.clear();
    }

    prev_time = clk.restart().asSeconds();

    if(mode=="Count time mode"){
    //COUNT RIDE TIME IF ROBOT START MOVING AND STOP COUNTING IF ROBOT REACH DESTINATION
    if(robot.left_wheel_velocity_ref != 0 || robot.right_wheel_velocity_ref != 0){
        count_time = true;
    }

    if(count_time){
        ride_time += prev_time;
        is_finish_point = robot.finish_point_reach(room,robot.x,robot.y,robot.radius);
    }
    if(is_finish_point){
        std::cout<<"Ride time: "<<ride_time<<std::endl;
        is_finish_point = false;
        count_time = false;
        ride_time = 0.0;
        robot.reset_robot_position();
    }
    }

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
    m_startButton(new QPushButton(tr("Start"))),
    m_ModeLabel(new QLabel(tr("Mode:"))),
    m_ModeComboBox(new QComboBox)
{
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        m_serialPortComboBox->addItem(info.portName());

    m_ModeComboBox->addItem("Free mode");
    m_ModeComboBox->addItem("Count time mode");

    m_TrafficConesSpinBox->setRange(0,5);
    m_TrafficConesSpinBox->setValue(0);

    auto mainLayout = new QGridLayout;
    mainLayout->addWidget(m_serialPortLabel, 0, 0);
    mainLayout->addWidget(m_serialPortComboBox, 0, 1);
    mainLayout->addWidget(m_TrafficConesLabel, 1, 0);
    mainLayout->addWidget(m_TrafficConesSpinBox, 1, 1);
    mainLayout->addWidget(m_startButton, 0, 2, 2, 1);
    mainLayout->addWidget(m_ModeLabel,2,0);
    mainLayout->addWidget(m_ModeComboBox,2,1);
    setLayout(mainLayout);

    setWindowTitle(tr("Robot Visualization"));
    m_serialPortComboBox->setFocus();

    connect(m_startButton, &QPushButton::clicked, this, &Menu::StartSimulation);
}

void Menu::StartSimulation(){
    play(m_TrafficConesSpinBox->value(),m_serialPortComboBox->currentText(),m_ModeComboBox->currentText());
}
