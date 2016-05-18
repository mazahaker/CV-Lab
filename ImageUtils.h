#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H
#include "Image.h"


class ImageUtils
{
public:
    static Image normalize(Image image);
    static std::vector<IntrestingPoint> ANMS(std::vector<IntrestingPoint> points, int pointsCount);
    static double getDistance(double x0, double x1, double y0, double y1);
};

#endif // IMAGEUTILS_H
