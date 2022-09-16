#include "headers.h"

#define PI 3.14159265
#define port "COM3"

void draw_circle(int x, int y, double radius, double width, double rot, char color, double dz=0.0)
{

    switch(color)
    {
    case 'g':
        glColor3d(0.5, 0.5, 0.4); //GREY
        break;
    case 'b':
        glColor3d(0.0, 0.0, 0.0); //BLACK
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
    for (int i = 0; i <= num; i++)
    {
        glVertex3f (x + radius * cos(i * 2 * PI / num), y, radius + radius * sin(i * 2 * PI / num) + dz);
    }
    glEnd(); //END

    glBegin(GL_QUAD_STRIP); //BEGIN CIRCLE
    for (int i = 0; i <= num; i++)
    {
        glVertex3f (x + radius * cos(i * 2 * PI / num), y, radius + radius * sin(i * 2 * PI / num) + dz);
        glVertex3f (x + radius * cos(i * 2 * PI / num), y + width, radius + radius * sin(i * 2 * PI / num) + dz);
        glVertex3f (x + radius * cos((i+1) * 2 * PI / num), y, radius + radius * sin((i+1) * 2 * PI / num) + dz);
        glVertex3f (x + radius * cos((i+1) * 2 * PI / num), y + width, radius + radius * sin((i+1) * 2 * PI / num) + dz);
    }
    glEnd(); //END

    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glVertex3f(x, y, radius); // center of circle
    for (int i = 0; i <= num; i++)
    {
        glVertex3f (x + radius * cos(i * 2 * PI / num), y + width, radius + radius * sin(i * 2 * PI / num) + dz);
    }
    glEnd(); //END
}


void draw_cube(double robot_size_x, double robot_size_y, double robot_size_z, double middle_z)
{
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

void draw_robot()
{
    const double robot_radius = 20.0;
    const double robot_height = 12.0;

    const double from_ground = 3.0;

    const double wheel_width = 1.0;
    const double wheel_radius = robot_height - from_ground;

    //BOX
    glColor3d(169/255.0, 169/255.0, 169/255.0);
    draw_cube(robot_radius, 2/1.5*robot_radius, 2 * wheel_radius - from_ground-1, wheel_radius+from_ground);
    //LEFT WHEEL
    draw_circle(0, -robot_radius/1.25, wheel_radius, wheel_width, 0.0, 'b'); //int x, int y, double radius, double width, double rot, char color
    //WHEEL AXLE
    draw_circle(0, -robot_radius/1.25, 2.0, 2*robot_radius/1.25, 0.0, 'b', wheel_radius);
    //RIGHT WHEEL
    draw_circle(0, robot_radius/1.25, wheel_radius, wheel_width, 0.0, 'b');
    //MIDDLE WHEEL
    draw_circle(-robot_radius/1.5, 0, 3.0, wheel_width, 0.0, 'b');
    //TRANSPARENT
    draw_circle(0, -robot_radius/1.25-7.5, wheel_radius, 10, 0.0, 't', 0.0); //int x, int y, double radius, double width, double rot, char color
    draw_circle(0, robot_radius/1.25-2.5, wheel_radius, robot_radius, 0.0, 't');
    draw_circle(-robot_radius/1.5, -wheel_width, 4.5, 3*wheel_width, 0.0, 't');
    //BOTTOM
    draw_circle(0, from_ground, robot_radius, 2.0, 90.0, 'g', -robot_radius); //(int x, int y, double radius, double width)
    //TOP
    draw_circle(0, 2 * wheel_radius + 1, robot_radius, 2.0, 0.0, 'g', -robot_radius);

}

void draw_floor(double width, double length)
{
    glBegin(GL_POLYGON);
    //glColor3d(225.0/255, 225.0/255, 208.0/255);
    glColor4d(0.5, 0.6, 0.4, 0.7);
    glVertex3d(-width/2, length/2, 0);
    glVertex3d(width/2, length/2, 0);
    glVertex3d(width/2, -length/2, 0);
    glVertex3d(-width/2, -length/2, 0);
    glEnd();
}

void draw_walls(double width, double length, double height)
{
    //RIGHT
    glBegin(GL_POLYGON);
    glColor3d(1.0, 1.0, 77.0/255);
    glVertex3d(-width/2, length/2, 0);
    glVertex3d(width/2, length/2, 0);
    glVertex3d(width/2, length/2, height);
    glVertex3d(-width/2, length/2, height);
    glEnd();

    //LEFT
    glBegin(GL_POLYGON);
    glColor3d(1.0, 1.0, 77.0/255);
    glVertex3d(-width/2, -length/2, 0);
    glVertex3d(width/2, -length/2, 0);
    glVertex3d(width/2, -length/2, height);
    glVertex3d(-width/2, -length/2, height);
    glEnd();

    //REAR
    glBegin(GL_POLYGON);
    glColor3d(1.0, 1.0, 153.0/255);
    glVertex3d(-width/2, -length/2, 0);
    glVertex3d(-width/2, length/2, 0);
    glVertex3d(-width/2, length/2, height);
    glVertex3d(-width/2, -length/2, height);
    glEnd();

    //FRONT
    glBegin(GL_POLYGON);
    glColor3d(1.0, 1.0, 153.0/255);
    glVertex3d(width/2, -length/2, 0);
    glVertex3d(width/2, length/2, 0);
    glVertex3d(width/2, length/2, height);
    glVertex3d(width/2, -length/2, height);
    glEnd();
}

void set_viewport(int width, int height, float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z)
{
    const float ar = (float)width / (float)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 1000.0);
    gluLookAt(eye_x, eye_y, eye_z, center_x, center_y, center_z, 0, 0, 10);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
}

void play()
{
    // create the window
    sf::Window window(sf::VideoMode(1500, 768), "Robot Visualization", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true); //limit the number of frames

    // activate the window
    window.setActive(true);

    float eye_x = 26;
    float eye_y = 5;
    float eye_z = 120;
    //float center_x = 0;
    //float center_y = 0;
    //float center_z = 0;

    float rot_z = 0;
    float move_x = 0;
    float move_y = 0;
    float prev_x = 0;
    float prev_y = 0;
    float prev_time = 0;

    const float linear_velocity = 200.0;
    const float angular_velocity = 60.0;
    const float camera_velocity = 150.0;

    //1200cm x 800cm x 250cm
    const double room_width = 1200.0;
    const double room_length = 800.0;
    const double room_height = 250.0;

    const double robot_radius = 20.0;


    // set viewport according to current window size
    //set_viewport(window.getSize().x, window.getSize().y, eye_x, eye_y, eye_z, center_x, center_y, center_z);

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    // setup lights

    GLfloat light_position[] = { eye_x, eye_y, eye_z, 1.0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 }; //otoczenie
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 }; //rozproszone
    GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 }; //lustrzane

    glLightfv( GL_LIGHT0, GL_POSITION, light_position);
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

    GLfloat global_ambient[] = {0.3, 0.3, 0.3, 0.1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);



    // load resources, initialize the OpenGL states, ...

    // run the main loop
    bool running = true;

    sf::Clock clk;

    std::string input;
    QByteArray readData;
    std::vector<std::string> data{};

    size_t  posOfSpace = 0;
    std::string space = " ";
    std::string dataToCut;
    long long sendTime;
    long long delay;

    QSerialPort *serial = new QSerialPort();
    serial->setPortName(port);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if(serial->open(QIODevice::ReadOnly))
    {
        running = true;
    }
    else
    {
        //error
        running = false;
        qDebug() << serial->errorString();
    }

    while (running) {
        // handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // end the program
                running = false;
            } else if (event.type == sf::Event::Resized) {
                // adjust the viewport when the window is resized
                 set_viewport(window.getSize().x, window.getSize().y, eye_x, eye_y, eye_z, move_x, move_y, 0);
            }
        }

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE) ;
        glEnable (GL_COLOR_MATERIAL);

        // draw stuff
        glPushMatrix();

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || input.find("left") != std::string::npos) {
            rot_z -=  (clk.restart().asSeconds() - prev_time) * angular_velocity;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || input.find("right") != std::string::npos) {
            rot_z +=  (clk.restart().asSeconds() - prev_time) * angular_velocity;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || input.find("up") != std::string::npos) {
            if(abs(move_x-(room_width/2)) > robot_radius &&  abs(move_x+(room_width/2)) > robot_radius){
            prev_x = move_x;
            move_x +=  (clk.restart().asSeconds() - prev_time) * linear_velocity * cos (rot_z*PI/180);
            } else {
            move_x = prev_x;
            }
            if(abs(move_y-(room_length/2)) > robot_radius &&  abs(move_y+(room_length/2)) > robot_radius){
            prev_y = move_y;
            move_y +=  (clk.restart().asSeconds() - prev_time) * linear_velocity * sin (rot_z*PI/180);
            } else {
            move_y = prev_y;
            }

            std::cout<<move_x<<" - "<<move_y<<std::endl;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || input.find("down") != std::string::npos) {
            if(abs(move_x-(room_width/2)) > robot_radius &&  abs(move_x+(room_width/2)) > robot_radius){
            prev_x = move_x;
            move_x -=  (clk.restart().asSeconds() - prev_time) * linear_velocity * cos (rot_z*PI/180);
            } else {
            move_x = prev_x;
            }
            if(abs(move_y-(room_length/2)) > robot_radius &&  abs(move_y+(room_length/2)) > robot_radius){
            prev_y = move_y;
            move_y -=  (clk.restart().asSeconds() - prev_time) * linear_velocity * sin (rot_z*PI/180);
            } else {
            move_y = prev_y;
            }
            std::cout<<move_x<<" - "<<move_y<<std::endl;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
            eye_x +=  (clk.restart().asSeconds() - prev_time) * camera_velocity;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
            eye_x -=  (clk.restart().asSeconds() - prev_time) * camera_velocity;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
            eye_y +=  (clk.restart().asSeconds() - prev_time) * camera_velocity;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
            eye_y -=  (clk.restart().asSeconds() - prev_time) * camera_velocity;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
            eye_z +=  (clk.restart().asSeconds() - prev_time) * camera_velocity;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
            eye_z -=  (clk.restart().asSeconds() - prev_time) * camera_velocity;
        }

        set_viewport(window.getSize().x, window.getSize().y, eye_x, eye_y, eye_z, move_x, move_y, 0);

        draw_floor(room_width, room_length);
        draw_walls(room_width, room_length, room_height);

        glTranslated(move_x, move_y, 0.0);
        glRotated(0, 1.0, 0.0, 0.0);
        glRotated(0, 0.0, 1.0, 0.0);
        glRotated(rot_z, 0, 0.0, 1.0);

        //ROBOT
        draw_robot();

        glPopMatrix();

        window.display();

        readData = serial->readAll();
        serial->waitForReadyRead(10);

        auto receiveTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();


        if(readData.toStdString().length()>0){
            input = readData.toStdString();

            dataToCut = input;


            while ((posOfSpace = dataToCut.find(space)) != std::string::npos) {
                    data.push_back(dataToCut.substr(0, posOfSpace));
                    dataToCut.erase(0, posOfSpace + space.length());
                }

           sendTime = std::stoll(data[0]);

           delay = receiveTime - sendTime;

           std::cout<<sendTime<<"-"<<receiveTime<<std::endl;
           std::cout<<"Delay: "<<delay<<std::endl;
           posOfSpace = 0;
           data.clear();

        }

        //sendTime = std::stol(data[0]);

        prev_time = clk.restart().asSeconds();

        // std::cout << eye_x << " " << eye_y << " " << eye_z << std::endl;

    }

    serial->close();

}




int main() {
    play();
    return 0;
}
