#ifndef ROOM_H
#define ROOM_H

#include <headers.h>
#include <drawingfunctions.h>

class Room
{
public:
    // room size parameters [cm]
    const double room_width = 1200.0;
    const double room_length = 800.0;
    const double room_height = 250.0;

    // door size parameters [cm]
    const double doors_height = 200.0;
    const double doors_width = 100.0;
    const double doors_position = 1200.0;

    // finish point position parameters
    double fp_x_position = -400;
    double fp_y_position = 0;
    double fp_radius = 10;
    double fp_height = 50.0;

    Room();

    // function which draw floor
    void draw_floor();

    // function which draw walls
    void draw_walls();

    // function which draw door
    void draw_door();

    // function which draw finish point
    void draw_finish_point();
};

#endif // ROOM_H
