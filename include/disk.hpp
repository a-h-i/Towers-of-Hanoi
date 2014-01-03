#pragma once
#include "gl.hpp"
#include "point.hpp"
#include "color.hpp"

class Disk{
private:
    Point disk_origin;
    float rgba[4];
    GLUquadricObj *quad;
	double in_radius;
	double out_radius;
    int slices;
    int loops;
    void construct_disk();
public:
	Disk(Point, Color ,double, double);
    Disk(Point, Color ,double, double, int, int);
	void draw();
	void move();//fix me
};