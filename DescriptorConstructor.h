#ifndef DESCRIPTORCUNSTRUCTOR_H
#define DESCRIPTORCUNSTRUCTOR_H
#include "Image.h"
#include "Descriptor.h"


class DescriptorConstructor
{
private:
    Image level;
    Image directionFi;
public:
    DescriptorConstructor(Image& inputImage);
    Descriptor createDescriptor(IntrestingPoint inputPoint);
    std::vector<IntrestingPoint> orientPoints(std::vector<IntrestingPoint> inputPoints);
};

#endif // DESCRIPTORCUNSTRUCTOR_H
