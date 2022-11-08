#ifndef ROOM_H
#define ROOM_H

#include <headers.h>
#include <drawingfunctions.h>

class Room
{
public:
    //ROOM PARAMETERS[CM]
    const double room_width = 1200.0;
    const double room_length = 800.0;
    const double room_height = 250.0;

    //DOOR PARAMETERS
    const double doors_height = 200.0;
    const double doors_width = 100.0;
    const double doors_position = 1200.0;

    //FINISH POINT VARIABLES
    double fp_x_position = -400;
    double fp_y_position = 0;
    double fp_radius = 10;
    double fp_height = 50.0;

    Room();
    void draw_floor();
    void draw_walls();
    void draw_doors();
    void draw_finish_point();
};

#endif // ROOM_H
