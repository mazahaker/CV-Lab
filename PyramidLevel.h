#ifndef PYRAMIDLEVEL_H
#define PYRAMIDLEVEL_H
#include "Image.h"

class PyramidLevel
{
private:
    Image image;
    int number;
    double sigma;
public:
    PyramidLevel(const Image &image, double levelSigma, int levelNumber);
    Image getImage() const;
    int getNumber() const;
    double getSigma() const;
};

#endif // PYRAMIDLEVEL_H
