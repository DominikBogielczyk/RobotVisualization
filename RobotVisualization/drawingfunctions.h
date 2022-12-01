#ifndef DRAWINGFUNCTIONS_H
#define DRAWINGFUNCTIONS_H

#include <headers.h>

// function which draw circle shape
void draw_circle(int x, int y, double radius, double width, double rot, char color, double dz = 0.0);

// function which draw cube
void draw_cube(double robot_size_x, double robot_size_y, double robot_size_z, double middle_z, int dx = 0, int dy = 0);

#endif // DRAWINGFUNCTIONS_H
