#include "headers.h"
#include "trafficcone.h"
#include "drawingfunctions.h"
#include "room.h"
#include "PID_controller.h"
#include "robot.h"
#include "pid_position_controller.h"

// QT classes to draw menu window
QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;

QT_END_NAMESPACE

// menu window class
class Menu: public QDialog {
    public: explicit Menu(QWidget * parent = nullptr);

    private slots: void StartSimulation();

    // elements of menu window
    private: QLabel * m_serialPortLabel = nullptr;
    QComboBox * m_serialPortComboBox = nullptr;
    QLabel * m_TrafficConesLabel = nullptr;
    QSpinBox * m_TrafficConesSpinBox = nullptr;
    QPushButton * m_startButton = nullptr;
    QLabel * m_ModeLabel = nullptr;
    QComboBox * m_ModeComboBox = nullptr;
};

// types of camera
enum cameraType {
    following = 0,
    internal = 1,
    top = 2
};

// types of robot control
enum controlType {
    velocity_control = 0,
    position_control = 1
};

// parameters for camera position
struct {
    float eye_x = 26;
    float eye_y = 5;
    float eye_z = 120;
    cameraType type = following;
}
camera;

// function which set viewport for camera
void set_viewport(int width, int height, cameraType cam, Robot robot) {
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 2000.0);

    if (cam == internal) {
        gluLookAt(robot.x, robot.y, 50, robot.x - width * cos(robot.rot_z * PI / 180), robot.y - width * sin(robot.rot_z * PI / 180), 10, 0, 0, 10);
    } else if (cam == following) {
        gluLookAt(robot.x + 150, camera.eye_y, camera.eye_z, robot.x, 0, 0, 0, 0, 10);
    } else if (cam == top) {
        gluLookAt(camera.eye_x, camera.eye_y, camera.eye_z, robot.x, robot.y, 0, 0, 0, 10);
    }
}

// function where we can change position of camera
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
            type = following;
            camera.eye_x = 26;
            camera.eye_y = 5;
            camera.eye_z = 120;
        } else if (type == following) {
            type = top;
            camera.eye_x = 0;
            camera.eye_y = 0;
            camera.eye_z = 500;
        } else if (type == top) {
            type = internal;
            camera.eye_x = 0;
            camera.eye_y = 0;
            camera.eye_z = 0;
        }
    }
}

