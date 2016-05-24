#ifndef PYRAMIDOCTAVE_H
#define PYRAMIDOCTAVE_H
#include "PyramidLevel.h"


class PyramidOctave
{
private:
    std::vector<PyramidLevel> levels;         //слои
    int number;
public:
    PyramidOctave(int num);
    void addLevel(PyramidLevel &level);
    PyramidLevel &getLevel(int i);
    int getNumber() const;
    int getLevelsCount() const;
    std::vector<PyramidLevel> getLevels() const;
};

#endif // PYRAMIDOCTAVE_H
