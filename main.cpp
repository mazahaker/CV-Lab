//#include <QCoreApplication>
#include <QApplication>
#include "Image.h"
#include "ImageUtils.h"
#include "DescriptorConstructor.h"

#include <QPixmap>
#include <QPainter>
#include <QTextDocument>


#include "Pyramid.h"

int checkAngle(int a, int b)
{
    return (a + b) % b;
}

//Поворачивает точку (x1,y1) вокруг (x0,y0)
std::pair<double, double> rotate(double x0, double y0, double x1, double y1, double angle)
{
    if(x0 == x1 && y0 == y1)
        return std::pair<double, double>(x0, y0);

    //находим угол
    double deltaX = x1 - x0;
    double deltaY = y0 - y1;
    double dist = hypot(deltaX, deltaY);

    double angleQQ = acos(deltaX / dist);
    if(deltaY < 0)
        angleQQ = (2 * M_PI - angleQQ) / M_PI * 180;
    else
        angleQQ = angleQQ / M_PI * 180;

    double newAngle = angleQQ - angle;
    if(newAngle < 0)
        newAngle += 360;

    double newDeltaX = cos(newAngle / 180 * M_PI) * dist;
    double newDeltaY = sin(newAngle / 180 * M_PI) * dist;

    double newX = x0 + newDeltaX;
    double newY = y0 - newDeltaY;

    return std::pair<double, double>(newX,newY);
}


void lab1() {
    Image resultImage("D:\\KazakovImages\\input\\3.jpg");
    resultImage.saveImage("D:\\KazakovImages\\output\\1.jpg", "MONO");
    resultImage.convolution(&blur[0][0], 3, 3).saveImage("D:\\KazakovImages\\output\\blur.jpg", "blur");
    resultImage.convolution(&emboss[0][0], 3, 3).saveImage("D:\\KazakovImages\\output\\emboss.jpg", "emboss");
    resultImage.convolution(&sharpness[0][0], 3, 3).saveImage("D:\\KazakovImages\\output\\sharpness.jpg", "sharpness");

    ImageUtils::normalize(resultImage.convolution(&sobelX[0][0], 3, 3)).saveImage("D:\\KazakovImages\\output\\sobelX.jpg", "sobelX");
    ImageUtils::normalize(resultImage.convolution(&sobelY[0][0], 3, 3)).saveImage("D:\\KazakovImages\\output\\sobelY.jpg", "sobelY");
    ImageUtils::normalize(resultImage.sobelGradient(resultImage.convolution(&sobelX[0][0], 3, 3), resultImage.convolution(&sobelY[0][0], 3, 3))).saveImage("D:\\KazakovImages\\output\\sobelG.jpg", "sobelG");
    printf("\nEXIT");
}

void lab2() {
    Image resultImage("D:\\KazakovImages\\input\\3.jpg");
    Pyramid pyramid(resultImage, 1.0, 5, 5);
    pyramid.pyramidSave("D:\\KazakovImages\\output\\");
}

void lab3() {
    Image resultImage("D:\\KazakovImages\\input\\3.jpg");
    resultImage.moravec(0.05);
    resultImage.saveImageWithPoints("D:\\KazakovImages\\output\\moravec.jpg", "moravec");
    resultImage.setPoints(ImageUtils::ANMS(resultImage.getPoints(), 50));
    resultImage.saveImageWithPoints("D:\\KazakovImages\\output\\moravec_ANMS_50.jpg", "moravec_ANMS_50");
    resultImage.haris(4);
    resultImage.saveImageWithPoints("D:\\KazakovImages\\output\\haris.jpg", "haris");
    resultImage.setPoints(ImageUtils::ANMS(resultImage.getPoints(), 50));
    resultImage.saveImageWithPoints("D:\\KazakovImages\\output\\haris_ANMS_50.jpg", "haris_ANMS_50");
}

void lab4() {
    Image img1("D:\\KazakovImages\\input\\q1.png");
    img1.haris(4);
    img1.setPoints(ImageUtils::ANMS(img1.getPoints(), 120));
    DescriptorConstructor constructor1(img1);
    std::vector<Descriptor> descriptors1;

    for(int i = 0; i < img1.getPoints().size(); i++){
        descriptors1.push_back(constructor1.createDescriptor(img1.getPoints().at(i)));
    }

    Image img2("D:\\KazakovImages\\input\\q2.png");
    img2.haris(4);
    img2.setPoints(ImageUtils::ANMS(img2.getPoints(), 120));
    DescriptorConstructor constructor2(img2);
    std::vector<Descriptor> descriptors2;
    for(int i = 0; i < img2.getPoints().size(); i++){
        descriptors2.push_back(constructor2.createDescriptor(img2.getPoints().at(i)));
    }

    printf("\nEXIT");


    QImage qIamqe1 = img1.getInputImage();
    QImage qIamqe2 = img2.getInputImage();
    int height = (qIamqe1.height() > qIamqe2.height()) ? qIamqe1.height() : qIamqe2.height();
    QImage image( qIamqe1.width() + qIamqe2.width(), height, img1.getInputImage().format() );

    QPainter painter;
    painter.begin(&image);
    painter.drawImage(QPoint(0,0), qIamqe1);
    painter.drawImage(QPoint(qIamqe1.width(),0), qIamqe2);

    for(int i = 0; i < img1.getPoints().size(); i++){
        painter.setPen(QColor(rand()%255, rand()%255, rand()%255, 255));
        painter.drawRect(img1.getPoints().at(i).getX() - 5, img1.getPoints().at(i).getY() - 5, 10, 10);
        painter.drawRect(img1.getPoints().at(i).getX() - 4, img1.getPoints().at(i).getY() - 4, 8, 8);
    }

    for(int i = 0; i < img2.getPoints().size(); i++){
        painter.setPen(QColor(rand()%255, rand()%255, rand()%255, 255));
        painter.drawRect(img2.getPoints().at(i).getX() - 5 + qIamqe1.width(), img2.getPoints().at(i).getY() - 5, 10, 10);
        painter.drawRect(img2.getPoints().at(i).getX() - 4 + qIamqe1.width(), img2.getPoints().at(i).getY() - 4, 8, 8);
    }

    //Поиск
    for(int i = 0; i < descriptors1.size(); i++){
        double firstMinValue = 10000;
        int firstMinValueIndex = 10000;
        double secondMinValue = 10000;
        int secondMinValueIndex = 10000;

        for(int j = 0; j < descriptors2.size(); j++){
            double dist = descriptors1.at(i).getDistance(descriptors2.at(j));
            if(dist < firstMinValue){
                secondMinValue = firstMinValue;
                secondMinValueIndex = firstMinValueIndex;

                firstMinValue = dist;
                firstMinValueIndex = j;
            } else {
                if(dist < secondMinValue){
                    secondMinValue = dist;
                    secondMinValueIndex = j;
                }
            }
        }

        if(firstMinValue / secondMinValue < 0.8){
            QPen pen(QColor(rand()%255 ,rand()%255, rand()%255));
            painter.setPen(pen);

            painter.drawRect(img1.getPoints().at(i).getX() - 5, img1.getPoints().at(i).getY() - 5, 10, 10);
            painter.drawRect(img1.getPoints().at(i).getX() - 4, img1.getPoints().at(i).getY() - 4, 8, 8);

            painter.drawRect(img2.getPoints().at(firstMinValueIndex).getX() - 5 + qIamqe1.width(), img2.getPoints().at(firstMinValueIndex).getY() - 5, 10, 10);
            painter.drawRect(img2.getPoints().at(firstMinValueIndex).getX() - 4 + qIamqe1.width(), img2.getPoints().at(firstMinValueIndex).getY() - 4, 8, 8);


            painter.drawLine(QPoint(descriptors1.at(i).getIntrestingPoint().getX(),descriptors1.at(i).getIntrestingPoint().getY()),QPoint(descriptors2.at(firstMinValueIndex).getIntrestingPoint().getX() + qIamqe1.width(),descriptors2.at(firstMinValueIndex).getIntrestingPoint().getY()));
        }
    }

    painter.end();

    image.save("D:\\result.jpg");
    printf("\nEXIT");


}

