#include "PyramidLevel.h"

PyramidLevel::PyramidLevel(const Image &inputImage, double levelSigma, int levelNumber) {
    image.copy(inputImage);
    number = levelNumber;
    sigma = levelSigma;
}

Image &PyramidLevel::getImage() {
    return image;
}

int PyramidLevel::getNumber() const {
    return number;
}

double PyramidLevel::getSigma() const {
    return sigma;
}

void PyramidLevel::setImageValue(int i,double value) {
    image.setPixel(i, value);
}

void PyramidLevel::setImage(Image inputImage) {
    image.copy(inputImage);
}

