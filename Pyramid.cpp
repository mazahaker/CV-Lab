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
                printf("temp sigma %lf\n", tempSigma);
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

int Pyramid::getOctavesCount() {
    return octaves.size();
}

PyramidOctave &Pyramid::getOctave(int i) {
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

void Pyramid::createBlobs() {
    for(int octaveIndex = 0; octaveIndex < octaves.size(); octaveIndex++){
//Разность гаусианов
        for(int levelIndex = 0; levelIndex < octaves.at(octaveIndex).getLevelsCount() - 1; levelIndex++){
            for(int x = 0; x < octaves.at(octaveIndex).getLevel(levelIndex).getImage().getWidth(); x++){
                for(int y = 0; y < octaves.at(octaveIndex).getLevel(levelIndex).getImage().getHeight(); y++){
                    octaves.at(octaveIndex).getLevel(levelIndex).getImage().setPixel(x,y, octaves.at(octaveIndex).getLevel(levelIndex+1).getImage().getPixel(x,y)-octaves.at(octaveIndex).getLevel(levelIndex).getImage().getPixel(x,y));
                }
            }
        }

        octaves.at(octaveIndex).getLevels().pop_back();

        for(int firstLevelIndex = 1; firstLevelIndex < octaves.at(octaveIndex).getLevelsCount() - 1; firstLevelIndex++){
            std::vector<IntrestingPoint> blobs;

            Image temp1(octaves[octaveIndex].getLevel(firstLevelIndex).getImage());
            Image temp2(octaves[octaveIndex].getLevel(firstLevelIndex+1).getImage());
            Image temp3(octaves[octaveIndex].getLevel(firstLevelIndex-1).getImage());

            for(int x = 1; x < temp1.getWidth()-1; x++){
                for(int y = 1; y <temp1.getHeight()-1; y++){
                    double extremumValue = temp1.getPixel(x,y);
                    bool max = true;
                    bool min = true;
                    if(temp1.getPixel(x-1, y+1) >= extremumValue || temp2.getPixel(x-1, y+1) >= extremumValue || temp3.getPixel(x-1, y+1) >= extremumValue) {
                        max = false;
                    }
                    if(temp1.getPixel(x, y+1) >= extremumValue || temp2.getPixel(x, y+1) >= extremumValue || temp3.getPixel(x, y+1) >= extremumValue) {
                        max = false;
                    }
                    if(temp1.getPixel(x+1, y+1) >= extremumValue || temp2.getPixel(x+1, y+1) >= extremumValue || temp3.getPixel(x+1, y+1) >= extremumValue) {
                        max = false;
                    }
                    if(temp1.getPixel(x-1, y) >= extremumValue || temp2.getPixel(x-1, y) >= extremumValue || temp3.getPixel(x-1, y) >= extremumValue) {
                        max = false;
                    }
                    if(temp2.getPixel(x, y) >= extremumValue || temp3.getPixel(x, y) >= extremumValue) {
                        max = false;
                    }
                    if(temp1.getPixel(x+1, y) >= extremumValue || temp2.getPixel(x+1, y) >= extremumValue || temp3.getPixel(x+1, y) >= extremumValue) {
                        max = false;
                    }
                    if(temp1.getPixel(x-1, y-1) >= extremumValue || temp2.getPixel(x-1, y-1) >= extremumValue || temp3.getPixel(x-1, y-1) >= extremumValue) {
                        max = false;
                    }
                    if(temp1.getPixel(x, y-1) >= extremumValue || temp2.getPixel(x, y-1) >= extremumValue || temp3.getPixel(x, y-1) >= extremumValue) {
                        max = false;
                    }
                    if(temp1.getPixel(x+1, y-1) >= extremumValue || temp2.getPixel(x+1, y-1) >= extremumValue || temp3.getPixel(x+1, y-1) >= extremumValue) {
                        max = false;
                    }


                    if(temp1.getPixel(x-1, y+1) <= extremumValue || temp2.getPixel(x-1, y+1) <= extremumValue || temp3.getPixel(x-1, y+1) <= extremumValue) {
                        min = false;
                    }
                    if(temp1.getPixel(x, y+1) <= extremumValue || temp2.getPixel(x, y+1) <= extremumValue || temp3.getPixel(x, y+1) <= extremumValue) {
                        min = false;
                    }
                    if(temp1.getPixel(x+1, y+1) <= extremumValue || temp2.getPixel(x+1, y+1) <= extremumValue || temp3.getPixel(x+1, y+1) <= extremumValue) {
                        min = false;
                    }
                    if(temp1.getPixel(x-1, y) <= extremumValue || temp2.getPixel(x-1, y) <= extremumValue || temp3.getPixel(x-1, y) <= extremumValue) {
                        min = false;
                    }
                    if(temp2.getPixel(x, y) <= extremumValue || temp3.getPixel(x, y) <= extremumValue) {
                        min = false;
                    }
                    if(temp1.getPixel(x+1, y) <= extremumValue || temp2.getPixel(x+1, y) <= extremumValue || temp3.getPixel(x+1, y) <= extremumValue) {
                        min = false;
                    }
                    if(temp1.getPixel(x-1, y-1) <= extremumValue || temp2.getPixel(x-1, y-1) <= extremumValue || temp3.getPixel(x-1, y-1) <= extremumValue) {
                        min = false;
                    }
                    if(temp1.getPixel(x, y-1) <= extremumValue || temp2.getPixel(x, y-1) <= extremumValue || temp3.getPixel(x, y-1) <= extremumValue) {
                        min = false;
                    }
                    if(temp1.getPixel(x+1, y-1) >= extremumValue || temp2.getPixel(x+1, y-1) >= extremumValue || temp3.getPixel(x+1, y-1) >= extremumValue) {
                        min = false;
                    }


                    if(max || min){
                        IntrestingPoint blob(
                                    x,
                                    y,
                                    extremumValue,
                                    octaveIndex,
                                    firstLevelIndex,
                                    octaves.at(octaveIndex).getLevel(firstLevelIndex).getSigma(),
                                    sqrt(2)*(octaves.at(octaveIndex).getLevel(firstLevelIndex).getSigma() * (double)pow(2,octaveIndex))
                                    );

                        double harris = octaves.at(octaveIndex).getLevel(firstLevelIndex).getImage().harrisForPoint(blob);

                        if(fabs(harris) > 1e-6){
                            blobs.push_back(blob);
                        }
                    }
                }
            }
            octaves.at(octaveIndex).getLevel(firstLevelIndex).getImage().setPoints(blobs);
        }
    }
}