void lab5() {
    Image img1("D:\\KazakovImages\\input\\z11.jpg");
//    Image img1("D:\\KazakovImages\\input\\q1.png");
    img1.haris(4);
    img1.setPoints(ImageUtils::ANMS(img1.getPoints(), 500));
    DescriptorConstructor constructor1(img1);
    img1.setPoints(constructor1.orientPoints(img1.getPoints()));
    std::vector<Descriptor> descriptors1;

    for(int i = 0; i < img1.getPoints().size(); i++){
        descriptors1.push_back(constructor1.createDescriptor(img1.getPoints().at(i)));
    }

    Image img2("D:\\KazakovImages\\input\\z22_130.jpg");
//    Image img2("D:\\KazakovImages\\input\\q2.png");
    img2.haris(4);
    img2.setPoints(ImageUtils::ANMS(img2.getPoints(), 500));
    DescriptorConstructor constructor2(img2);
    img2.setPoints(constructor2.orientPoints(img2.getPoints()));
    std::vector<Descriptor> descriptors2;
    for(int i = 0; i < img2.getPoints().size(); i++){
        descriptors2.push_back(constructor2.createDescriptor(img2.getPoints().at(i)));
    }

    printf("Find process...\n");


    QImage qIamqe1 = img1.getInputImage();
    QImage qIamqe2 = img2.getInputImage();
    int height = (qIamqe1.height() > qIamqe2.height()) ? qIamqe1.height() : qIamqe2.height();
    QImage image( qIamqe1.width() + qIamqe2.width(), height, img1.getInputImage().format() );

    QPainter painter;
    painter.begin(&image);

    QFont font = QApplication::font();
    font.setPixelSize(10);
    font.setBold(false);
    painter.setFont( font );

    painter.drawImage(QPoint(0,0), qIamqe1);
    painter.drawImage(QPoint(qIamqe1.width(),0), qIamqe2);

    for(int i = 0; i < img1.getPoints().size(); i++){
        painter.setPen(QColor(rand()%255, rand()%255, rand()%255, 255));
        painter.drawEllipse(img1.getPoints().at(i).getX() - 8, img1.getPoints().at(i).getY() - 8, 16, 16);
        painter.drawEllipse(img1.getPoints().at(i).getX() - 7, img1.getPoints().at(i).getY() - 7, 14, 14);
        painter.drawLine(
                    img1.getPoints().at(i).getX(),
                    img1.getPoints().at(i).getY(),
                    img1.getPoints().at(i).getX() + (15 * cos(img1.getPoints().at(i).getAngle() * M_PI / 180.0)),
                    img1.getPoints().at(i).getY() + (15 * sin(img1.getPoints().at(i).getAngle() * M_PI / 180.0)) );
        //painter.drawText(img1.getPoints().at(i).getX(), img1.getPoints().at(i).getY() + 15, QString::number(img1.getPoints().at(i).getAngle()));
    }

    for(int i = 0; i < img2.getPoints().size(); i++){
        painter.setPen(QColor(rand()%255, rand()%255, rand()%255, 255));
        painter.drawEllipse(img2.getPoints().at(i).getX() - 8 + qIamqe1.width(), img2.getPoints().at(i).getY() - 8, 16, 16);
        painter.drawEllipse(img2.getPoints().at(i).getX() - 7 + qIamqe1.width(), img2.getPoints().at(i).getY() - 7, 14, 14);
        painter.drawLine(
                    img2.getPoints().at(i).getX() + qIamqe1.width(),
                    img2.getPoints().at(i).getY(),
                    img2.getPoints().at(i).getX() + (15 * cos(img2.getPoints().at(i).getAngle() * M_PI / 180.0)) + qIamqe1.width(),
                    img2.getPoints().at(i).getY() + (15 * sin(img2.getPoints().at(i).getAngle() * M_PI / 180.0)) );
        //painter.drawText(img2.getPoints().at(i).getX() + qIamqe1.width(), img2.getPoints().at(i).getY() + 15, QString::number(img2.getPoints().at(i).getAngle()));
    }

    //Поиск
    int findCount = 0;
    for(int i = 0; i < descriptors1.size(); i++){
        double firstMinValue = 10000;
        int firstMinValueIndex = 10000;
        double secondMinValue = 10000;
        int secondMinValueIndex = 10000;

        for(int j = 0; j < descriptors2.size(); j++){
            double dist = descriptors1.at(i).getDistance(descriptors2.at(j));
            if(dist < firstMinValue){
                secondMinValue = firstMinValue;
                secondMinValueIndex = firstMinValueIndex;

                firstMinValue = dist;
                firstMinValueIndex = j;
            } else {
                if(dist < secondMinValue){
                    secondMinValue = dist;
                    secondMinValueIndex = j;
                }
            }
        }

        if(firstMinValue / secondMinValue < 0.8){
            findCount++;
            QPen pen(QColor(rand()%255 ,rand()%255, rand()%255));
            painter.setPen(pen);

            painter.drawEllipse(img1.getPoints().at(i).getX() - 8, img1.getPoints().at(i).getY() - 8, 16, 16);
            painter.drawEllipse(img1.getPoints().at(i).getX() - 7, img1.getPoints().at(i).getY() - 7, 14, 14);
            painter.drawLine(
                        img1.getPoints().at(i).getX(),
                        img1.getPoints().at(i).getY(),
                        img1.getPoints().at(i).getX() + (15 * cos(img1.getPoints().at(i).getAngle() * M_PI / 180.0)),
                        img1.getPoints().at(i).getY() + (15 * sin(img1.getPoints().at(i).getAngle() * M_PI / 180.0)) );

            painter.drawEllipse(img2.getPoints().at(firstMinValueIndex).getX() - 8 + qIamqe1.width(), img2.getPoints().at(firstMinValueIndex).getY() - 8, 16, 16);
            painter.drawEllipse(img2.getPoints().at(firstMinValueIndex).getX() - 7 + qIamqe1.width(), img2.getPoints().at(firstMinValueIndex).getY() - 7, 14, 14);
            painter.drawLine(
                        img2.getPoints().at(firstMinValueIndex).getX() + qIamqe1.width(),
                        img2.getPoints().at(firstMinValueIndex).getY(),
                        img2.getPoints().at(firstMinValueIndex).getX() + (15 * cos(img2.getPoints().at(firstMinValueIndex).getAngle() * M_PI / 180.0)) + qIamqe1.width(),
                        img2.getPoints().at(firstMinValueIndex).getY() + (15 * sin(img2.getPoints().at(firstMinValueIndex).getAngle() * M_PI / 180.0)) );

            painter.drawLine(
                        QPoint(descriptors1.at(i).getIntrestingPoint().getX(),
                               descriptors1.at(i).getIntrestingPoint().getY()),
                        QPoint(descriptors2.at(firstMinValueIndex).getIntrestingPoint().getX() + qIamqe1.width(),
                               descriptors2.at(firstMinValueIndex).getIntrestingPoint().getY()));
        }
    }

    painter.end();
    double percent = (((double) findCount / ((descriptors1.size() + descriptors2.size())/2))*100);
    printf("FINDED DESCRIPTORS: %lf percent\n", percent);
    image.save("D:\\result.jpg");
    printf("EXIT\n");


}


