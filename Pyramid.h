#ifndef PYRAMID_H
#define PYRAMID_H
#include "PyramidOctave.h"

class Pyramid
{
private:
    std::vector<PyramidOctave> octaves;         //октавы
public:
    Pyramid(const Image& inputImage, const double startSigma , const int octaveCount, const double levelCount);
    void addOctave(PyramidOctave &octave);
    PyramidOctave getOctave(int i) const;
    void pyramidSave(QString path);
};

#endif // PYRAMID_H
