#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H
#include "Kernels.h"
#include "IntrestingPoint.h"

const int histogramsCount = 16;
const int binCount = 8;

class Descriptor
{
private:
    double histograms[histogramsCount * binCount];
    IntrestingPoint point;
public:
    Descriptor(const IntrestingPoint &inputPoint);

    void setHistValue(int histogramNumber, int basketNumber, double value);
    void addHistValue(int histogramNumber, int basketNumber, double value);
    double getHistValue(int histogramNumber, int basketNumber) const;
    void normalize();
    double getDistance(const Descriptor& descriptor) const;

    IntrestingPoint getIntrestingPoint();
};

#endif // DESCRIPTOR_H