void lab6() {
    Image img1("D:\\KazakovImages\\input\\m3.jpg");
    Pyramid firstPyramid(img1, 1.6, 10, 10);
    //firstPyramid.pyramidSave("D:\\KazakovImages\\output\\");
    firstPyramid.createBlobs();
    std::vector<Descriptor> descriptors1;
    printf("Find process...\n");
    for(int i = 0; i < firstPyramid.getOctavesCount(); i++)
    {
        for(int j = 0; j < firstPyramid.getOctave(i).getLevelsCount(); j++){
            Image img(firstPyramid.getOctave(i).getLevel(j).getImage());
            DescriptorConstructor constructor1(img);  //фабрика для уровня и октавы
            firstPyramid.getOctave(i).getLevel(j).getImage().setPoints(constructor1.orientPoints(firstPyramid.getOctave(i).getLevel(j).getImage().getPoints()));
            for(int k = 0; k < firstPyramid.getOctave(i).getLevel(j).getImage().getPoints().size(); k++){
                descriptors1.push_back(constructor1.createDescriptor(firstPyramid.getOctave(i).getLevel(j).getImage().getPoints().at(k)));
                //printf("POINT\n");
            }
        }
    }
    printf("Find process...\n");

    Image img2("D:\\KazakovImages\\input\\l9_2_2.jpg");
    Pyramid secondPyramid(img2, 1.6, 10, 10);
    secondPyramid.createBlobs();
    std::vector<Descriptor> descriptors2;

    for(int i = 0; i < secondPyramid.getOctavesCount(); i++)
    {
        for(int j = 0; j < secondPyramid.getOctave(i).getLevelsCount(); j++){
            Image img(secondPyramid.getOctave(i).getLevel(j).getImage());
            DescriptorConstructor constructor2(img);  //фабрика для уровня и октавы
            secondPyramid.getOctave(i).getLevel(j).getImage().setPoints(constructor2.orientPoints(secondPyramid.getOctave(i).getLevel(j).getImage().getPoints()));
            for(int k = 0; k < secondPyramid.getOctave(i).getLevel(j).getImage().getPoints().size(); k++){
                descriptors2.push_back(constructor2.createDescriptor(secondPyramid.getOctave(i).getLevel(j).getImage().getPoints().at(k)));
                //printf("POINT\n");
            }
        }
    }
    printf("Find process...\n");


    QImage qIamqe1 = ImageUtils::normalize(img1).getInputImage();
    QImage qIamqe2 = ImageUtils::normalize(img2).getInputImage();
    int height = (qIamqe1.height() > qIamqe2.height()) ? qIamqe1.height() : qIamqe2.height();
    QImage image( qIamqe1.width() + qIamqe2.width(), height, img1.getInputImage().format() );

    QPainter painter;
    painter.begin(&image);

    QFont font = QApplication::font();
    font.setPixelSize(10);
    font.setBold(false);
    painter.setFont( font );

    painter.drawImage(QPoint(0,0), qIamqe1);
    painter.drawImage(QPoint(qIamqe1.width(),0), qIamqe2);

    //    printf("SIZE1 %d\n\n", descriptors1.size());
    //    for(int i = 0; i < descriptors1.size(); i++){
    //        int x = descriptors1.at(i).getIntrestingPoint().getX();
    //        int y = descriptors1.at(i).getIntrestingPoint().getY();
    //        double r1 = descriptors1.at(i).getIntrestingPoint().getRadius();
    //        x *= pow(2, descriptors1.at(i).getIntrestingPoint().getOctave());
    //        y *= pow(2, descriptors1.at(i).getIntrestingPoint().getOctave());
    //        painter.setPen(QColor(rand()%255, rand()%255, rand()%255, 255));
    //        painter.drawEllipse(x - r1, y - r1, r1*2, r1*2);
    //        painter.drawEllipse(x - r1, y - r1, r1*2, r1*2);
    //        painter.drawLine(
    //                    x,
    //                    y,
    //                    x + (r1 * cos(descriptors1.at(i).getIntrestingPoint().getAngle() * M_PI / 180.0)),
    //                    y + (r1 * sin(descriptors1.at(i).getIntrestingPoint().getAngle() * M_PI / 180.0)) );
    //        //painter.drawText(img1.getPoints().at(i).getX(), img1.getPoints().at(i).getY() + 15, QString::number(img1.getPoints().at(i).getAngle()));
    //    }
    //    printf("SIZE2 %d\n\n", descriptors2.size());
    //    for(int i = 0; i < descriptors2.size(); i++){
    //        int x = descriptors2.at(i).getIntrestingPoint().getX();
    //        int y = descriptors2.at(i).getIntrestingPoint().getY();
    //        double r2 = descriptors2.at(i).getIntrestingPoint().getRadius();
    //        x *= pow(2, descriptors2.at(i).getIntrestingPoint().getOctave());
    //        x+= qIamqe1.width();
    //        y *= pow(2, descriptors2.at(i).getIntrestingPoint().getOctave());
    //        painter.setPen(QColor(rand()%255, rand()%255, rand()%255, 255));
    //        painter.drawEllipse(x - r2, y - r2, r2*2, r2*2);
    //        painter.drawEllipse(x - r2, y - r2, r2*2, r2*2);
    //        painter.drawLine(
    //                    x,
    //                    y,
    //                    x + (r2 * cos(descriptors2.at(i).getIntrestingPoint().getAngle() * M_PI / 180.0)),
    //                    y + (r2 * sin(descriptors2.at(i).getIntrestingPoint().getAngle() * M_PI / 180.0)));
    //        //painter.drawText(img2.getPoints().at(i).getX() + qIamqe1.width(), img2.getPoints().at(i).getY() + 15, QString::number(img2.getPoints().at(i).getAngle()));
    //    }

    //Поиск
    int findCount = 0;
    for(int i = 0; i < descriptors1.size(); i++){
        double firstMinValue = 10000;
        int firstMinValueIndex = 10000;
        double secondMinValue = 10000;
        int secondMinValueIndex = 10000;

        for(int j = 0; j < descriptors2.size(); j++){
            double dist = descriptors1.at(i).getDistance(descriptors2.at(j));
            if(dist < firstMinValue){
                secondMinValue = firstMinValue;
                secondMinValueIndex = firstMinValueIndex;

                firstMinValue = dist;
                firstMinValueIndex = j;
            } else {
                if(dist < secondMinValue){
                    secondMinValue = dist;
                    secondMinValueIndex = j;
                }
            }
        }

        if(firstMinValue / secondMinValue < 0.8){
            findCount++;
            QPen pen(QColor(rand()%255 ,rand()%255, rand()%255));
            painter.setPen(pen);

            int x1 = descriptors1.at(i).getIntrestingPoint().getX();
            int y1 = descriptors1.at(i).getIntrestingPoint().getY();
            double r1 = descriptors1.at(i).getIntrestingPoint().getRadius();
            //printf("R1 %lf\n", r1);
            x1 *= pow(2, descriptors1.at(i).getIntrestingPoint().getOctave());
            y1 *= pow(2, descriptors1.at(i).getIntrestingPoint().getOctave());

            int x2 = descriptors2.at(firstMinValueIndex).getIntrestingPoint().getX();
            int y2 = descriptors2.at(firstMinValueIndex).getIntrestingPoint().getY();
            double r2 = descriptors2.at(firstMinValueIndex).getIntrestingPoint().getRadius();
            //printf("R2 %lf\n", r2);
            x2 *= pow(2, descriptors2.at(firstMinValueIndex).getIntrestingPoint().getOctave());
            y2 *= pow(2, descriptors2.at(firstMinValueIndex).getIntrestingPoint().getOctave());
            x2+= qIamqe1.width();

            painter.drawEllipse(x1 - r1, y1 - r1, r1*2, r1*2);
            //painter.drawEllipse(x1 - 7, y1 - 7, 14, 14);
            painter.drawLine(
                        x1,
                        y1,
                        x1 + (15 * cos(descriptors1.at(i).getIntrestingPoint().getAngle() * M_PI / 180.0)),
                        y1 + (15 * sin(descriptors1.at(i).getIntrestingPoint().getAngle() * M_PI / 180.0))
                        );

            painter.drawEllipse(x2 - r2, y2 - r2, r2*2, r2*2);
            //painter.drawEllipse(x2 - 7, y2 - 7, 14, 14);
            painter.drawLine(
                        x2,
                        y2,
                        x2 + (15 * cos(descriptors2.at(firstMinValueIndex).getIntrestingPoint().getAngle() * M_PI / 180.0)),
                        y2 + (15 * sin(descriptors2.at(firstMinValueIndex).getIntrestingPoint().getAngle() * M_PI / 180.0))
                        );

            painter.drawLine(
                        QPoint(x1,
                               y1),
                        QPoint(x2,
                               y2)
                        );
        }
    }

    painter.end();
    image.save("D:\\result.jpg");
    printf("EXIT\n");
}

