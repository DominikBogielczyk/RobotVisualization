#include "bluetooth.h"

#define PI 3.14159265


void draw_robot(double size_x, double size_y, double size_z, double middle_z)
{
    double half_size_x = size_x / 2.0;
    double half_size_y = size_y / 2.0;
    double half_size_z = size_z / 2.0;

    // bottom
    glBegin(GL_POLYGON);
    glVertex3d(-half_size_x, half_size_y, middle_z - half_size_z);
    glVertex3d(half_size_x, half_size_y, middle_z - half_size_z);
    glVertex3d(half_size_x, -half_size_y, middle_z - half_size_z);
    glVertex3d(-half_size_x, -half_size_y, middle_z - half_size_z);
    glEnd();

    // top
    glColor3d(0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(-half_size_x, half_size_y, middle_z + half_size_z);
    glVertex3d(half_size_x, half_size_y, middle_z + half_size_z);
    glVertex3d(half_size_x, -half_size_y, middle_z + half_size_z);
    glVertex3d(-half_size_x, -half_size_y, middle_z + half_size_z);
    glEnd();

    // left
    glColor3d(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(-half_size_x, -half_size_y, middle_z + half_size_z);
    glVertex3d(-half_size_x, half_size_y, middle_z + half_size_z);
    glVertex3d(-half_size_x, half_size_y, middle_z - half_size_z);
    glVertex3d(-half_size_x, -half_size_y, middle_z - half_size_z);
    glEnd();

    // right
    glColor3d(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(half_size_x, -half_size_y, middle_z + half_size_z);
    glVertex3d(half_size_x, half_size_y, middle_z + half_size_z);
    glVertex3d(half_size_x, half_size_y, middle_z - half_size_z);
    glVertex3d(half_size_x, -half_size_y, middle_z - half_size_z);
    glEnd();

    // front
    glColor3d(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(-half_size_x, -half_size_y, middle_z + half_size_z);
    glVertex3d(half_size_x, -half_size_y, middle_z + half_size_z);
    glVertex3d(half_size_x, -half_size_y, middle_z - half_size_z);
    glVertex3d(-half_size_x, -half_size_y, middle_z - half_size_z);
    glEnd();

    // back
    glColor3d(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(-half_size_x, half_size_y, middle_z + half_size_z);
    glVertex3d(half_size_x, half_size_y, middle_z + half_size_z);
    glVertex3d(half_size_x, half_size_y, middle_z - half_size_z);
    glVertex3d(-half_size_x, half_size_y, middle_z - half_size_z);
    glEnd();
}



void draw_circle(int x, int y, int num, double radius, double width)
{

    glColor3d(0.0, 0.0, 0.0); //BLACK

    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glVertex3f(x, y, radius); // center of circle
    for (int i = 0; i <= num; i++)
    {
        glVertex3f (x + radius * cos(i * 2 * PI / num), y, radius + radius * sin(i * 2 * PI / num));
    }
    glEnd(); //END

    glBegin(GL_QUAD_STRIP); //BEGIN CIRCLE
    for (int i = 0; i <= num; i++)
    {
        glVertex3f (x + radius * cos(i * 2 * PI / num), y, radius + radius * sin(i * 2 * PI / num));
        glVertex3f (x + radius * cos(i * 2 * PI / num), y + width, radius + radius * sin(i * 2 * PI / num));
        glVertex3f (x + radius * cos((i+1) * 2 * PI / num), y, radius + radius * sin((i+1) * 2 * PI / num));
        glVertex3f (x + radius * cos((i+1) * 2 * PI / num), y + width, radius + radius * sin((i+1) * 2 * PI / num));
    }
    glEnd(); //END

    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glVertex3f(x, y, radius); // center of circle
    for (int i = 0; i <= num; i++)
    {
        glVertex3f (x + radius * cos(i * 2 * PI / num), y + width, radius + radius * sin(i * 2 * PI / num));
    }
    glEnd(); //END

}

void draw_floor(double width, double length)
{
    glBegin(GL_POLYGON);
    glColor3d(225.0/255, 225.0/255, 208.0/255);
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




int main() {
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
    float prev_time = 0;

    const float linear_velocity = 200.0;
    const float angular_velocity = 60.0;
    const float camera_velocity = 150.0;


    //1200cm x 800cm x 250cm
    const double room_width = 1200.0;
    const double room_length = 800.0;
    const double room_height = 250.0;

    const double robot_size_x = 14.0;
    const double robot_size_y = 20.0;
    const double robot_size_z = 10.0;
    const double wheel_width = 1.0;
    const double wheel_radius = 8.0;


    // set viewport according to current window size
    //set_viewport(window.getSize().x, window.getSize().y, eye_x, eye_y, eye_z, center_x, center_y, center_z);

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    glShadeModel(GL_SMOOTH);

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

    glEnable(GL_NORMALIZE) ;

    // load resources, initialize the OpenGL states, ...

    // run the main loop
    bool running = true;
    float scale = 1.0;

    sf::Clock clk;


    Bluetooth blth = Bluetooth();

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

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            rot_z -=  (clk.restart().asSeconds() - prev_time) * angular_velocity;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            rot_z +=  (clk.restart().asSeconds() - prev_time) * angular_velocity;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            move_x +=  (clk.restart().asSeconds() - prev_time) * linear_velocity * cos (rot_z*PI/180);
            move_y +=  (clk.restart().asSeconds() - prev_time) * linear_velocity * sin (rot_z*PI/180);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            move_x -=  (clk.restart().asSeconds() - prev_time) * linear_velocity * cos (rot_z*PI/180);
            move_y -=  (clk.restart().asSeconds() - prev_time) * linear_velocity * sin (rot_z*PI/180);
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


       // glScaled(scale, scale, scale);

        //LEFT WHEEL
        draw_circle(0, -(robot_size_y/2 + 1) - wheel_width, 40, wheel_radius, wheel_width); //(int x, int y, int num, double radius, double width)

        //RIGHT WHEEL
        draw_circle(0, robot_size_y/2 + 1, 40, wheel_radius, wheel_width);

        //ROBOT
        draw_robot(robot_size_x, robot_size_y, robot_size_z, wheel_radius + robot_size_z/3); //(double size, double middle_z)

        glPopMatrix();

        window.display();

        prev_time = clk.restart().asSeconds();

       // std::cout << eye_x << " " << eye_y << " " << eye_z << std::endl;

    }

    return 0;
}
