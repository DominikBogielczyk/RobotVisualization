#define PI 3.14159265

void draw_cube(double size, double z) {
    double half_cube_size = size / 2.0;

    // bottom
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, half_cube_size, -z);
    glVertex3d(half_cube_size, half_cube_size, -z);
    glVertex3d(half_cube_size, -half_cube_size, -z);
    glVertex3d(-half_cube_size, -half_cube_size, -z);
    glEnd();

    // top
    glColor3d(0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(-half_cube_size, -half_cube_size, half_cube_size);
    glEnd();

    // left
    glColor3d(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(-half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(-half_cube_size, half_cube_size, -z);
    glVertex3d(-half_cube_size, -half_cube_size, -z);
    glEnd();

    // right
    glColor3d(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, -z);
    glVertex3d(half_cube_size, -half_cube_size, -z);
    glEnd();

    // front
    glColor3d(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, -z);
    glVertex3d(-half_cube_size, -half_cube_size, -z);
    glEnd();

    // back
    glColor3d(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, -z);
    glVertex3d(-half_cube_size, half_cube_size, -z);
    glEnd();
}

void draw_floor(double width, double length, double z)
{
    glBegin(GL_POLYGON);
    glColor3d(225.0/255, 225.0/255, 208.0/255);
    glVertex3d(-width/2, length/2, z);
    glVertex3d(width/2, length/2, z);
    glVertex3d(width/2, -length/2, z);
    glVertex3d(-width/2, -length/2, z);
    glEnd();
}

void draw_walls(double width, double length, double z, double height)
{
    //RIGHT
    glBegin(GL_POLYGON);
    glColor3d(1.0, 1.0, 77.0/255);
    glVertex3d(-width/2, length/2, z);
    glVertex3d(width/2, length/2, z);
    glVertex3d(width/2, length/2, z+height);
    glVertex3d(-width/2, length/2, z+height);
    glEnd();

    //LEFT
    glBegin(GL_POLYGON);
    glColor3d(1.0, 1.0, 77.0/255);
    glVertex3d(-width/2, -length/2, z);
    glVertex3d(width/2, -length/2, z);
    glVertex3d(width/2, -length/2, z+height);
    glVertex3d(-width/2, -length/2, z+height);
    glEnd();

    //REAR
    glBegin(GL_POLYGON);
    glColor3d(1.0, 1.0, 153.0/255);
    glVertex3d(-width/2, -length/2, z);
    glVertex3d(-width/2, length/2, z);
    glVertex3d(-width/2, length/2, z+height);
    glVertex3d(-width/2, -length/2, z+height);
    glEnd();
}

void set_viewport(int width, int height, float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z)
{
    const float ar = (float)width / (float)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    gluLookAt(eye_x, eye_y, eye_z, center_x, center_y, center_z, 0, 0, 1);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
}

void play()
{
    // create the window
    sf::Window window(sf::VideoMode(1500, 768), "Robot Visualization", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    float eye_x = 30;
    float eye_y = -15;
    float eye_z = 15;
    float center_x = 0;
    float center_y = 0;
    float center_z = 0;

    float rot_z = 0;
    float move_x = 0;
    float prev_time = 0;


    // set viewport according to current window size
    set_viewport(window.getSize().x, window.getSize().y, eye_x, eye_y, eye_z, center_x, center_y, center_z);

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    glShadeModel(GL_SMOOTH);

    // setup lights

    GLfloat light_position[] = { 2.0, 0.0, 2.0, 1.0 };
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };

    glLightfv( GL_LIGHT0, GL_POSITION, light_position);
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

    GLfloat global_ambient[] = {0.3, 0.3, 0.3, 0.1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glEnable(GL_NORMALIZE) ;

    // load resources, initialize the OpenGL states, ...

    // run the main loop
    bool running;
    float scale = 1.0;

    sf::Clock clk;

    std::string movement;
    QByteArray readData;

    QSerialPort *serial = new QSerialPort();
    serial->setPortName("COM3");
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
                 set_viewport(window.getSize().x, window.getSize().y, eye_x, eye_y, eye_z, center_x, center_y, center_z);
            }

            else if (sf::Event::MouseWheelScrolled) {
              //  scale += event.mouseWheelScroll.delta * 0.2;
                //std::cout << event.mouseWheelScroll.x << std::endl;

        }
        }

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE) ;
        glEnable (GL_COLOR_MATERIAL);

        // draw stuff

        glPushMatrix();



        readData = serial->readAll();
        serial->waitForReadyRead(10);

        if(readData.toStdString().length()>0){
            movement = readData.toStdString();
        }


        if(movement == "left") {
            rot_z -=  (clk.restart().asSeconds() - prev_time) * 30;
        }
        else if(movement == "right") {
            rot_z +=  (clk.restart().asSeconds() - prev_time) * 30;
        }
        else if(movement == "up") {
            move_x +=  (clk.restart().asSeconds() - prev_time) * 3;
        }
        else if(movement == "down") {
            move_x -=  (clk.restart().asSeconds() - prev_time) * 3;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
            eye_x +=  (clk.restart().asSeconds() - prev_time) * 15.0;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
            eye_x -=  (clk.restart().asSeconds() - prev_time) * 15.0;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
            eye_y +=  (clk.restart().asSeconds() - prev_time) * 15.0;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
            eye_y -=  (clk.restart().asSeconds() - prev_time) * 15.0;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
            eye_z +=  (clk.restart().asSeconds() - prev_time) * 15.0;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
            eye_z -=  (clk.restart().asSeconds() - prev_time) * 15.0;
        }

        set_viewport(window.getSize().x, window.getSize().y, eye_x, eye_y, eye_z, move_x, center_y, center_z);



        draw_floor(40, 80, 0); //(double width, double height, double z)
        draw_walls(40, 80, 0, 25); //(double width, double height, double z)

        glRotated(0, 1.0, 0.0, 0.0);
        glRotated(0, 0.0, 1.0, 0.0);
        glRotated(rot_z, 0, 0.0, 1.0);
        glTranslated(move_x, 0, 0.0);

        glScaled(scale, scale, scale);

        draw_cube(2.0, 0); //(double size)


        glPopMatrix();

       // std::cout << move_x << std::endl;
        // end the current frame (internally swaps the front and back buffers)
        window.display();

        prev_time = clk.restart().asSeconds();
    }
    serial->close();
}




int main() {

    play();

    return 0;
}
