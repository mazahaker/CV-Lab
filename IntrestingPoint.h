#ifndef INTRESTINGPOINT_H
#define INTRESTINGPOINT_H
#include <QtMath>

class IntrestingPoint
{
private:
    int x,y;
    double value;
    double angle;
    int octave;
    int level;
    int radius;
    double sigma;
public:
    IntrestingPoint();
    IntrestingPoint(int inputX, int inputY, double inputValue);
    IntrestingPoint(int inputX, int inputY, double inputValue, int inputOctave, int inputLevel, double inputSigma, double inputRadius);
    int getX() const;
    int getY() const;
    int getOctave() const;
    int getLevel() const;
    double getValue() const;
    double getAngle() const;
    void setValue(double inputValue);
    double getDistance(IntrestingPoint point) const;
    void setAngle(double inputAngle);
    void setX(int inputX);
    void setY(int inputY);
    double getRadius() const;
    double getSigma() const;
};

#endif // INTRESTINGPOINT_H
