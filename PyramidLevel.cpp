#include "PyramidLevel.h"

PyramidLevel::PyramidLevel(const Image &inputImage, double levelSigma, int levelNumber) {
    image.copy(inputImage);
    number = levelNumber;
    sigma = levelSigma;
}

Image PyramidLevel::getImage() const {
    return image;
}

int PyramidLevel::getNumber() const {
    return number;
}

double PyramidLevel::getSigma() const {
    return sigma;
}