// function where we connect to bluetooth device
bool connectBluetooth(QSerialPort * serial, QString serialport) {
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

// main function of visualisation
void play(int number_of_traffic_cones, QString serialport, QString mode) {

    // create the window
    sf::Window window(sf::VideoMode(1500, 768), "Robot Visualization", sf::Style::Default, sf::ContextSettings(32));

    //limit the number of frames
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // set max frame rate to 60 FPS
    window.setFramerateLimit(60);

    // variable where is saved time of one cicle of program
    float prev_time = 0;

    // variable which save time from last connection check
    float check_time = 0;

    // variable which return if is connection or not
    bool isConnection = 0;

    // clock variables
    sf::Clock plot_clock;
    sf::Clock update_clock;
    int from_prev_plot = 0;
    int from_prev_update = 0;

    // variables which return time of collision with traffic cone
    float collision_delay = 0;

    // setting default type of robot control
    controlType control_type = velocity_control;

    // file service
    std::ofstream myfile;

    // creating objects of visualization
    Room room;

    Robot robot;

    std::vector < TrafficCone > trafficCones;

    for (int i = 0; i < number_of_traffic_cones; i++) {
        trafficCones.push_back(TrafficCone(300 - 150 * i, 0));
    }

    // creating regulators for velocity and position control
    PID_controller pid_controller;

    PID_position_controller pid_position_controller;

    // setting background color
    glClearColor(0, 0, 0, 1);

    // setting OpenGL settings
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
    }; // surroundings
    GLfloat light_diffuse[] = {
        0.4,
        0.4,
        0.4,
        1.0
    }; // scattered
    GLfloat light_specular[] = {
        0.0,
        0.0,
        0.0,
        1.0
    }; // mirrored

    // setting light
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

    // creating clock object
    sf::Clock clk;

    // variables when will be saved input and output data
    std::string input;
    QByteArray output;
    QByteArray readData;
    std::vector < std::string > data {};

    // variables which help send or recieve data
    size_t posOfsep = 0;
    std::string separation_sign = ";";
    std::string dataToCut;
    std::string control;

    // creating serialport object
    QSerialPort * serial = new QSerialPort();
    // opening serial port if succesffuly then start visulisation
    bool running = connectBluetooth(serial, serialport);

    // add sound effect during collision with traffic cone
    sf::SoundBuffer buffer;
    sf::Sound sound;
    bool sound_played = 0;
    if (!buffer.loadFromFile("collision_sound.wav")) {
        std::cout << "Sound can't be load!" << std::endl;
    } else {
        sound.setBuffer(buffer);
    }

    // variables used to count ride time
    bool is_finish_point = false;
    bool count_time = 0;
    float ride_time = 0;

    // main loop of visulisation
    while (running) {
        // event service like close window or press keys
        sf::Event event;
        while (window.pollEvent(event)) {
            // close window
            if (event.type == sf::Event::Closed) {
                running = false;
                printf("\n");
            }
            if (event.type == sf::Event::KeyPressed) {
                // change camera
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

        // checking what type of control is set
        if (control_type == position_control) {
            // robot position control which return reference angular velocities of wheels
            std::tie(robot.left_wheel_velocity_ref, robot.right_wheel_velocity_ref) = pid_position_controller.PID_position_control_3(robot.y_ref, robot.x_ref, robot.y, robot.x, robot.rot_z, robot.track_between_wheels, robot.wheel_radius);

            // set robot wheels voltages in position control
            robot.u_left = pid_controller.PID_wheel_control(robot.left_wheel_velocity_ref, robot.left_wheel_velocity, "left");
            robot.u_right = pid_controller.PID_wheel_control(robot.right_wheel_velocity_ref, robot.right_wheel_velocity, "right");
        } else if (control_type == velocity_control) {
            // robot velocity control
            robot.u_left = pid_controller.PID_wheel_control(robot.left_wheel_velocity_ref, robot.left_wheel_velocity, "left");
            robot.u_right = pid_controller.PID_wheel_control(robot.right_wheel_velocity_ref, robot.right_wheel_velocity, "right");
        }

        // robot respond for getting voltage input
        robot.object_respond();

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);

        // draw stuff
        glPushMatrix();

        // robot movement
        robot.robot_movement(clk, prev_time, room.room_width, room.room_length);

        // collision with walls case
        robot.collisions();

        // check collision with traffic cones
        for (size_t i = 0; i < trafficCones.size(); i++) {
            if (robot.traffic_cone_robot_collisions(trafficCones[i])) {
                // if collison then move traffic cone
                trafficCones[i].pos_x += (clk.restart().asSeconds() - prev_time) * robot.linear_velocity * cos(robot.rot_z * PI / 180);
                trafficCones[i].pos_y += (clk.restart().asSeconds() - prev_time) * robot.linear_velocity * sin(robot.rot_z * PI / 180);
                robot.traffic_cones_collision = 1;
                ride_time = 0;
                count_time = false;
            }
        }

        // send message if is collision with walls or not
        if (robot.collision_front || robot.collision_rear || robot.collision_right || robot.collision_left) {
            robot.collision = 1;
        } else {
            robot.collision = 0;
        }

        // send message to connected device that there is collision
        if (robot.collision == 1 && robot.last_collision == 0) {
            output = "collision";
            serial -> write(output);
            robot.last_collision = 1;
        } else if (robot.collision == 0 && robot.last_collision == 1) {
            output = "ok";
            serial -> write(output);
            robot.last_collision = 0;
        }

        // restart robot position and play sound after robot collision with traffic cone
        if (robot.traffic_cones_collision) {
            if (!sound_played) {
                sound.play();
                sound_played = 1;
            }
            collision_delay += prev_time;
            if (collision_delay >= 1) {
                sound_played = 0;
                robot.traffic_cones_collision = 0;
                collision_delay = 0;
                robot.reset_robot_position();
                for (size_t i = 0; i < trafficCones.size(); i++) {
                    trafficCones[i].pos_x = 300 - 150 * static_cast < int > (i);
                    trafficCones[i].pos_y = 0;
                }
            }
        }

        // camera movement
        cameraHandling(clk, prev_time, camera.type, false);
        set_viewport(window.getSize().x, window.getSize().y, camera.type, robot);

        // update data to plot every 200ms
        from_prev_plot += plot_clock.restart().asMilliseconds();
        from_prev_update += update_clock.restart().asMilliseconds();
        if (from_prev_update >= 200) {

            for (size_t i = 0; i < 200 - 1; i++) {
                robot.x_tab[i] = robot.x_tab[i + 1];
                robot.y_tab[i] = robot.y_tab[i + 1];
                robot.uL_tab[i] = robot.uL_tab[i + 1];
                robot.uR_tab[i] = robot.uR_tab[i + 1];
                robot.wl_tab[i] = robot.wl_tab[i + 1];
                robot.wp_tab[i] = robot.wp_tab[i + 1];
                robot.wlref_tab[i] = robot.wlref_tab[i + 1];
                robot.wpref_tab[i] = robot.wpref_tab[i + 1];
                robot.xref_tab[i] = robot.xref_tab[i + 1];
                robot.yref_tab[i] = robot.yref_tab[i + 1];
            }
            robot.x_tab[199] = robot.x;
            robot.y_tab[199] = robot.y;
            robot.uL_tab[199] = robot.u_left;
            robot.uR_tab[199] = robot.u_right;
            robot.wl_tab[199] = robot.left_wheel_velocity;
            robot.wp_tab[199] = robot.right_wheel_velocity;
            robot.wlref_tab[199] = robot.left_wheel_velocity_ref;
            robot.wpref_tab[199] = robot.right_wheel_velocity_ref;
            robot.xref_tab[199] = robot.x_ref;
            robot.yref_tab[199] = robot.y_ref;

            from_prev_update = 0;
        }
        // new data to plot every 500ms
        if (from_prev_plot >= 500) {
            myfile.open("robot_data.txt", std::ios::out);
            myfile << "x;y;v,ω;ωL;ωP;ωLset;ωPset;xset;yset\n";
            for (size_t i = 0; i < 200; i++) {
                myfile << robot.x_tab[i] << ";" << robot.y_tab[i] << ";" << robot.uL_tab[i] << ";" << robot.uR_tab[i] << ";" << robot.wl_tab[i] << ";" << robot.wp_tab[i]
                          << ";" << robot.wlref_tab[i] << ";" << robot.wpref_tab[i] << ";" << robot.xref_tab[i] << ";" << robot.yref_tab[i] <<"\n";

            }
            myfile.close();
            from_prev_plot = 0;

        }

        // draw elements of visualisation
        room.draw_floor();
        room.draw_walls();
        room.draw_door();
        if (mode == "Count time mode") {
            room.draw_finish_point();
        }

        for (size_t i = 0; i < trafficCones.size(); i++) {
            trafficCones[i].draw_traffic_cone();
        }

        glTranslated(robot.x, robot.y, 0.0);
        glRotated(0, 1.0, 0.0, 0.0);
        glRotated(0, 0.0, 1.0, 0.0);
        glRotated(robot.rot_z, 0, 0.0, 1.0);

        robot.draw_robot();

        glPopMatrix();

        window.display();

        // checking if is connection with bluetooth device if not print text and set connection flag to 0
        if (check_time < 1.2) {
            check_time += prev_time;
        } else if (check_time >= 1.2) {
           // std::cout << "No connection!" << std::endl;
            isConnection = 0;
            // stop robot when connection lost
            robot.left_wheel_velocity_ref = 0;
            robot.right_wheel_velocity_ref = 0;
        }

        // read data from bluetooth device
        readData = serial -> readAll();
        serial -> waitForReadyRead(10);

        // if there is input data then save it
        if (readData.toStdString().length() > 0) {
            input = readData.toStdString();

            // check if there is input to robot data or connection check data
            if (input.find("check") != std::string::npos) {
                check_time = 0;
                serial -> write("connectionOK");
                isConnection = 1;
            } else {
                // received command "camera" change type of camera
                if (input.find("camera") != std::string::npos) {
                    cameraHandling(clk, prev_time, camera.type, true);
                }

                dataToCut = input;

                // check if there is robot control type change
                if (input.find("pos") != std::string::npos) {
                    control_type = position_control;
                    pid_position_controller.stop = 0;
                } else if (input.find("vel") != std::string::npos) {
                    control_type = velocity_control;
                }

                // seperate data from connected bluetooth device
                while ((posOfsep = dataToCut.find(separation_sign)) != std::string::npos) {
                    data.push_back(dataToCut.substr(0, posOfsep));
                    dataToCut.erase(0, posOfsep + separation_sign.length());
                }

                // send respond that data came succesfully
                output = QString::fromStdString(data[1]).toUtf8();
                serial -> write(output);

                // transform velocity or position data from string to float
                control = data[0];

                if (control_type == position_control) {
                    robot.input_extraction(control, 1);
                    pid_position_controller.start_x = robot.x;
                    pid_position_controller.start_y = robot.y;
                    robot.start_x = robot.x;
                    robot.start_y = robot.y;
                    robot.rot_z = robot.rot_z_0_360;
                    pid_position_controller.stop = 0;
                } else if (control_type == velocity_control) {
                    robot.input_extraction(control, 0);
                    robot.start_x = robot.x;
                    robot.start_y = robot.y;
                }
            }
            data.clear();
        }

        //std::cout << robot.x << ", " << robot.y << std::endl;

        // save time of last loop
        prev_time = clk.restart().asSeconds();

        if (mode == "Count time mode") {
            //count ride time of robot start moving and stop counting if robot reach destination
            if (robot.left_wheel_velocity_ref != 0 || robot.right_wheel_velocity_ref != 0) {
                count_time = true;
            }
            if (count_time) {
                ride_time += prev_time;
                is_finish_point = robot.finish_point_reach(room);
            }
            if (is_finish_point) {
                std::cout << "Ride time: " << ride_time << std::endl;
                is_finish_point = false;
                count_time = false;
                ride_time = 0.0;
                robot.reset_robot_position();
            }
        }
    }

    // close serial port after closing window
    serial -> close();
}


int main(int argc, char ** argv) {
    // create menu window
    QApplication app(argc, argv);
    Menu menu;
    menu.show();
    return app.exec();
}

// creating menu window function
Menu::Menu(QWidget * parent):
    QDialog(parent),
    m_serialPortLabel(new QLabel(tr("Serial port:"))),
    m_serialPortComboBox(new QComboBox),
    m_TrafficConesLabel(new QLabel(tr("How many traffic cones:"))),
    m_TrafficConesSpinBox(new QSpinBox),
    m_startButton(new QPushButton(tr("Start"))),
    m_ModeLabel(new QLabel(tr("Mode:"))),
    m_ModeComboBox(new QComboBox) {
        const auto infos = QSerialPortInfo::availablePorts();
        for (const QSerialPortInfo & info: infos)
            m_serialPortComboBox -> addItem(info.portName());

        m_ModeComboBox -> addItem("Free mode");
        m_ModeComboBox -> addItem("Count time mode");

        m_TrafficConesSpinBox -> setRange(0, 5);
        m_TrafficConesSpinBox -> setValue(0);

        auto mainLayout = new QGridLayout;
        mainLayout -> addWidget(m_serialPortLabel, 0, 0);
        mainLayout -> addWidget(m_serialPortComboBox, 0, 1);
        mainLayout -> addWidget(m_TrafficConesLabel, 1, 0);
        mainLayout -> addWidget(m_TrafficConesSpinBox, 1, 1);
        mainLayout -> addWidget(m_startButton, 0, 2, 2, 1);
        mainLayout -> addWidget(m_ModeLabel, 2, 0);
        mainLayout -> addWidget(m_ModeComboBox, 2, 1);
        setLayout(mainLayout);

        setWindowTitle(tr("Robot Visualization"));
        m_serialPortComboBox -> setFocus();

        connect(m_startButton, & QPushButton::clicked, this, & Menu::StartSimulation);
    }

// function with start visualisation after pressing start button in menu
void Menu::StartSimulation() {
    play(m_TrafficConesSpinBox -> value(), m_serialPortComboBox -> currentText(), m_ModeComboBox -> currentText());
}
