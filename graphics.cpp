#include "graphics.h"
#include <iostream>
#include <algorithm>
#include <time.h>
#include <vector>
#include "Quad.h"
using namespace std;


// Global variables
GLdouble width, height;
int wd;
bool welcome = true;
bool victory();
vector<Quad> squareVec;
vector<Quad> borderVec;
int clickCount = 0;
int times = 0;
time_t beginningTime = time (NULL);


void init() {
    // I was getting the same random numbers everytime, adding this line fixed it
    // https://stackoverflow.com/questions/9421463/rand-generating-same-number-upon-compilation
    srand(time(0));
    //initialize vectors
    for (int i = 0; i < 5; i++) {
        for (int q = 0; q < 5; q++) {
            //randomize colors
            if (rand() % 10 > 5) {
                squareVec.push_back(Quad({1,1,0}, {100+(i*100), 90+(q*100)}, 75, 75));
            } else {
                squareVec.push_back(Quad({.2,.2,.2}, {100+(i*100), 90+(q*100)}, 75, 75));
            }
            borderVec.push_back(Quad({0,0,0}, {100+(i*100), 90+(q*100)}, 80, 80));
        }
    }
    width = 600;
    height = 600;
    srand(time(0));
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

void welcomeDisplay() {
    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height);

    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    drawText("Welcome to Lights Out", 200, 150);
    drawText("Clicking tiles turns them on / off", 150, 200);
    drawText("The goal is to have all of the tiles off (grey)", 100, 250);

    drawText("Good luck, press s to continue.", 165, 400);

    glFlush();  // Render now
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height);

    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    //draw the borders and squares
    for (int i = 0; i < squareVec.size(); i++) {
        borderVec[i].draw();
        squareVec[i].draw();
    }

    //draws number of clicks
    drawText("Click count: " + to_string(clickCount), 15, 20);


    //keeps track of time, I was having trouble using the timer function defined by glut
    time_t seconds;

    seconds = time (NULL);
    drawText(to_string(seconds - beginningTime), 550, 20);

    //checks for victory
    if (victory()) {
        drawText("You win!", 295);
    }

    glFlush();  // Render now
}

bool victory() {
    for (int i = 0; i < squareVec.size(); i++) {
        if (squareVec[i].getFill().red == 1 &&
            squareVec[i].getFill().green == 1 &&
            squareVec[i].getFill().blue == 0) {
            return false;
        }
    } return true;
}

void drawText(string text, int y) {
    //draws text
    glColor3f(255, 255, 0);

    int stringWidth = 0;
    for (const char &letter: text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, letter);
        stringWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, letter);
    }
    //centers text, was not working when drawing multiple things per screen
    glRasterPos2i((width/2)-stringWidth/2, y);
}

void drawText(string text, int x, int y) {
    glColor3f(255, 255, 0);

    int stringWidth = 0;
    glRasterPos2i(x, y);
    for (const char &letter: text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, letter);
        stringWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, letter);
    }
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }

    //gets out of the initial display window
    if (key == 115 && welcome) {
        glutDisplayFunc(display);
        welcome = false;
    }

    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {
        break;
    }

    glutPostRedisplay();
}

void cursor(int x, int y) {
    //checks if the square is being hovered over and turns the border's color to red
    for (int i = 0; i < squareVec.size(); i++) {
        if (squareVec[i].isOverlapping(x,y)) {
            borderVec[i].hover();
        } else {
            borderVec[i].notHover();
        }
    }
    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {
    vector<int> bottomEdges = {0,5,10,15,20,25};
    vector<int> topEdges = {4,9,14,19};
    for (int i = 0; i < squareVec.size(); i++) {
        if ((!welcome && button == GLUT_LEFT_BUTTON) && (state == GLUT_UP) && (squareVec[i].isOverlapping(x,y))) {
            clickCount++;
            squareVec[i].pressDown();
            if (!(find(bottomEdges.begin(), bottomEdges.end(), i+1) != bottomEdges.end() )) {
                squareVec[i+1].pressDown();
            }
            if (!(find(topEdges.begin(), topEdges.end(), i-1) != topEdges.end() )) {
                squareVec[i-1].pressDown();
            }
            squareVec[i+5].pressDown();
            squareVec[i-5].pressDown();
        }
    }

    glutPostRedisplay();
}


void timer(int dummy) {
    glutPostRedisplay();
    glutTimerFunc(30, timer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Confetti!" /* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(welcomeDisplay);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}
