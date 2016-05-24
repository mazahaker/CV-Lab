#ifndef IMAGE_H
#define IMAGE_H

//#include <time.h>
#include <QImage>
#include <QPainter>
#include <QRgb>
#include <QString>
#include <memory>
#include <QtMath>
#include "IntrestingPoint.h"
#include "Kernels.h"
//#include "ImageUtils.h"
//#include <algorithm>

class Image
{
private:
    std::unique_ptr<double[]> Image_;               //представление изображения
    std::vector<IntrestingPoint> points;            //Интересные точки
    int height;                                    //высота
    int width;                                     //ширина
    double minOrig = 300;
    double maxOrig = -300;
    double min = 300;
    double max = -300;

    QImage::Format format_ = QImage::Format_RGB32;  //формат  изображения

    void setHeight(int height_);
    void setWidth(int width_);

public:
    Image();
    Image(QString path);                            //Конструктор с загрузкой и преобразованием в Ч/Б (приводит к 0-1 тут же)
    Image(int sizeX, int sizeY);                    //Конструктор создания пустого изображения
    Image(const Image& image);                      //КОнструктор копирования

    int getHeight() const;
    int getWidth() const;

    void setPixel(int x, int y, double bright);
    double getPixel(int x, int y) const;

    void setPixel(int i, double bright);
    double getPixel(int i) const;

    double toMono(QRgb color);                      //Преобразование в Ч/Б

    void saveImage(QString file, QString message);                   //Сохранение (преобразование к 0-255 тут же)

    void saveImageWithPoints(QString file, QString message);

    Image convolution(const double* kernel, int u, int v) const;            //Свертка

    Image sobelGradient(const Image &xSobel, const Image &ySobel) const;    //Градиент собеля

    void copy(const Image &inputImage);

    void changeSize(int width, int height);

    Image gaussFilter(double sigma);

    static int arrayIndex(int x, int y, int width, int height);

    void moravec(double T);

    void haris(double T);

    std::vector<IntrestingPoint> getPoints();

    void setPoints (std::vector<IntrestingPoint> inputPoints);

    QImage getInputImage();

    Image convolution(const double *row, const double *column, int u, int v) const;

    double harrisForPoint(IntrestingPoint point);
    double convolutionForPoint(const double *kernel, int x, int y) const;

//    std::unique_ptr<double[]> getPixels() ;
};
#endif // IMAGE_H
