#ifndef INTRESTINGPOINT_H
#define INTRESTINGPOINT_H
#include <QtMath>

class IntrestingPoint
{
private:
    int x,y;
    double value;
    double angle;
public:
    IntrestingPoint();
    IntrestingPoint(int inputX, int inputY, double inputValue);
    int getX() const;
    int getY() const;
    double getValue() const;
    double getAngle() const;
    void setValue(double inputValue);
    double getDistance(IntrestingPoint point) const;
    void setAngle(double inputAngle);
    void setX(int inputX);
    void setY(int inputY);
};

#endif // INTRESTINGPOINT_H
