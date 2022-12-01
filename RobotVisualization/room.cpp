#include "room.h"

Room::Room()
{

}

void  Room::draw_floor() {
  glBegin(GL_POLYGON);
  glColor3f(255 / 255, 255 / 255, 255 / 255);
  glVertex3d(-room_width / 2, room_length / 2, 0);
  glVertex3d(room_width / 2, room_length / 2, 0);
  glVertex3d(room_width / 2, -room_length / 2, 0);
  glVertex3d(-room_width / 2, -room_length / 2, 0);
  glEnd();

  glColor3d(169 / 255.0, 169 / 255.0, 169 / 255.0);

  glLineWidth(4.0);
  glColor3f(50 / 255, 50 / 255, 255 / 255);

  int n = 6;
  for (int i = 1; i < n; i++) {
    glBegin(GL_LINES);
    glVertex3f(-room_width / 2, -room_length / 2 + i * room_length / n, 0);
    glVertex3f(room_width / 2, -room_length / 2 + i * room_length / n, 0);
    glEnd();
  }
  for (int i = 1; i < n; i++) {
    glBegin(GL_LINES);
    glVertex3f(-room_width / 2 + i * room_width / n, -room_length / 2, 0);
    glVertex3f(-room_width / 2 + i * room_width / n, room_length / 2, 0);
    glEnd();
  }

}

void Room::draw_walls() {
  // right
  glBegin(GL_POLYGON);
  glColor3f(50.f / 255, 50.f / 255, 255.f / 255);
  glVertex3d(-room_width / 2, room_length / 2, 0);
  glVertex3d(room_width / 2, room_length / 2, 0);
  glVertex3d(room_width / 2, room_length / 2, room_height);
  glVertex3d(-room_width / 2, room_length / 2, room_height);
  glEnd();

  // left
  glBegin(GL_POLYGON);
  glColor3f(50.f / 255, 50.f / 255, 255.f / 255);
  glVertex3d(-room_width / 2, -room_length / 2, 0);
  glVertex3d(room_width / 2, -room_length / 2, 0);
  glVertex3d(room_width / 2, -room_length / 2, room_height);
  glVertex3d(-room_width / 2, -room_length / 2, room_height);
  glEnd();

  // rear
  glBegin(GL_POLYGON);
  glColor3f(51.f / 255, 153.f / 255, 255.f / 255);
  glVertex3d(-room_width / 2, -room_length / 2, 0);
  glVertex3d(-room_width / 2, room_length / 2, 0);
  glVertex3d(-room_width / 2, room_length / 2, room_height);
  glVertex3d(-room_width / 2, -room_length / 2, room_height);
  glEnd();

  // front
  glBegin(GL_POLYGON);
  glColor3f(51.f / 255, 153.f / 255, 255.f / 255);
  glVertex3d(room_width / 2, -room_length / 2, 0);
  glVertex3d(room_width / 2, room_length / 2, 0);
  glVertex3d(room_width / 2, room_length / 2, room_height);
  glVertex3d(room_width / 2, -room_length / 2, room_height);
  glEnd();
}

void Room::draw_door() {
  glBegin(GL_POLYGON);
  glColor3d(110.0 / 255.0, 64.0 / 255.0, 48.0 / 255);
  glVertex3d(-doors_position / 2 + 1, -doors_width / 2, 0);
  glVertex3d(-doors_position / 2 + 1, doors_width / 2, 0);
  glVertex3d(-doors_position / 2 + 1, doors_width / 2, doors_height);
  glVertex3d(-doors_position / 2 + 1, -doors_width / 2, doors_height);
  glEnd();
}

void Room::draw_finish_point(){

    glColor4d(1.0, 0.0, 0.0, 0.4);

    glPushMatrix();
    glRotated(90.0, 1.0, 0.0, 0.0);
    glRotated(0, 0.0, 1.0, 0.0);
    glRotated(0, 0, 0.0, 1.0);

    int num = 20;

    glBegin(GL_QUAD_STRIP); // begin circle
    for (int i = 0; i <= num; i++) {
      glVertex3f(fp_x_position + fp_radius * cos(i * 2 * PI / num), fp_y_position,fp_radius * sin(i * 2 * PI / num));
      glVertex3f(fp_x_position + fp_radius * cos(i * 2 * PI / num), fp_y_position + fp_height,fp_radius * sin(i * 2 * PI / num));
      glVertex3f(fp_x_position + fp_radius * cos((i + 1) * 2 * PI / num), fp_y_position,fp_radius * sin((i + 1) * 2 * PI / num));
      glVertex3f(fp_x_position + fp_radius * cos((i + 1) * 2 * PI / num), fp_y_position + fp_height,fp_radius * sin((i + 1) * 2 * PI / num));
    }
    glEnd(); // end

    glPopMatrix();
}
