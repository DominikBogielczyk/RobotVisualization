#ifndef DRAWINGFUNCTIONS_H
#define DRAWINGFUNCTIONS_H

#include <headers.h>

void draw_circle(int x, int y, double radius, double width, double rot, char color, double dz = 0.0);
void draw_cube(double robot_size_x, double robot_size_y, double robot_size_z, double middle_z, int dx = 0, int dy = 0);
void draw_floor(double width, double length);
void draw_walls(double width, double length, double height);
void draw_doors(double height, double width, double position);

#endif // DRAWINGFUNCTIONS_H
