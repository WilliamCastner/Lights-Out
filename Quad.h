#ifndef LIGHTS_OUT_WCASTNER_QUAD_H
#define LIGHTS_OUT_WCASTNER_QUAD_H

#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>

//struct for color
struct color {
    double red;
    double green;
    double blue;
};

//struct for coords
struct coordinates {
    int x;
    int y;
};

class Quad {
protected:
    color fill;
    coordinates center;
    unsigned int width;
    unsigned int height;
    color yellow = {1, 1, 0};
    color grey = {.2, .2, .2};

public:
    //default constructor
    Quad();

    //constructor with color, center, width, and height
    Quad(color fill, coordinates center, unsigned int width, unsigned int height);

    //gets left x value of quad
    int getLeftX() const;

    //gets right x value of quad
    int getRightX() const;

    //gets top y value
    int getTopY() const;

    //gets bottom y value
    int getBottomY() const;

    //gets the fill
    color getFill() const;

    //sets the fill with a color
    void setColor(color fill);

    //runs when quad is clicked
    void pressDown();

    //runs when quad is hovered over
    void hover();

    //runs when quad is not hovered over
    void notHover();

    //checks x and y to see if it is overlapping quad
    bool isOverlapping(int x, int y);

    //draw function
    virtual void draw() const;
};


#endif //LIGHTS_OUT_WCASTNER_QUAD_H