void lab9() {
    const int xBins = 50, yBins = 50, aBins = 30, sBins = 5;
    int accumulate[xBins][yBins][aBins][sBins];
    for(int a = 0; a < xBins; a++)
        for(int b = 0; b < yBins; b++)
            for(int c = 0; c < aBins; c++)
                for(int d = 0; d < sBins; d++)
                    accumulate[a][b][c][d] = 0;
    Image img1("D:\\KazakovImages\\input\\m3_2.jpg");
    Pyramid firstPyramid(img1, 1.6, 10, 10);
    firstPyramid.createBlobs();
    std::vector<Descriptor> descriptors1;
    printf("Find process...\n");
    for(int i = 0; i < firstPyramid.getOctavesCount(); i++)
    {
        for(int j = 0; j < firstPyramid.getOctave(i).getLevelsCount(); j++){
            Image img(firstPyramid.getOctave(i).getLevel(j).getImage());
            DescriptorConstructor constructor1(img);
            firstPyramid.getOctave(i).getLevel(j).getImage().setPoints(constructor1.orientPoints(firstPyramid.getOctave(i).getLevel(j).getImage().getPoints()));
            for(int k = 0; k < firstPyramid.getOctave(i).getLevel(j).getImage().getPoints().size(); k++){
                descriptors1.push_back(constructor1.createDescriptor(firstPyramid.getOctave(i).getLevel(j).getImage().getPoints().at(k)));
            }
        }
    }

    Image img2("D:\\KazakovImages\\input\\l9_2_2.jpg");
    Pyramid secondPyramid(img2, 1.6, 10, 10);
    secondPyramid.createBlobs();
    std::vector<Descriptor> descriptors2;

    for(int i = 0; i < secondPyramid.getOctavesCount(); i++)
    {
        for(int j = 0; j < secondPyramid.getOctave(i).getLevelsCount(); j++){
            Image img(secondPyramid.getOctave(i).getLevel(j).getImage());
            DescriptorConstructor constructor2(img);
            secondPyramid.getOctave(i).getLevel(j).getImage().setPoints(constructor2.orientPoints(secondPyramid.getOctave(i).getLevel(j).getImage().getPoints()));
            for(int k = 0; k < secondPyramid.getOctave(i).getLevel(j).getImage().getPoints().size(); k++){
                descriptors2.push_back(constructor2.createDescriptor(secondPyramid.getOctave(i).getLevel(j).getImage().getPoints().at(k)));
            }
        }
    }
    printf("Find process...\n");


    QImage qIamqe1 = ImageUtils::normalize(img1).getInputImage();
    QImage qIamqe2 = ImageUtils::normalize(img2).getInputImage();
    int height = (qIamqe1.height() > qIamqe2.height()) ? qIamqe1.height() : qIamqe2.height();
    QImage image( qIamqe1.width() + qIamqe2.width(), height, img1.getInputImage().format() );

    QPainter painter;
    painter.begin(&image);

    painter.drawImage(QPoint(0,0), qIamqe1);
    painter.drawImage(QPoint(qIamqe1.width(),0), qIamqe2);

    std::vector<std::pair<IntrestingPoint, IntrestingPoint>> pairs;

    //Поиск
    int findCount = 0;
    for(int i = 0; i < descriptors1.size(); i++){
        double firstMinValue = 10000;
        int firstMinValueIndex = 10000;
        double secondMinValue = 10000;
        int secondMinValueIndex = 10000;

        for(int j = 0; j < descriptors2.size(); j++){
            double dist = descriptors1.at(i).getDistance(descriptors2.at(j));
            if(dist < firstMinValue){
                secondMinValue = firstMinValue;
                secondMinValueIndex = firstMinValueIndex;

                firstMinValue = dist;
                firstMinValueIndex = j;
            } else {
                if(dist < secondMinValue){
                    secondMinValue = dist;
                    secondMinValueIndex = j;
                }
            }
        }

        if(firstMinValue / secondMinValue < 0.8){
            std::pair<IntrestingPoint, IntrestingPoint> pair;
            pair.first = descriptors1.at(i).getIntrestingPoint();
            pair.second = descriptors2.at(firstMinValueIndex).getIntrestingPoint();
            pairs.push_back(pair);
        }
    }

    for(int i = 0; i < pairs.size(); i++) {
        double angle, coef;
        int x1,x2,x3,x4,y1,y2,y3,y4;
        angle = pairs.at(i).first.getAngle() - pairs.at(i).second.getAngle();
        coef = pairs.at(i).first.getRadius()/ pairs.at(i).second.getRadius();

        //Лево верх
        x1 = (pairs.at(i).first.getX() - pairs.at(i).second.getX()) * coef;
        y1 = (pairs.at(i).first.getY() - pairs.at(i).second.getY()) * coef;
        std::pair<double, double> var =rotate(pairs.at(i).first.getX(),pairs.at(i).first.getY(),x1,y1,angle);
        x1 = var.first;
        y1 = var.second;

        int xBin = floor(1.0*x1/qIamqe1.width() * xBins + 0.5);
        int yBin = floor(1.0*y1 / qIamqe1.height() * yBins + 0.5);
        int aBin = floor(angle / 360 * aBins + 0.5);
        int sBin = floor(log2(coef) / log2(1.25) + 4.5);


        for(int xx = xBin - 1; xx <= xBin; xx++ )
        {
            if(xx >= xBins || xx < 0)
                continue;
            for(int yy = yBin - 1; yy <= yBin; yy++ )
            {
                if(yy >= yBins || yy < 0)
                    continue;
                for(int aa = aBin - 1; aa <= aBin; aa++ )
                {
                    int ang = checkAngle(aa, aBins);
                    for(int ss = sBin - 1; ss <= sBin && ss < sBins && ss >= 0; ss++ )
                    {
                        if(ss >= sBins || ss < 0)
                            continue;

                        accumulate[xx][yy][ang][ss]++;
                    }
                }
            }
        }


        //Право верх
        x2 = (pairs.at(i).first.getX() + (qIamqe2.width() - pairs.at(i).second.getX())) * coef;
        y2 = (pairs.at(i).first.getY() - pairs.at(i).second.getY()) * coef;
        var =rotate(pairs.at(i).first.getX(),pairs.at(i).first.getY(),x2,y2,angle);
        x2 = var.first;
        y2 = var.second;

        //Лево низ
        x3 = (pairs.at(i).first.getX() - pairs.at(i).second.getX()) * coef;
        y3 = (pairs.at(i).first.getY() - pairs.at(i).second.getY() + qIamqe2.height()) * coef;
        var =rotate(pairs.at(i).first.getX(),pairs.at(i).first.getY(),x3,y3,angle);
        x3 = var.first;
        y3 = var.second;


        //Право низ
        x4 = (pairs.at(i).first.getX() + (qIamqe2.width() - pairs.at(i).second.getX())) * coef;
        y4 = (pairs.at(i).first.getY() - pairs.at(i).second.getY() + qIamqe2.height()) * coef;
        var =rotate(pairs.at(i).first.getX(),pairs.at(i).first.getY(),x4,y4,angle);
        x4 = var.first;
        y4 = var.second;

        painter.drawLine(QPoint(x1, y1), QPoint(x2,y2));
        painter.drawLine(QPoint(x1, y1), QPoint(x3,y3));
        painter.drawLine(QPoint(x4, y4), QPoint(x2,y2));
        painter.drawLine(QPoint(x4, y4), QPoint(x3,y3));
    }
    int maxa = 0, maxb = 0, maxc = 0, maxd = 0, maxvalue = 0;
    int index = 0, oldIndex = 0;
    for(int a = 0; a < xBins; a++)
        for(int b = 0; b < yBins; b++)
            for(int c = 0; c < aBins; c++)
                for(int d = 0; d < sBins; d++)
                    if(maxvalue < accumulate[a][b][c][d]) {
                        //                        double oldoldX = (maxa2 * (qIamqe1.width() / xBins) + qIamqe2.width());
                        //                        double oldoldY = (maxb2 * (qIamqe2.height() / yBins) + qIamqe2.height());
                        //                        double oldNewX = (maxa * (qIamqe1.width() / xBins));
                        //                        double oldNewY = (maxb * (qIamqe1.height() / yBins));
                        //                        if(oldNewX > oldoldX && oldNewY > oldoldY ) {
                        //                            oldIndex = index;
                        //                            maxa2 = maxa;
                        //                            maxb2 = maxb;
                        //                            maxc2 = maxc;
                        //                            maxd2 = maxd;
                        //                            maxvalue2 = maxvalue;
                        //                        }
                        index ++;
                        maxa = a;
                        maxb = b;
                        maxc = c;
                        maxd = d;

                        maxvalue = accumulate[a][b][c][d];
                    }
    std::pair<double, double> var;
    //для 1ой
    int x1,x2,x3,x4,y1,y2,y3,y4;
    double angle, coef;

    x1 = maxa * (qIamqe1.width() / xBins);
    y1 = maxb * (qIamqe1.height() / yBins);
    angle = maxc * (360 / aBins);
    coef =  pow(1.25, maxd - 3.5);

    printf("x1 - %d\nx2 - %d\na - %lf\ns - %lf\nmaxa - %d\n", x1,y1,angle,coef,maxa);

    //Право верх
    var =rotate(0,0,qIamqe2.width()*coef,0,angle);
    x2 = x1 + var.first;
    y2 = y1 + var.second;

    printf("x2 - %d\ny2 - %d\n", x2,y2);

    //Лево низ
    var =rotate(0,0,0,qIamqe2.height()*coef,angle);
    x3 = x1 + var.first;
    y3 = y1 + var.second;

    printf("x3 - %d\ny3 - %d\n", x3,y3);


    //Право низ
    var =rotate(0,0,qIamqe2.width()*coef,qIamqe2.height()*coef,angle);
    x4 = x1 + var.first;
    y4 = y1 + var.second;

    printf("x4 - %d\ny4 - %d\n", x4,y4);

    QPen pen(QColor(rand()%255 ,rand()%255, rand()%255));
    painter.setPen(pen);

    painter.drawLine(QPoint(x1+3, y1), QPoint(x1-3, y1));
    painter.drawLine(QPoint(x1, y1+3), QPoint(x1, y1-3));


    painter.drawLine(QPoint(x1, y1), QPoint(x2,y2));
    painter.drawLine(QPoint(x1, y1), QPoint(x3,y3));
    painter.drawLine(QPoint(x4, y4), QPoint(x2,y2));
    painter.drawLine(QPoint(x4, y4), QPoint(x3,y3));
    //

    //для 2ой
    int maxa2 = 0, maxb2 = 0, maxc2 = 0, maxd2 = 0, maxvalue2 = 0;
    for(int a = 0; a < xBins; a++)
        for(int b = 0; b < yBins; b++)
            for(int c = 0; c < aBins; c++)
                for(int d = 0; d < sBins; d++)
                    if(maxvalue2 < accumulate[a][b][c][d] && accumulate[a][b][c][d] < maxvalue) {
                        double oldNewX = (a * (qIamqe1.width() / xBins));
                        double oldNewY = (b * (qIamqe1.height() / yBins));

                        if(ImageUtils::getDistance(1.0*x1, oldNewX, 1.0*y1, oldNewY) > (qIamqe2.width() + qIamqe2.height()/3)) {
                            //                            oldIndex = index;
                            //                            maxa2 = maxa;
                            //                            maxb2 = maxb;
                            //                            maxc2 = maxc;
                            //                            maxd2 = maxd;
                            //                            maxvalue2 = maxvalue;
                            //                        }
                            //printf("!TWO! %lf   %lf   %lf   %lf", );
                            maxa2 = a;
                            maxb2 = b;
                            maxc2 = c;
                            maxd2 = d;

                            maxvalue2 = accumulate[a][b][c][d];
                        }
                    }
    double temp1 = x1;
    double temp2 = y1;

    x1 = maxa2 * (qIamqe1.width() / xBins);
    y1 = maxb2 * (qIamqe1.height() / yBins);
    angle = maxc2 * (360 / aBins);
    coef =  pow(1.25, maxd2 - 3.5);


    for(int a = 0; a < xBins; a++)
        for(int b = 0; b < yBins; b++)
            for(int c = 0; c < aBins; c++)
                for(int d = 0; d < sBins; d++)
                    if(maxvalue2 < accumulate[a][b][c][d] && accumulate[a][b][c][d] < maxvalue) {
                        double oldNewX = (a * (qIamqe1.width() / xBins));
                        double oldNewY = (b * (qIamqe1.height() / yBins));

                        if(ImageUtils::getDistance(1.0*temp1, oldNewX, 1.0*temp2, oldNewY) > (qIamqe2.width() + qIamqe2.height()/3)) {
                            //                            oldIndex = index;
                            //                            maxa2 = maxa;
                            //                            maxb2 = maxb;
                            //                            maxc2 = maxc;
                            //                            maxd2 = maxd;
                            //                            maxvalue2 = maxvalue;
                            //                        }
                            //printf("!TWO! %lf   %lf   %lf   %lf", );
                            maxa2 = a;
                            maxb2 = b;
                            maxc2 = c;
                            maxd2 = d;

                            maxvalue2 = accumulate[a][b][c][d];
                        }
                    }

    x1 = maxa2 * (qIamqe1.width() / xBins);
    y1 = maxb2 * (qIamqe1.height() / yBins);
    angle = maxc2 * (360 / aBins);
    coef =  pow(1.25, maxd2 - 3.5);



    printf("x1 - %d\nx2 - %d\na - %lf\ns - %lf\nmaxa - %d\n", x1,y1,angle,coef,maxa);

    //Право верх
    var =rotate(0,0,qIamqe2.width()*coef,0,angle);
    x2 = x1 + var.first;
    y2 = y1 + var.second;

    printf("x2 - %d\ny2 - %d\n", x2,y2);

    //Лево низ
    var =rotate(0,0,0,qIamqe2.height()*coef,angle);
    x3 = x1 + var.first;
    y3 = y1 + var.second;

    printf("x3 - %d\ny3 - %d\n", x3,y3);


    //Право низ
    var =rotate(0,0,qIamqe2.width()*coef,qIamqe2.height()*coef,angle);
    x4 = x1 + var.first;
    y4 = y1 + var.second;

    printf("x4 - %d\ny4 - %d\n", x4,y4);

    QPen pen1(QColor(rand()%255 ,rand()%255, rand()%255));
    painter.setPen(pen1);

    painter.drawLine(QPoint(x1+3, y1), QPoint(x1-3, y1));
    painter.drawLine(QPoint(x1, y1+3), QPoint(x1, y1-3));


    painter.drawLine(QPoint(x1, y1), QPoint(x2,y2));
    painter.drawLine(QPoint(x1, y1), QPoint(x3,y3));
    painter.drawLine(QPoint(x4, y4), QPoint(x2,y2));
    painter.drawLine(QPoint(x4, y4), QPoint(x3,y3));

    //
    painter.end();
    image.save("D:\\result1.jpg");
    printf("EXIT\n");


    //Усреднение, пересчет для максимальных
    std::vector<std::pair<IntrestingPoint, IntrestingPoint>> inliers;
    for(int i = 0; i < pairs.size(); i++) {
        double angle, coef;
        int x1,x2,x3,x4,y1,y2,y3,y4;
        angle = pairs.at(i).first.getAngle() - pairs.at(i).second.getAngle();
        coef = pairs.at(i).first.getRadius()/ pairs.at(i).second.getRadius();

        //Лево верх
        x1 = (pairs.at(i).first.getX() - pairs.at(i).second.getX()) * coef;
        y1 = (pairs.at(i).first.getY() - pairs.at(i).second.getY()) * coef;
        std::pair<double, double> var =rotate(pairs.at(i).first.getX(),pairs.at(i).first.getY(),x1,y1,angle);
        x1 = var.first;
        y1 = var.second;

        int xBin = floor(1.0*x1/qIamqe1.width() * xBins + 0.5);
        int yBin = floor(1.0*y1 / qIamqe1.height() * yBins + 0.5);
        int aBin = floor(angle / 360 * aBins + 0.5);
        int sBin = floor(log2(coef) / log2(1.25) + 4.5);


        for(int xx = xBin - 1; xx <= xBin; xx++ )
        {
            if(xx >= xBins || xx < 0)
                continue;
            for(int yy = yBin - 1; yy <= yBin; yy++ )
            {
                if(yy >= yBins || yy < 0)
                    continue;
                for(int aa = aBin - 1; aa <= aBin; aa++ )
                {
                    int ang = checkAngle(aa, aBins);
                    for(int ss = sBin - 1; ss <= sBin && ss < sBins && ss >= 0; ss++ )
                    {
                        if(ss >= sBins || ss < 0)
                            continue;
                        if(xx == maxa && yy == maxb && ang == maxc && ss == maxd) {
                            inliers.push_back(pairs.at(i));
                        }
                    }
                }
            }
        }
    }

    double sumX = 0, sumY = 0, sumA = 0, sumS = 0;
    printf("SIZE %d\n\n", inliers.size());
    for(int i = 0; i < inliers.size(); i ++)
    {
        double coef, angle, x1, y1;
        angle = inliers.at(i).first.getAngle() - inliers.at(i).second.getAngle();
        angle = checkAngle(angle, 360);
        coef = inliers.at(i).first.getRadius()/ inliers.at(i).second.getRadius();

        //Лево верх
        x1 = (inliers.at(i).first.getX() - inliers.at(i).second.getX()) * coef;
        y1 = (inliers.at(i).first.getY() - inliers.at(i).second.getY()) * coef;
        std::pair<double, double> var =rotate(inliers.at(i).first.getX(),inliers.at(i).first.getY(),x1,y1,angle);
        x1 = var.first;
        y1 = var.second;

        sumX += x1;
        sumY += y1;
        sumA += angle;
        sumS += coef;
    }

    double avgX = sumX / inliers.size();
    double avgY = sumY / inliers.size();
    double avgA = sumA / inliers.size();
    double avgS = sumS / inliers.size();

    printf("X - %lf\nY - %lf\nA - %lf\nS - %lf\n", avgX,avgY,avgA,avgS);


    //Право верх
    var =rotate(0,0,qIamqe2.width()*avgS,0,avgA);
    x2 = avgX + var.first;
    y2 = avgY + var.second;

    printf("x2 - %d\ny2 - %d\n", x2,y2);

    //Лево низ
    var =rotate(0,0,0,qIamqe2.height()*avgS,avgA);
    x3 = avgX + var.first;
    y3 = avgY + var.second;

    printf("x3 - %d\ny3 - %d\n", x3,y3);


    //Право низ
    var =rotate(0,0,qIamqe2.width()*avgS,qIamqe2.height()*avgS,avgA);
    x4 = avgX + var.first;
    y4 = avgY + var.second;

    printf("x4 - %d\ny4 - %d\n", x4,y4);


    int height2 = (qIamqe1.height() > qIamqe2.height()) ? qIamqe1.height() : qIamqe2.height();
    QImage image2( qIamqe1.width() + qIamqe2.width(), height2, img1.getInputImage().format() );

    QPainter painter2;
    painter2.begin(&image2);

    painter2.drawImage(QPoint(0,0), qIamqe1);
    painter2.drawImage(QPoint(qIamqe1.width(),0), qIamqe2);

    painter2.drawLine(QPoint(avgX+3, avgY), QPoint(avgX-3, avgY));
    painter2.drawLine(QPoint(avgX, avgY+3), QPoint(avgX, avgY-3));


    painter2.drawLine(QPoint(avgX, avgY), QPoint(x2,y2));
    painter2.drawLine(QPoint(avgX, avgY), QPoint(x3,y3));
    painter2.drawLine(QPoint(x4, y4), QPoint(x2,y2));
    painter2.drawLine(QPoint(x4, y4), QPoint(x3,y3));


    //для 2го!!!!!!!!!!!
    inliers.clear();
    for(int i = 0; i < pairs.size(); i++) {
        double angle, coef;
        int x1,x2,x3,x4,y1,y2,y3,y4;
        angle = pairs.at(i).first.getAngle() - pairs.at(i).second.getAngle();
        coef = pairs.at(i).first.getRadius()/ pairs.at(i).second.getRadius();

        //Лево верх
        x1 = (pairs.at(i).first.getX() - pairs.at(i).second.getX()) * coef;
        y1 = (pairs.at(i).first.getY() - pairs.at(i).second.getY()) * coef;
        std::pair<double, double> var =rotate(pairs.at(i).first.getX(),pairs.at(i).first.getY(),x1,y1,angle);
        x1 = var.first;
        y1 = var.second;

        int xBin = floor(1.0*x1  / qIamqe1.width() * xBins + 0.5);
        int yBin = floor(1.0*y1 / qIamqe1.height() * yBins + 0.5);
        int aBin = floor(angle / 360 * aBins + 0.5);
        int sBin = floor(log2(coef) / log2(1.25) + 4.5);


        for(int xx = xBin - 1; xx <= xBin; xx++ )
        {
            if(xx >= xBins || xx < 0)
                continue;
            for(int yy = yBin - 1; yy <= yBin; yy++ )
            {
                if(yy >= yBins || yy < 0)
                    continue;
                for(int aa = aBin - 1; aa <= aBin; aa++ )
                {
                    int ang = checkAngle(aa, aBins);
                    for(int ss = sBin - 1; ss <= sBin && ss < sBins && ss >= 0; ss++ )
                    {
                        if(ss >= sBins || ss < 0)
                            continue;
                        if(xx == maxa2 && yy == maxb2 && ang == maxc2 && ss == maxd2) {
                            inliers.push_back(pairs.at(i));
                        }
                    }
                }
            }
        }
    }

    sumX = 0; sumY = 0; sumA = 0; sumS = 0;
    printf("SIZE %d\n\n", inliers.size());
    for(int i = 0; i < inliers.size(); i ++)
    {
        double coef, angle, x1, y1;
        angle = inliers.at(i).first.getAngle() - inliers.at(i).second.getAngle();
        angle = checkAngle(angle, 360);
        coef = inliers.at(i).first.getRadius()/ inliers.at(i).second.getRadius();

        //Лево верх
        x1 = (inliers.at(i).first.getX() - inliers.at(i).second.getX()) * coef;
        y1 = (inliers.at(i).first.getY() - inliers.at(i).second.getY()) * coef;
        std::pair<double, double> var =rotate(inliers.at(i).first.getX(),inliers.at(i).first.getY(),x1,y1,angle);
        x1 = var.first;
        y1 = var.second;

        sumX += x1;
        sumY += y1;
        sumA += angle;
        sumS += coef;
    }

    avgX = sumX / inliers.size();
    avgY = sumY / inliers.size();
    avgA = sumA / inliers.size();
    avgS = sumS / inliers.size();

    printf("X - %lf\nY - %lf\nA - %lf\nS - %lf\n", avgX,avgY,avgA,avgS);


    //Право верх
    var =rotate(0,0,qIamqe2.width()*avgS,0,avgA);
    x2 = avgX + var.first;
    y2 = avgY + var.second;

    printf("x2 - %d\ny2 - %d\n", x2,y2);

    //Лево низ
    var =rotate(0,0,0,qIamqe2.height()*avgS,avgA);
    x3 = avgX + var.first;
    y3 = avgY + var.second;

    printf("x3 - %d\ny3 - %d\n", x3,y3);


    //Право низ
    var =rotate(0,0,qIamqe2.width()*avgS,qIamqe2.height()*avgS,avgA);
    x4 = avgX + var.first;
    y4 = avgY + var.second;

    printf("x4 - %d\ny4 - %d\n", x4,y4);

    painter2.drawLine(QPoint(avgX+3, avgY), QPoint(avgX-3, avgY));
    painter2.drawLine(QPoint(avgX, avgY+3), QPoint(avgX, avgY-3));


    painter2.drawLine(QPoint(avgX, avgY), QPoint(x2,y2));
    painter2.drawLine(QPoint(avgX, avgY), QPoint(x3,y3));
    painter2.drawLine(QPoint(x4, y4), QPoint(x2,y2));
    painter2.drawLine(QPoint(x4, y4), QPoint(x3,y3));

    ////////
    painter2.end();
    image2.save("D:\\result2.jpg");
    printf("EXIT\n");

}


