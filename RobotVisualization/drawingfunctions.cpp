#include <drawingfunctions.h>


void draw_circle(int x, int y, double radius, double width, double rot, char color, double dz) {

  switch (color) {
  case 'g':
    glColor3f(0 / 255.f, 0 / 255.f, 0 / 255.f); //BLACK
    break;
  case 'b':
    glColor3d(0 / 255.f, 0 / 255.f, 153 / 255.f); //BLUE
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

void draw_cube(double robot_size_x, double robot_size_y, double robot_size_z, double middle_z, int dx, int dy) {
  double half_size_x = robot_size_x / 2.0;
  double half_size_y = robot_size_y / 2.0;
  double half_size_z = robot_size_z / 2.0;

  //glColor3d(0.5, 0.5, 0.4); //GREY
  // bottom
  glBegin(GL_POLYGON);
  glVertex3d(-half_size_x + dx, half_size_y + dy, middle_z - half_size_z);
  glVertex3d(half_size_x + dx, half_size_y + dy, middle_z - half_size_z);
  glVertex3d(half_size_x + dx, -half_size_y + dy, middle_z - half_size_z);
  glVertex3d(-half_size_x + dx, -half_size_y + dy, middle_z - half_size_z);
  glEnd();

  // top
  glBegin(GL_POLYGON);
  glVertex3d(-half_size_x + dx, half_size_y + dy, middle_z + half_size_z);
  glVertex3d(half_size_x + dx, half_size_y + dy, middle_z + half_size_z);
  glVertex3d(half_size_x + dx, -half_size_y + dy, middle_z + half_size_z);
  glVertex3d(-half_size_x + dx, -half_size_y + dy, middle_z + half_size_z);
  glEnd();

  // left
  glBegin(GL_POLYGON);
  glVertex3d(-half_size_x + dx, -half_size_y + dy, middle_z + half_size_z);
  glVertex3d(-half_size_x + dx, half_size_y + dy, middle_z + half_size_z);
  glVertex3d(-half_size_x + dx, half_size_y + dy, middle_z - half_size_z);
  glVertex3d(-half_size_x + dx, -half_size_y + dy, middle_z - half_size_z);
  glEnd();

  // right
  glBegin(GL_POLYGON);
  glVertex3d(half_size_x + dx, -half_size_y + dy, middle_z + half_size_z);
  glVertex3d(half_size_x + dx, half_size_y + dy, middle_z + half_size_z);
  glVertex3d(half_size_x + dx, half_size_y + dy, middle_z - half_size_z);
  glVertex3d(half_size_x + dx, -half_size_y + dy, middle_z - half_size_z);
  glEnd();

  // front
  glBegin(GL_POLYGON);
  glVertex3d(-half_size_x + dx, -half_size_y + dy, middle_z + half_size_z);
  glVertex3d(half_size_x + dx, -half_size_y + dy, middle_z + half_size_z);
  glVertex3d(half_size_x + dx, -half_size_y + dy, middle_z - half_size_z);
  glVertex3d(-half_size_x + dx, -half_size_y + dy, middle_z - half_size_z);
  glEnd();

  // back
  glBegin(GL_POLYGON);
  glVertex3d(-half_size_x + dx, half_size_y + dy, middle_z + half_size_z);
  glVertex3d(half_size_x + dx, half_size_y + dy, middle_z + half_size_z);
  glVertex3d(half_size_x + dx, half_size_y + dy, middle_z - half_size_z);
  glVertex3d(-half_size_x + dx, half_size_y + dy, middle_z - half_size_z);
  glEnd();

}
