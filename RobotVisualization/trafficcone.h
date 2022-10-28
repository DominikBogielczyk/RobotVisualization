#ifndef TRAFFICCONE_H
#define TRAFFICCONE_H

#include <headers.h>
#include <drawingfunctions.h>

class TrafficCone
{
public:
    int num = 30;
    float h = 50;
    float r_up = 3;
    float r_down = 14;
    int dz = 1;
    float pos_x;
    float pos_y;
    float cube_length = 35;

    TrafficCone(float x, float y);
    void draw_traffic_cone();

};

#endif // TRAFFICCONE_H
