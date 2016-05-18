#include "ImageUtils.h"

Image ImageUtils::normalize(Image image) {
    Image resultImage(image);
    double currentBright = resultImage.getPixel(0,0);
    double maxBright = currentBright;
    double minBright = currentBright;

    for(int x = 0; x < resultImage.getWidth(); x++){
        for(int y = 0; y < resultImage.getHeight(); y++){
            currentBright = resultImage.getPixel(x,y);
            if(maxBright < currentBright) maxBright = currentBright;
            if(minBright > currentBright) minBright = currentBright;
        }
    }

    for(int x = 0; x < resultImage.getWidth(); x++){
        for(int y = 0; y < resultImage.getHeight(); y++){
            currentBright = resultImage.getPixel(x,y);
            currentBright = ((currentBright - minBright) / (maxBright - minBright));
            resultImage.setPixel(x,y,currentBright);
        }
    }
    return resultImage;
}

std::vector<IntrestingPoint> ImageUtils::ANMS(std::vector<IntrestingPoint> points, int pointsCount){
    printf("ANMS Start\n");
    int currentRadius = 1;
    do{
        for(int x = 0; x < points.size(); x++){
            for(int y = 0; y < points.size(); y++){
                //double currentDistance = sqrt(pow((points.at(x).getX() - points.at(y).getX()),2) + pow((points.at(x).getY() - points.at(y).getY()),2));
                if(x == y) continue;
                if( points.at(x).getDistance(points.at(y)) < currentRadius && points.at(x).getValue() < points.at(y).getValue() ) {
                    points.erase(points.begin() + x);
                }
            }
        }
        currentRadius++;
    }while(points.size() > pointsCount);
    printf("ANMS End\n");
    return points;
}

double ImageUtils::getDistance(double x0, double x1, double y0, double y1){
    return sqrt(pow((x1 - x0),2) + pow((y1 - y0),2));
}
