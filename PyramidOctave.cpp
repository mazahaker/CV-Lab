#include "PyramidOctave.h"

PyramidOctave::PyramidOctave(int octaveNumber)
{
    number = octaveNumber;
}

void PyramidOctave::addLevel(PyramidLevel &level) {
    levels.push_back(level);
}

PyramidLevel PyramidOctave::getLevel(int i) const {
    if(levels.size() > i) {
        return levels.at(i);
    }
}

int PyramidOctave::getNumber() const {
    return number;
}

int PyramidOctave::getLevelsCount() const {
    return levels.size();
}

