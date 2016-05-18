//#include <QCoreApplication>
#include <QApplication>
#include "Image.h"
#include "ImageUtils.h"
#include "DescriptorConstructor.h"

#include <QPixmap>
#include <QPainter>
#include <QTextDocument>


#include "Pyramid.h"

void lab1() {
    Image resultImage("D:\\KazakovImages\\input\\3.jpg");
    resultImage.saveImage("D:\\KazakovImages\\output\\1.jpg", "MONO");
    resultImage.convolution(&blur[0][0], 3, 3).saveImage("D:\\KazakovImages\\output\\blur.jpg", "blur");
    resultImage.convolution(&emboss[0][0], 3, 3).saveImage("D:\\KazakovImages\\output\\emboss.jpg", "emboss");
    resultImage.convolution(&sharpness[0][0], 3, 3).saveImage("D:\\KazakovImages\\output\\sharpness.jpg", "sharpness");

    ImageUtils::normalize(resultImage.convolution(&sobelX[0][0], 3, 3)).saveImage("D:\\KazakovImages\\output\\sobelX.jpg", "sobelX");
    ImageUtils::normalize(resultImage.convolution(&sobelY[0][0], 3, 3)).saveImage("D:\\KazakovImages\\output\\sobelY.jpg", "sobelY");
    ImageUtils::normalize(resultImage.sobelGradient(resultImage.convolution(&sobelX[0][0], 3, 3), resultImage.convolution(&sobelY[0][0], 3, 3))).saveImage("E:\\KazakovImages\\output\\sobelG.jpg", "sobelG");
    printf("\nEXIT");
}

void lab2() {
    Image resultImage("D:\\KazakovImages\\input\\3.jpg");
    Pyramid pyramid(resultImage, 1.6, 5, 5);
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

    image.save("D:\\combo1.jpg");
     printf("\nEXIT");


}

void lab5() {
    Image img1("D:\\KazakovImages\\input\\k1.jpg");
    img1.haris(4);
    img1.setPoints(ImageUtils::ANMS(img1.getPoints(), 500));
    DescriptorConstructor constructor1(img1);
    img1.setPoints(constructor1.orientPoints(img1.getPoints()));
    std::vector<Descriptor> descriptors1;

    for(int i = 0; i < img1.getPoints().size(); i++){
        descriptors1.push_back(constructor1.createDescriptor(img1.getPoints().at(i)));
    }

    Image img2("D:\\KazakovImages\\input\\k1_180.jpg");
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    lab5();
    return a.exec();
}

