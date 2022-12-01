#include "trafficcone.h"

TrafficCone::TrafficCone(float x,float y)
{
    // set traffic cone position
    pos_x = x;
    pos_y = y;
}

void TrafficCone::draw_traffic_cone()
{
    // draw traffic cone with has two colors white and red
    float h1 = 0.7 * h;
    float r1 = (r_up - r_down) * 0.7 + r_down;
    float h2 = 0.35 * h;
    float r2 = (r_up - r_down) * 0.35 + r_down;

    glBegin(GL_QUAD_STRIP);
    glColor3f(255 / 255.f, 69 / 255.f, 0.f);
    for (int i = 0; i <= num; i++) {
      glVertex3f(pos_x + r_up * cos(i * 2 * PI / num), pos_y + r_up * sin(i * 2 * PI / num), dz + h);
      glVertex3f(pos_x + r1 * cos(i * 2 * PI / num), pos_y + r1 * sin(i * 2 * PI / num), dz + h1);
      glVertex3f(pos_x + r_up * cos((i + 1) * 2 * PI / num), pos_y + r_up * sin((i + 1) * 2 * PI / num), dz + h);
      glVertex3f(pos_x + r1 * cos((i + 1) * 2 * PI / num), pos_y + r1 * sin((i + 1) * 2 * PI / num), dz + h1);
    }
    glEnd(); // end

    glBegin(GL_QUAD_STRIP);
    glColor3f(255 / 255.f, 255 / 255.f, 255 / 255.f);
    for (int i = 0; i <= num; i++) {
      glVertex3f(pos_x + r1 * cos(i * 2 * PI / num), pos_y + r1 * sin(i * 2 * PI / num), dz + h1);
      glVertex3f(pos_x + r2 * cos(i * 2 * PI / num), pos_y + r2 * sin(i * 2 * PI / num), dz + h2);
      glVertex3f(pos_x + r1 * cos((i + 1) * 2 * PI / num), pos_y + r1 * sin((i + 1) * 2 * PI / num), dz + h1);
      glVertex3f(pos_x + r2 * cos((i + 1) * 2 * PI / num), pos_y + r2 * sin((i + 1) * 2 * PI / num), dz + h2);
    }
    glEnd(); // end

    glBegin(GL_QUAD_STRIP);
    glColor3f(255 / 255.f, 69 / 255.f, 0.f);
    for (int i = 0; i <= num; i++) {
      glVertex3f(pos_x + r2 * cos(i * 2 * PI / num), pos_y + r2 * sin(i * 2 * PI / num), dz + h2);
      glVertex3f(pos_x + r_down * cos(i * 2 * PI / num), pos_y + r_down * sin(i * 2 * PI / num), dz);
      glVertex3f(pos_x + r2 * cos((i + 1) * 2 * PI / num), pos_y + r2 * sin((i + 1) * 2 * PI / num), dz + h2);
      glVertex3f(pos_x + r_down * cos((i + 1) * 2 * PI / num), pos_y + r_down * sin((i + 1) * 2 * PI / num), dz);
    }
    glEnd(); // end

    draw_cube(cube_length, cube_length, 2, 1, pos_x, pos_y);
}
