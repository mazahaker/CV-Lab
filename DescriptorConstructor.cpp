#include "DescriptorConstructor.h"
#include "ImageUtils.h"

DescriptorConstructor::DescriptorConstructor(Image& inputImage)
{
    level.copy(inputImage.sobelGradient(inputImage.convolution(&sobelX[0][0], 3, 3), inputImage.convolution(&sobelY[0][0], 3, 3)));
    directionFi.copy(inputImage);
    Image sobelXImage(inputImage.convolution(&sobelX[0][0], 3, 3));
    Image sobelYImage(inputImage.convolution(&sobelY[0][0], 3, 3));
//    int t = rand()%255;
//    sobelXImage.saveImage("D:\\KazakovImages\\output\\" + QString::number(t) + "descSobelX.jpg", "desc sobelX saved");
//    sobelYImage.saveImage("D:\\KazakovImages\\output\\" + QString::number(t) + "descSobelY.jpg", "desc sobelY saved");

    for(int x = 0; x < inputImage.getWidth(); x++){
        for(int y = 0; y < inputImage.getHeight(); y++){
            directionFi.setPixel(x,y, atan2(sobelYImage.getPixel(x,y), sobelXImage.getPixel(x,y)) * 180.0 / M_PI + 180.0);
        }
    }
//    for(int x = 0; x < inputImage.getWidth(); x++){
//        for(int y = 0; y < inputImage.getHeight(); y++){
//            printf("%lf\n", directionFi.getPixel(x,y));
//        }
//    }
    //printf("Desc created\n");
}

Descriptor DescriptorConstructor::createDescriptor(const IntrestingPoint inputPoint) {
    Descriptor resultDescriptor(inputPoint);

    double binSize = 360.0 / binCount;
    int partsCount = 4;

    double descriptorRadius = 8.0;

    descriptorRadius *= inputPoint.getSigma();

    for(int i = -descriptorRadius; i < descriptorRadius; i++){
        for(int j = -descriptorRadius; j < descriptorRadius; j++){

            //Поворачиваем
            int angledX = (i * cos((360.0 - inputPoint.getAngle()) * M_PI / 180.0) - j * sin((360.0 - inputPoint.getAngle()) * M_PI / 180.0) + 0.5);
            int angledY = (i * sin((360.0 - inputPoint.getAngle()) * M_PI / 180.0) + j * cos((360.0 - inputPoint.getAngle()) * M_PI / 180.0) + 0.5);

            //В пределах дескриптора?
            if(ImageUtils::getDistance((double)angledX,0.0,(double)angledY,0.0) < sqrt(pow(descriptorRadius,2) + pow(descriptorRadius,2))){

                //За границей?
                if(angledX < -descriptorRadius) { angledX = 0; }
                else { if(angledX >= descriptorRadius) { angledX = descriptorRadius -1; }
                    else { angledX += descriptorRadius; }}
//angledX += descriptorRadius;
                if(angledY < -descriptorRadius) { angledY = 0; }
                else { if(angledY >= descriptorRadius) { angledY = descriptorRadius -1; }
                    else { angledY += descriptorRadius; }}
//angledY += descriptorRadius;
                //Направление Фи
                double localPfi =  directionFi.getPixel(inputPoint.getX() + i, inputPoint.getY() + j) - inputPoint.getAngle();

                //За границей?
                localPfi = (localPfi < 0) ? localPfi+360 : localPfi;
                localPfi = (localPfi > 360) ? localPfi-360 : localPfi;

                //Текущий х у
                int x = angledX / ((descriptorRadius * 2) / partsCount) -0.5;
                int y = angledY / ((descriptorRadius * 2) / partsCount) -0.5;

                //Номер гистограммы
                int histogramNumber = x * partsCount + y;
                //Номер корзины
                int binNumber = (localPfi / binSize + 0.5);

//                //Раскидываем по корзинам
//                double localBinCenter = (double)binNumber * binSize + binSize / 2.0;

//                int relatedBin;
//                if(localPfi < localBinCenter)
//                    relatedBin = binNumber - 1;
//                else
//                    relatedBin = binNumber + 1;

//                double thisCenterDistance = abs(localBinCenter - localPfi);
//                double relatedCenterDistance = binSize - thisCenterDistance;

//                resultDescriptor.addHistValue(histogramNumber, binNumber,  level.getPixel(inputPoint.getX() + i, inputPoint.getY() + j) * (1 - thisCenterDistance / binSize));
//                resultDescriptor.addHistValue(histogramNumber, relatedBin, level.getPixel(inputPoint.getX() + i, inputPoint.getY() + j) * (1 - relatedCenterDistance / binSize));


////////////////////////lab7
                double currentLevel = level.getPixel(inputPoint.getX() + i, inputPoint.getY() + j);
                for(int xx = x; xx <= x+1; xx++){

                    if(xx < 0 || xx >= partsCount) continue;
                    double xCenter = descriptorRadius/2 * xx + descriptorRadius / 4;
                    double leightX = abs(angledX - xCenter);
                    double weightX = 1 - leightX / (descriptorRadius/2);
                    if(xx == 0 && xCenter > angledX || xx == 3 && xCenter < angledX ) {
                        weightX = 1;
                    }

                    for(int yy = y; yy <= y+1; yy++){

                        if(yy < 0 || yy >= partsCount) continue;
                        double yCenter = descriptorRadius/2 * yy + descriptorRadius / 4;
                        double leightY = abs(angledX - yCenter);
                        double weightY = 1 - leightY / (descriptorRadius/2);
                        if(yy == 0 && yCenter > angledY || yy == 3 && yCenter < angledY ) {
                            weightY = 1;
                        }

                        for(int currentBin = binNumber - 1; currentBin <= binNumber; currentBin ++){

                            double localBinCenter = (double)currentBin * binSize + binSize / 2.0;
                            double leight = abs(localBinCenter - localPfi);
                            double weight = 1 - leight / binSize;

                            histogramNumber = xx * partsCount + yy;
                            resultDescriptor.addHistValue(histogramNumber, binNumber, currentLevel * weightX * weightY * weight);
                        }
                    }
                }
            }
        }
    }
    resultDescriptor.normalize();

    return resultDescriptor;
}


