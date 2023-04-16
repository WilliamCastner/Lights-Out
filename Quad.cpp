#include "Quad.h"

Quad::Quad() {
    fill = {0, 0, 0};
    center = {0, 0};
    width = 20;
    height = 29;
}

Quad::Quad(color fill, coordinates center, unsigned int width, unsigned int height) {
    this->fill = fill;
    this->center = center;
    this->width = width;
    this->height = height;

}

int Quad::getLeftX() const {
    return center.x - (width / 2);
}
int Quad::getRightX() const {
    return center.x + (width / 2);
}
int Quad::getTopY() const {
    return center.y - (height / 2);
}
int Quad::getBottomY() const {
    return center.y + (height / 2);
}

color Quad::getFill() const{
    return fill;
}

void Quad::setColor(color bla) {
    this->fill = bla;
}

void Quad::hover() {
    setColor(color({255, 0, 0}));
}

void Quad::notHover() {
    setColor(color({0,0,0}));
}

void Quad::pressDown() {
    if (fill.red == grey.red && fill.blue == grey.blue && fill.green == grey.green) {
        setColor(yellow);
    } else {
        setColor(grey);
    }
}

bool Quad::isOverlapping(int x, int y) {
    if (x < getRightX() &&
        x > getLeftX() &&
        y > getTopY() &&
        y < getBottomY()) {
        return true;
    }
    return false; // Placeholder for compilation
}

void Quad::draw() const {
    glColor3f(fill.red, fill.green, fill.blue);
    glBegin(GL_QUADS);
    glVertex2i(getLeftX(), getTopY());
    glVertex2i(getRightX(), getTopY());
    glVertex2i(getRightX(), getBottomY());
    glVertex2i(getLeftX(), getBottomY());
    glEnd();
}