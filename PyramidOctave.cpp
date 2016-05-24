#include "PyramidOctave.h"

PyramidOctave::PyramidOctave(int octaveNumber)
{
    number = octaveNumber;
}

void PyramidOctave::addLevel(PyramidLevel &level) {
    levels.push_back(level);
}

PyramidLevel &PyramidOctave::getLevel(int i)  {
    if(levels.size() > i) {
        return levels[i];
    }
}

int PyramidOctave::getNumber() const {
    return number;
}

int PyramidOctave::getLevelsCount() const {
    return levels.size();
}

std::vector<PyramidLevel> PyramidOctave::getLevels() const{
    return levels;
}

