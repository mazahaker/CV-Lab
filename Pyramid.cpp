#include "Pyramid.h"

Pyramid::Pyramid(const Image &inputImage, const double inputSigma, const int octaveCount, const double levelCount)
{
    Image levelImage = inputImage;

    double levelSigma = inputSigma;
    Image internalImage = levelImage.gaussFilter(sqrt(inputSigma * inputSigma - 0.5 * 0.5));

    for(int octaveNumber = 0; octaveNumber < octaveCount; octaveNumber++) {
        PyramidOctave octave(octaveNumber);
        levelSigma = inputSigma;

        if(octaveNumber > 0) {
            internalImage.copy(getOctave(octaveNumber-1).getLevel(levelCount-3).getImage());
            internalImage.changeSize(qRound((double)internalImage.getWidth() / 2.0), qRound((double)internalImage.getHeight() / 2.0));
        }
        for(int levelNumber = 0; levelNumber < levelCount + 2; levelNumber++) {
            if(levelNumber == 0 ) {
                PyramidLevel level(internalImage,levelSigma,levelNumber);
                octave.addLevel(level);
            } else {
                double tempSigma;
                levelSigma = inputSigma * pow(pow(2.0, 1.0 / (levelCount-1)), levelNumber);
                internalImage.copy(octave.getLevel(levelNumber-1).getImage());
                tempSigma = sqrt(levelSigma * levelSigma - octave.getLevel(levelNumber-1).getSigma() * octave.getLevel(levelNumber-1).getSigma());
                PyramidLevel level(internalImage.gaussFilter(tempSigma),levelSigma,levelNumber);
                octave.addLevel(level);
            }

        }
        addOctave(octave);
    }
}

void Pyramid::addOctave(PyramidOctave &octave) {
    octaves.push_back(octave);
}

PyramidOctave Pyramid::getOctave(int i) const {
    if(octaves.size() > i) {
        return octaves.at(i);
    }
}

void Pyramid::pyramidSave(QString path) {
    for(int i = 0; i < octaves.size(); i++){
        for(int j = 0; j < octaves.at(i).getLevelsCount(); j++){
            QString fullPath = path + "octave_" + QString::number(octaves.at(i).getNumber()) +
                    "_level_" + QString::number(octaves.at(i).getLevel(j).getNumber()) + "_sigma_" + QString::number(octaves.at(i).getLevel(j).getSigma(), 'f', 3) + ".bmp";
            QString message = "octave_" + QString::number(octaves.at(i).getNumber()) +
                    "_level_" + QString::number(octaves.at(i).getLevel(j).getNumber()) + "_sigma_" + QString::number(octaves.at(i).getLevel(j).getSigma(), 'f', 3);
            octaves.at(i).getLevel(j).getImage().saveImage(fullPath,message);
        }
    }
}

