#include "Descriptor.h"


Descriptor::Descriptor(const IntrestingPoint &inputPoint):point(inputPoint)
{
    std::fill(std::begin (histograms), std::end (histograms), 0);
}

double Descriptor::getHistValue(int histogramNumber, int basketNumber) const {
    if(basketNumber >= binCount) basketNumber = 0;
    if(basketNumber < 0) basketNumber = binCount - 1;
    return histograms[basketNumber * histogramsCount + histogramNumber];
}

void Descriptor::addHistValue(int histogramNumber, int basketNumber, double value) {
    double curValue = getHistValue(histogramNumber, basketNumber);
    setHistValue(histogramNumber, basketNumber, value + curValue);
}

void Descriptor::setHistValue(int histogramNumber, int basketNumber, double value) {
    if(basketNumber >= binCount) basketNumber = 0;
    if(basketNumber < 0) basketNumber = binCount - 1;
    histograms[basketNumber * histogramsCount + histogramNumber] = value;
}

void Descriptor::normalize()
{
    //accumulate(begin, end)
    double size = 0;
    for (double x : histograms)  size += pow(x,2);
    size = sqrt(size);

    for(int i = 0; i < histogramsCount * binCount; i++){
        if(histograms[i] / size > 0.2) {
            histograms[i] = 0.2;
        } else {
            histograms[i] = histograms[i]/size;
        }
    }

    size = 0;
    for (double x : histograms)  size += pow(x,2);
    size = sqrt(size);

    for(int i = 0; i < histogramsCount * binCount; i++){
        histograms[i] = histograms[i] / size;
    }

}

double Descriptor::getDistance(const Descriptor &descriptor) const
{
    double result = 0;
    for(int i = 0; i < histogramsCount; i++){
        for(int j = 0; j < binCount; j++){
            result += pow((descriptor.getHistValue(i,j) - getHistValue(i,j)),2);
        }
    }
    return sqrt(result);
}

IntrestingPoint Descriptor::getIntrestingPoint() {
    return point;
}