std::vector<IntrestingPoint> DescriptorConstructor::orientPoints(std::vector<IntrestingPoint> inputPoints) {
    std::vector<IntrestingPoint> orientPoints;

    for(int index = 0; index < inputPoints.size(); index++) {
        const int localBinCount = 36;
        double localBinSize = 360.0 / localBinCount;
        int radius = 8;
        radius *= inputPoints.at(index).getSigma();

        double localBin[localBinCount];
        std::fill(std::begin (localBin), std::end (localBin), 0);

        for(int i = -radius; i < radius; i++){
            for(int j = -radius; j < radius; j++){

                //В пределах ?
                if(ImageUtils::getDistance((double)i,0.0,(double)j,0.0) < sqrt(pow(radius,2) + pow(radius,2))){

                    //Направление Фи
                    double localPfi =  directionFi.getPixel(inputPoints.at(index).getX() + i, inputPoints.at(index).getY() + j);

                    //Номер корзины
                    int binNumber = (localPfi / localBinSize + 0.5);

                    //Раскидываем по корзинам
                    double localBinCenter = (double)binNumber * localBinSize + localBinSize / 2.0;

                    int relatedBin;
                    if(localPfi < localBinCenter)
                        relatedBin = binNumber - 1;
                    else
                        relatedBin = binNumber + 1;

                    double thisCenterDistance = abs(localBinCenter - localPfi);
                    double relatedCenterDistance = localBinSize - thisCenterDistance;

                    localBin[binNumber] += level.getPixel(inputPoints.at(index).getX() + i, inputPoints.at(index).getY() + j) * (1 - thisCenterDistance / localBinSize);
                    localBin[relatedBin] += level.getPixel(inputPoints.at(index).getX() + i, inputPoints.at(index).getY() + j) * (1 - relatedCenterDistance / localBinSize);
                }
            }
        }

        double firstMaxValue = -1;
        int firstMaxValueIndex = -1;
        double secondMaxValue = -1;
        int secondMaxValueIndex = -1;

        //Нашли первую и вторую максимальную
        for(int i = 0; i < localBinCount; i++){
            if(localBin[i] > firstMaxValue){
                secondMaxValue = firstMaxValue;
                secondMaxValueIndex = firstMaxValueIndex;

                firstMaxValue = localBin[i];
                firstMaxValueIndex = i;
            } else {
                if(localBin[i] > secondMaxValue){
                    secondMaxValue = localBin[i];
                    secondMaxValueIndex = i;
                }
            }
        }

        //добавили первую
        IntrestingPoint firstPoint(inputPoints.at(index));
        firstPoint.setAngle((double)firstMaxValueIndex * localBinSize);
        orientPoints.push_back(firstPoint);

//        printf("IP MAX FIRST %d\n", firstMaxValueIndex);


        //если вторая >= 0.8 от первой, то добваляем то же
        if(secondMaxValue >= (firstMaxValue * 0.8)) {
            IntrestingPoint secondPoint(inputPoints.at(index));
            secondPoint.setAngle((double)secondMaxValueIndex * localBinSize);
            orientPoints.push_back(secondPoint);
//            printf("IP MAX SECOND %d\n", secondMaxValueIndex);
        }
    }
    return orientPoints;
}