void findImage(Image img1, Image img2, std::vector<std::pair<IntrestingPoint, IntrestingPoint>> pairs, QString message) {
    printf("\n%s\n", message.toLocal8Bit().constData());
    const int xBins = 50, yBins = 50, aBins = 30, sBins = 5;
    int accumulate[xBins][yBins][aBins][sBins];
    for(int a = 0; a < xBins; a++)
        for(int b = 0; b < yBins; b++)
            for(int c = 0; c < aBins; c++)
                for(int d = 0; d < sBins; d++)
                    accumulate[a][b][c][d] = 0;

    QImage qIamqe1 = ImageUtils::normalize(img1).getInputImage();
    QImage qIamqe2 = ImageUtils::normalize(img2).getInputImage();
    int height = (qIamqe1.height() > qIamqe2.height()) ? qIamqe1.height() : qIamqe2.height();

    ////ПОИСК ИЗОБР В ИЗОБР
        for(int i = 0; i < pairs.size(); i++) {
            double angle, coef;
            int x1,y1;
            angle = pairs.at(i).first.getAngle() - pairs.at(i).second.getAngle();
            coef = pairs.at(i).first.getRadius()/ pairs.at(i).second.getRadius();

            //Лево верх
            x1 = (pairs.at(i).first.getX() - pairs.at(i).second.getX()) * coef;
            y1 = (pairs.at(i).first.getY() - pairs.at(i).second.getY()) * coef;
            std::pair<double, double> var =rotate(pairs.at(i).first.getX(),pairs.at(i).first.getY(),x1,y1,angle);
            x1 = var.first;
            y1 = var.second;

            int xBin = floor(1.0*x1/qIamqe1.width() * xBins + 0.5);
            int yBin = floor(1.0*y1 / qIamqe1.height() * yBins + 0.5);
            int aBin = floor(angle / 360 * aBins + 0.5);
            int sBin = floor(log2(coef) / log2(1.25) + 4.5);


            for(int xx = xBin - 1; xx <= xBin; xx++ )
            {
                if(xx >= xBins || xx < 0)
                    continue;
                for(int yy = yBin - 1; yy <= yBin; yy++ )
                {
                    if(yy >= yBins || yy < 0)
                        continue;
                    for(int aa = aBin - 1; aa <= aBin; aa++ )
                    {
                        int ang = checkAngle(aa, aBins);
                        for(int ss = sBin - 1; ss <= sBin && ss < sBins && ss >= 0; ss++ )
                        {
                            if(ss >= sBins || ss < 0)
                                continue;

                            accumulate[xx][yy][ang][ss]++;
                        }
                    }
                }
            }
        }
        int maxa = 0, maxb = 0, maxc = 0, maxd = 0, maxvalue = 0;
        for(int a = 0; a < xBins; a++)
            for(int b = 0; b < yBins; b++)
                for(int c = 0; c < aBins; c++)
                    for(int d = 0; d < sBins; d++)
                        if(maxvalue < accumulate[a][b][c][d]) {
                            maxa = a;
                            maxb = b;
                            maxc = c;
                            maxd = d;

                            maxvalue = accumulate[a][b][c][d];
                        }
        std::pair<double, double> var;
        int x1,x2,x3,x4,y1,y2,y3,y4;



        //Усреднение, пересчет для максимальных
        std::vector<std::pair<IntrestingPoint, IntrestingPoint>> inliers;
        for(int i = 0; i < pairs.size(); i++) {
            double angle, coef;
            int x1,x2,x3,x4,y1,y2,y3,y4;
            angle = pairs.at(i).first.getAngle() - pairs.at(i).second.getAngle();
            coef = pairs.at(i).first.getRadius()/ pairs.at(i).second.getRadius();

            //Лево верх
            x1 = (pairs.at(i).first.getX() - pairs.at(i).second.getX()) * coef;
            y1 = (pairs.at(i).first.getY() - pairs.at(i).second.getY()) * coef;
            std::pair<double, double> var =rotate(pairs.at(i).first.getX(),pairs.at(i).first.getY(),x1,y1,angle);
            x1 = var.first;
            y1 = var.second;

            int xBin = floor(1.0*x1/qIamqe1.width() * xBins + 0.5);
            int yBin = floor(1.0*y1 / qIamqe1.height() * yBins + 0.5);
            int aBin = floor(angle / 360 * aBins + 0.5);
            int sBin = floor(log2(coef) / log2(1.25) + 4.5);


            for(int xx = xBin - 1; xx <= xBin; xx++ )
            {
                if(xx >= xBins || xx < 0)
                    continue;
                for(int yy = yBin - 1; yy <= yBin; yy++ )
                {
                    if(yy >= yBins || yy < 0)
                        continue;
                    for(int aa = aBin - 1; aa <= aBin; aa++ )
                    {
                        int ang = checkAngle(aa, aBins);
                        for(int ss = sBin - 1; ss <= sBin && ss < sBins && ss >= 0; ss++ )
                        {
                            if(ss >= sBins || ss < 0)
                                continue;
                            if(xx == maxa && yy == maxb && ang == maxc && ss == maxd) {
                                inliers.push_back(pairs.at(i));
                            }
                        }
                    }
                }
            }
        }

        double sumX = 0, sumY = 0, sumA = 0, sumS = 0;
        //printf("SIZE %d\n\n", inliers.size());
        if(inliers.size() > 5) {
            printf("IMAGE FOUND!\n\n");
            for(int i = 0; i < inliers.size(); i ++)
            {
                double coef, angle, x1, y1;
                angle = inliers.at(i).first.getAngle() - inliers.at(i).second.getAngle();
                angle = checkAngle(angle, 360);
                coef = inliers.at(i).first.getRadius()/ inliers.at(i).second.getRadius();

                //Лево верх
                x1 = (inliers.at(i).first.getX() - inliers.at(i).second.getX()) * coef;
                y1 = (inliers.at(i).first.getY() - inliers.at(i).second.getY()) * coef;
                std::pair<double, double> var =rotate(inliers.at(i).first.getX(),inliers.at(i).first.getY(),x1,y1,angle);
                x1 = var.first;
                y1 = var.second;

                sumX += x1;
                sumY += y1;
                sumA += angle;
                sumS += coef;
            }

            double avgX = sumX / inliers.size();
            double avgY = sumY / inliers.size();
            double avgA = sumA / inliers.size();
            double avgS = sumS / inliers.size();

            //printf("X - %lf\nY - %lf\nA - %lf\nS - %lf\n", avgX,avgY,avgA,avgS);


            //Право верх
            var =rotate(0,0,qIamqe2.width()*avgS,0,avgA);
            x2 = avgX + var.first;
            y2 = avgY + var.second;

            //printf("x2 - %d\ny2 - %d\n", x2,y2);

            //Лево низ
            var =rotate(0,0,0,qIamqe2.height()*avgS,avgA);
            x3 = avgX + var.first;
            y3 = avgY + var.second;

           // printf("x3 - %d\ny3 - %d\n", x3,y3);


            //Право низ
            var =rotate(0,0,qIamqe2.width()*avgS,qIamqe2.height()*avgS,avgA);
            x4 = avgX + var.first;
            y4 = avgY + var.second;

           // printf("x4 - %d\ny4 - %d\n", x4,y4);


            QImage resultImage( qIamqe1.width() + qIamqe2.width(), height, img1.getInputImage().format() );

            QPainter painter2;
            painter2.begin(&resultImage);

            painter2.drawImage(QPoint(0,0), qIamqe1);
            painter2.drawImage(QPoint(qIamqe1.width(),0), qIamqe2);

            painter2.drawLine(QPoint(avgX+3, avgY), QPoint(avgX-3, avgY));
            painter2.drawLine(QPoint(avgX, avgY+3), QPoint(avgX, avgY-3));


            painter2.drawLine(QPoint(avgX, avgY), QPoint(x2,y2));
            painter2.drawLine(QPoint(avgX, avgY), QPoint(x3,y3));
            painter2.drawLine(QPoint(x4, y4), QPoint(x2,y2));
            painter2.drawLine(QPoint(x4, y4), QPoint(x3,y3));

            painter2.end();
            QString path = "C:\\kurs\\output\\image "+ message +".jpg";
            resultImage.save(path);
        } else {
            printf("IMAGE NOT FOUND!\n\n");
        }
}

