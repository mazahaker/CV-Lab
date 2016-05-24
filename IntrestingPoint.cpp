#include "IntrestingPoint.h"

IntrestingPoint::IntrestingPoint() {
    x = 0;
    y = 0;
    value = 0.0;
    angle = 0.0;
    sigma = -1;
    octave = -1;
    level = -1;
    radius = 0;
}

IntrestingPoint::IntrestingPoint(int inputX, int inputY, double inputValue)
{
    x = inputX;
    y = inputY;
    value = inputValue;
    angle = 0.0;
}

IntrestingPoint::IntrestingPoint(int inputX, int inputY, double inputValue, int inputOctave, int inputLevel, double inputSigma, double inputRadius) {
    x = inputX;
    y = inputY;
    value = inputValue;
    angle = 0.0;
    octave = inputOctave;
    level = inputLevel;
    sigma = inputSigma;
    radius = inputRadius;
}

int IntrestingPoint::getX() const {
    return x;
}

int IntrestingPoint::getY() const {
    return y;
}

double IntrestingPoint::getValue() const {
    return value;
}

void IntrestingPoint::setValue(double inputValue) {
    value = inputValue;
}

void IntrestingPoint::setAngle(double inputAngle) {
    angle = inputAngle;
}

void IntrestingPoint::setX(int inputX) {
    x = inputX;
}

void IntrestingPoint::setY(int inputY) {
    y = inputY;
}

double IntrestingPoint::getDistance(IntrestingPoint point) const {
    return sqrt(pow((x - point.getX()),2) + pow((y - point.getY()),2));
}

double IntrestingPoint::getAngle() const {
    return angle;
}
int IntrestingPoint::getOctave() const {
    return octave;
}
int IntrestingPoint::getLevel() const {
    return level;
}

double IntrestingPoint::getRadius() const {
    return radius;
}

double IntrestingPoint::getSigma() const {
    return sigma;
}