const std::vector<std::pair<IntrestingPoint, IntrestingPoint>> createPairs(std::vector<Descriptor> &descriptors1, std::vector<Descriptor> &descriptors2) {
    std::vector<std::pair<IntrestingPoint, IntrestingPoint>> pairs;

    //Поиск
    for(int i = 0; i < descriptors1.size(); i++){
        double firstMinValue = 10000;
        int firstMinValueIndex = 10000;
        double secondMinValue = 10000;
        int secondMinValueIndex = 10000;

        for(int j = 0; j < descriptors2.size(); j++){
            double dist = descriptors1.at(i).getDistance(descriptors2.at(j));
            if(dist < firstMinValue){
                secondMinValue = firstMinValue;
                secondMinValueIndex = firstMinValueIndex;

                firstMinValue = dist;
                firstMinValueIndex = j;
            } else {
                if(dist < secondMinValue){
                    secondMinValue = dist;
                    secondMinValueIndex = j;
                }
            }
        }

        if(firstMinValue / secondMinValue < 0.8){
            std::pair<IntrestingPoint, IntrestingPoint> pair;
            pair.first = descriptors1.at(i).getIntrestingPoint();
            pair.second = descriptors2.at(firstMinValueIndex).getIntrestingPoint();
            pairs.push_back(pair);
        }
    }
    return pairs;
}

void kurs() {
    printf("Create pyramid, blobs for img1.jpg...\n");
    const Image img1("C:\\kurs\\input\\img1.jpg");
    Pyramid firstPyramid(img1, 1.6, 10, 10);
    firstPyramid.createBlobs();
    std::vector<Descriptor> descriptors1;
    for(int i = 0; i < firstPyramid.getOctavesCount(); i++)
    {
        for(int j = 0; j < firstPyramid.getOctave(i).getLevelsCount(); j++){
            Image img(firstPyramid.getOctave(i).getLevel(j).getImage());
            DescriptorConstructor constructor1(img);
            firstPyramid.getOctave(i).getLevel(j).getImage().setPoints(constructor1.orientPoints(firstPyramid.getOctave(i).getLevel(j).getImage().getPoints()));
            for(int k = 0; k < firstPyramid.getOctave(i).getLevel(j).getImage().getPoints().size(); k++){
                descriptors1.push_back(constructor1.createDescriptor(firstPyramid.getOctave(i).getLevel(j).getImage().getPoints().at(k)));
            }
        }
    }
    printf("Create pyramid, blobs for img2.jpg...\n");
    const Image img2("C:\\kurs\\input\\img2.jpg");
    Pyramid pyramid2(img2, 1.6, 10, 10);
    pyramid2.createBlobs();
    std::vector<Descriptor> descriptors2;
    for(int i = 0; i < pyramid2.getOctavesCount(); i++)
    {
        for(int j = 0; j < pyramid2.getOctave(i).getLevelsCount(); j++){
            Image img(pyramid2.getOctave(i).getLevel(j).getImage());
            DescriptorConstructor constructor1(img);
            pyramid2.getOctave(i).getLevel(j).getImage().setPoints(constructor1.orientPoints(pyramid2.getOctave(i).getLevel(j).getImage().getPoints()));
            for(int k = 0; k < pyramid2.getOctave(i).getLevel(j).getImage().getPoints().size(); k++){
                descriptors2.push_back(constructor1.createDescriptor(pyramid2.getOctave(i).getLevel(j).getImage().getPoints().at(k)));
            }
        }
    }
    printf("Create pyramid, blobs for img3.jpg...\n");
    Image img3("C:\\kurs\\input\\img3.jpg");
    Pyramid pyramid3(img3, 1.6, 10, 10);
    pyramid3.createBlobs();
    std::vector<Descriptor> descriptors3;
    for(int i = 0; i < pyramid3.getOctavesCount(); i++)
    {
        for(int j = 0; j < pyramid3.getOctave(i).getLevelsCount(); j++){
            Image img(pyramid3.getOctave(i).getLevel(j).getImage());
            DescriptorConstructor constructor1(img);
            pyramid3.getOctave(i).getLevel(j).getImage().setPoints(constructor1.orientPoints(pyramid3.getOctave(i).getLevel(j).getImage().getPoints()));
            for(int k = 0; k < pyramid3.getOctave(i).getLevel(j).getImage().getPoints().size(); k++){
                descriptors3.push_back(constructor1.createDescriptor(pyramid3.getOctave(i).getLevel(j).getImage().getPoints().at(k)));
            }
        }
    }


    printf("Create pyramid, blobs for searchImg.jpg...\n");
    const Image foundImage("C:\\kurs\\input\\searchImg.jpg");
    Pyramid foundPyramid(foundImage, 1.6, 10, 10);
    foundPyramid.createBlobs();
    std::vector<Descriptor> foundDescriptors;

    for(int i = 0; i < foundPyramid.getOctavesCount(); i++)
    {
        for(int j = 0; j < foundPyramid.getOctave(i).getLevelsCount(); j++){
            Image img(foundPyramid.getOctave(i).getLevel(j).getImage());
            DescriptorConstructor constructor2(img);
            foundPyramid.getOctave(i).getLevel(j).getImage().setPoints(constructor2.orientPoints(foundPyramid.getOctave(i).getLevel(j).getImage().getPoints()));
            for(int k = 0; k < foundPyramid.getOctave(i).getLevel(j).getImage().getPoints().size(); k++){
                foundDescriptors.push_back(constructor2.createDescriptor(foundPyramid.getOctave(i).getLevel(j).getImage().getPoints().at(k)));
            }
        }
    }

    findImage(img1, foundImage, createPairs(descriptors1, foundDescriptors), "IMG1");
    findImage(img2, foundImage, createPairs(descriptors2, foundDescriptors), "IMG2");
    findImage(img3, foundImage, createPairs(descriptors3, foundDescriptors), "IMG3");



//    std::vector<std::pair<IntrestingPoint, IntrestingPoint>> pairs1;

//    //Поиск
//    for(int i = 0; i < descriptors3.size(); i++){
//        double firstMinValue = 10000;
//        int firstMinValueIndex = 10000;
//        double secondMinValue = 10000;
//        int secondMinValueIndex = 10000;

//        for(int j = 0; j < foundDescriptors.size(); j++){
//            double dist = descriptors3.at(i).getDistance(foundDescriptors.at(j));
//            if(dist < firstMinValue){
//                secondMinValue = firstMinValue;
//                secondMinValueIndex = firstMinValueIndex;

//                firstMinValue = dist;
//                firstMinValueIndex = j;
//            } else {
//                if(dist < secondMinValue){
//                    secondMinValue = dist;
//                    secondMinValueIndex = j;
//                }
//            }
//        }

//        if(firstMinValue / secondMinValue < 0.8){
//            std::pair<IntrestingPoint, IntrestingPoint> pair;
//            pair.first = descriptors3.at(i).getIntrestingPoint();
//            pair.second = foundDescriptors.at(firstMinValueIndex).getIntrestingPoint();
//            pairs1.push_back(pair);
//        }
//    }

//    findImage(img3, foundImage, pairs1);
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    kurs();
//    lab9();
//    lab6();
//    lab5();
//    lab4();
//    lab3();
//    lab2();
//    lab1();
    return a.exec();
}



