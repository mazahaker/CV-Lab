#include "Image.h"

Image::Image() {
}

Image::Image(QString path)
{
    QImage Picture(path);
    format_ = Picture.format();

    if(Picture.width() != 0 && Picture.height() != 0){

        setWidth(Picture.width());
        setHeight(Picture.height());

        Image_ = std::make_unique<double[]>(Picture.width() * Picture.height());

        for(int x = 0; x < getWidth(); x++){
            for(int y = 0; y < getHeight(); y++){
                setPixel(x,y,toMono(Picture.pixel(x,y)));
            }
        }



        for(int x = 0; x < getWidth(); x++){
            for(int y = 0; y < getHeight(); y++){
                if(getPixel(x,y) > maxOrig) {
                    maxOrig = getPixel(x,y);
                }
                if(getPixel(x,y) < minOrig) {
                    minOrig = getPixel(x,y);
                }
                setPixel(x,y, getPixel(x,y)/255);
//                if(getPixel(x,y) > max) {
//                    max = getPixel(x,y);
//                }
//                if(getPixel(x,y) < min) {
//                    min = getPixel(x,y);
//                }
            }
        }

        printf("min %lf max %lf minOr %lf maxOr %lf\n\n", min,max,minOrig,maxOrig);


        printf("Image load! (%d x %d)\n",getWidth(), getHeight());
    } else {
        printf("Image not load!\n");
        exit(-1);
    }
}

Image::Image(const Image &image) {
    if(image.getWidth() != 0 && image.getHeight() != 0){
        Image_ = std::make_unique<double[]>(image.getWidth() * image.getHeight());
        setWidth(image.getWidth());
        setHeight(image.getHeight());

        format_ = image.format_;

        for(int x = 0; x < image.getWidth(); x++){
            for(int y = 0; y < image.getHeight(); y++){
                setPixel(x,y,image.getPixel(x,y));
            }
        }
    } else {
        printf("Copy image fail\n");
        exit(-1);
    }
}

Image::Image(int sizeX, int sizeY) {
    if(sizeX != 0 && sizeY != 0){
        Image_ = std::make_unique<double[]>(sizeX * sizeY);
        setWidth(sizeX);
        setHeight(sizeY);
    }
}

int Image::getHeight() const {
    return height;
}

int Image::getWidth() const {
    return width;
}

void Image::setHeight(int height_) {
    height = height_;
}

void Image::setWidth(int width_) {
    width = width_;
}

double Image::toMono(QRgb pixel) {
    double bright = ((double)qRed(pixel) * 0.299 + (double)qGreen(pixel) * 0.587 + (double)qBlue(pixel) * 0.114);
    return bright;
}

void Image::setPixel(int x, int y, double bright) {
    Image_[arrayIndex(x,y,width,height)] = bright;
}

void Image::setPixel(int i, double bright) {
    Image_[i] = bright;
}

double Image::getPixel(int x, int y) const {
    return Image_[arrayIndex(x,y,width,height)];
}

double Image::getPixel(int i) const {
    return Image_[i];
}

//std::unique_ptr<double[]> Image::getPixels()  {
//    return Image_;
//}

void Image::saveImage(QString file, QString message) {
    QImage picture(getWidth(),getHeight(),format_);
    double currentBright;
    minOrig = 0;
    maxOrig = 255;
    max = -300;
    min = 300;
    for(int x = 0; x < getWidth(); x++){
        for(int y = 0; y < getHeight(); y++){
            if(getPixel(x,y) > max) {
                max = getPixel(x,y);
            }
            if(getPixel(x,y) < min) {
                min = getPixel(x,y);
            }
        }
    }
    for(int y = 0; y < getHeight(); y++){
        for(int x = 0; x < getWidth(); x++){
            currentBright = ((getPixel(x, y) - min)/(max-min)) * (maxOrig - minOrig);
            picture.setPixel(x, y, qRgb(currentBright, currentBright, currentBright));
        }
    }

    picture.save(file);
    printf("Save file OK! %s \n", message.toLocal8Bit().constData());
}

void Image::saveImageWithPoints(QString file, QString message) {
    QImage picture(getWidth(),getHeight(),format_);
    double currentBright;
    for(int y = 0; y < getHeight(); y++){
        for(int x = 0; x < getWidth(); x++){
            currentBright = getPixel(x, y) * 256;
            picture.setPixel(x, y, qRgb(currentBright, currentBright, currentBright));
        }
    }

    QPainter painter;
    painter.begin(&picture);
    painter.drawImage(QPoint(0,0), picture);

    for(int i = 0; i < points.size(); i++){
        painter.setPen(QColor(rand()%255, rand()%255, rand()%255, 255));
        painter.drawRect(points.at(i).getX() - 5, points.at(i).getY() - 5, 10, 10);
        painter.drawRect(points.at(i).getX() - 4, points.at(i).getY() - 4, 8, 8);
    }

    painter.end();

    picture.save(file);
    printf("Save file with points OK! %s \n", message.toLocal8Bit().constData());
}

Image Image::convolution(const double *kernel, int x_size, int y_size) const {
    Image resultImage = *this;

    int x0, x1, y0, y1, n;
    double resultPixel = 0.0;

    for(int i = 0; i < getWidth(); i++){
        for(int j = 0; j < getHeight(); j++) {
            x0 = i - (x_size / 2);
            x1 = i + (x_size / 2);
            y0 = j - (y_size / 2);
            y1 = j + (y_size / 2);
            resultPixel = 0.0;
            n = x_size * y_size - 1;

            for(int y = y0 ; y <= y1; y++) {
                for(int x = x0; x <= x1; x++, n--){
                    resultPixel += (kernel[n] * getPixel(x,y));
                }
            }
            resultImage.setPixel(i,j,resultPixel);

        }
    }

    return resultImage;
}

//Image Image::convolution(const double *row, const double *column, int x_size, int y_size) const {
//    Image resultImage = *this;

//    int x0, x1, y0, y1, n;
//    double resultPixel = 0.0;

//    for(int i = 0; i < getWidth(); i++){
//        for(int j = 0; j < getHeight(); j++) {
//            x0 = i - (x_size / 2);
//            x1 = i + (x_size / 2);
//            y0 = j - (y_size / 2);
//            y1 = j + (y_size / 2);


//            std::vector<double> temp;
//            for(int xi = x0, n=0; xi <= x1; xi++, n=0){
//                resultPixel = 0.0;
//                for(int yj = y0; yj <= y1; yj++, n++){
//                    resultPixel += (row[n] * getPixel(xi,yj));
//                }
//                temp.push_back(resultPixel);
//            }
//            resultPixel = 0.0;
//            for(int yi = 0; yi < y_size; yi++){
//                resultPixel+=column[yi] * temp.at(yi);
//            }

//            resultImage.setPixel(i,j,resultPixel);

//        }
//    }

//    return resultImage;
//}

Image Image::convolution(const double *row, const double *column, int u, int v) const
{
    return this->convolution(row, u, 1).convolution(column, 1, v);
//    Image resultImage1(getWidth(), getHeight());
//    Image resultImage2(getWidth(), getHeight());
//    resultImage1.copy(convolution(row, u, 1));
//    resultImage2.copy(resultImage1.convolution(column, 1, v));
//    return resultImage2;
}

Image Image::sobelGradient(const Image &xSobel, const Image &ySobel) const
{
    Image resultImage(getWidth(), getHeight());

    for(int x = 0; x < getWidth(); x++){
        for(int y = 0; y < getHeight(); y++) {
            resultImage.setPixel(x, y, sqrt((xSobel.getPixel(x,y) * xSobel.getPixel(x,y)) + (ySobel.getPixel(x,y) * ySobel.getPixel(x,y))));
        }
    }

    return resultImage;
}

void Image::copy(const Image &inputImage)
{
    Image_ = std::make_unique<double[]>((inputImage.getWidth() * inputImage.getHeight()));
    setHeight(inputImage.getHeight());
    setWidth(inputImage.getWidth());

    format_ = inputImage.format_;

    for(int x = 0; x < getWidth(); x++){
        for(int y = 0; y < getHeight(); y++) {
            setPixel(x, y, inputImage.getPixel(x,y));
        }
    }
}

void Image::changeSize(int width, int height) {
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            Image_[arrayIndex(x,y,width,height)] = getPixel(x * 2, y * 2);
        }
    }
    setWidth(width);
    setHeight(height);
}

void Image::setPoints(std::vector<IntrestingPoint> inputPoints) {
    points = inputPoints;
}

std::vector<IntrestingPoint> Image::getPoints() {
    return points;
}

Image Image::gaussFilter(double sigma) {
    if(sigma != 1.0) {
        int kernelSize = ceil(3 * sigma) * 2 + 1;

        printf("Gaus kernel size = %d, Sigma = %f\n", kernelSize, sigma);
        double kernel[kernelSize][kernelSize];
        double kernelRow[kernelSize];
        double kernelColumn[kernelSize];
        double sum = 0;

        for(int x = 0, x_g = - kernelSize / 2; x < kernelSize; x ++, x_g++) {
            for(int y = 0, y_g = - kernelSize / 2; y < kernelSize; y ++, y_g++) {
                kernel[x][y] = pow(M_E, (-(x_g * x_g + y_g * y_g) / (2.0 * (sigma * sigma)))) * (1.0 / (2.0 * M_PI * (sigma * sigma)));
                //printf("%f ", kernel[x][y]);
                sum+=kernel[x][y];
            }
            //printf("\n");
        }
        printf("Kernel summ = %f \n\n", sum);

        sum = 0;
        for(int x = 0, x_g = - kernelSize / 2; x < kernelSize; x ++, x_g++) {
            kernelRow[x] = pow(M_E, (-((pow(x_g,2)) / (2.0 * (pow(sigma,2)))))) / sqrt(2.0 * M_PI * sigma);
            kernelColumn[x] = kernelRow[x];
            //printf("%f ", kernelColumn[x]);
            sum+=kernelColumn[x];
            //printf("\n");
        }
        //printf("Kernel summ1 = %f \n\n", sum);

        //TODO
        if(sum!= 1.0) {
            for(int x = 0; x < kernelSize; x ++) {
                kernelColumn[x] += (1-sum)/kernelSize;
                kernelRow[x] += (1-sum)/kernelSize;
                printf("%f ", kernelColumn[x]);
            }
        }



        return convolution(&kernel[0][0], kernelSize, kernelSize);
        //return convolution(&kernelRow[0], &kernelColumn[0], kernelSize, kernelSize);
    }
    return *this;
}

int Image::arrayIndex(int x, int y, int width, int height) {
    if(x < 0) x = 0;
    if(x >= width) x = width - 1;
    if(y < 0) y = 0;
    if(y >= height) y = height - 1;
    return x * height + y;
}


void Image::moravec(double T) {
    printf("Moravec\n");

    int windowSize = 5/2;
    int shiftSize = 3/2;

    std::unique_ptr<double[]> _operator = std::make_unique<double[]>(width * height);

    points.clear();

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){

            double operatorMinValue = 300.0;

            for(int windowX = -windowSize; windowX < windowSize; windowX++){
                for(int windowY = -windowSize; windowY < windowSize; windowY++){
                    if(windowX == 0 && windowY == 0) continue;

                    double operatorCurValue = 0;
                    double bright;
                    double brightShift;

                    for(int shiftX = -shiftSize; shiftX < shiftSize; shiftX++){
                        for(int shiftY = -shiftSize; shiftY < shiftSize; shiftY++){
                            bright = getPixel(x + shiftX, y + shiftY);
                            brightShift = getPixel(x + shiftX + windowX, y + shiftY + windowY);
                            operatorCurValue += (bright - brightShift) * (bright - brightShift);
                        }
                    }

                    if(operatorCurValue < operatorMinValue){
                        operatorMinValue = operatorCurValue;
                    }
                }
            }

            _operator[arrayIndex(x,y,width,height)] = operatorMinValue;
        }
    }

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){

            if(_operator[arrayIndex(x,y,width,height)] > T){

                bool max = true;
                int areaSize = 3/2;

                for(int px = -areaSize; px < areaSize && max; px++){
                    for(int py = -areaSize; py < areaSize && max; py++){

                        if(_operator[arrayIndex(x,y,width,height)] < _operator[arrayIndex(x+px,y+py,width,height)]){
                            max = false;
                        }
                    }
                }

                if(max){
                    IntrestingPoint point(x, y, _operator[arrayIndex(x,y,width,height)]);
                    points.push_back(point);
                }
            }
        }
    }

    printf("Moravec end\n");
}

void Image::haris(double T) {
    printf("Haris\n");

    int windowSize = 5/2;

    std::unique_ptr<double[]> _operator = std::make_unique<double[]>(width * height);

    points.clear();

    Image imageSobelX(convolution(&sobelX[0][0], 3, 3));
    Image imageSobelY(convolution(&sobelY[0][0], 3, 3));

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){

            double a = 0, b = 0, c = 0;

            for(int windowX = -windowSize; windowX < windowSize; windowX++){
                for(int windowY = -windowSize; windowY < windowSize; windowY++){

                    double Ix = imageSobelX.getPixel(x + windowX, y + windowY);
                    double Iy = imageSobelY.getPixel(x + windowX, y + windowY);

                    a += Ix * Ix;
                    b += Ix * Iy;
                    c += Iy * Iy;
                }
            }

            double det = a * c - b * b;
            double trace = a + c;
            double k = 0.04;
            double f = det - k * trace * trace;

            _operator[arrayIndex(x , y, width, height)] = f;
        }
    }

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){

            if(_operator[arrayIndex(x,y,width,height)] > T){

                bool max = true;
                int areaSize = 3/2;

                for(int px = -areaSize; px < areaSize && max; px++){
                    for(int py = -areaSize; py < areaSize && max; py++){

                        if(_operator[arrayIndex(x,y,width,height)] < _operator[arrayIndex(x+px,y+py,width,height)]){
                            max = false;
                        }
                    }
                }

                if(max){
                    IntrestingPoint point(x, y, _operator[arrayIndex(x,y,width,height)]);
                    points.push_back(point);
                }
            }
        }
    }

    printf("Haris end\n");
}

double Image::harrisForPoint(IntrestingPoint point)
{
    int _dx = 3, _dy = 3;
    double k = 0.06;
    double a = 0, b = 0, c = 0;
    double Ix, Iy;

    for(int dx = -_dx; dx < _dx; dx++){
        for(int dy = -_dy; dy < _dy; dy++){

            Ix = convolutionForPoint(&sobelX[0][0], point.getX() + dx, point.getY() + dy);
            Iy = convolutionForPoint(&sobelY[0][0], point.getX() + dx, point.getY() + dy);

            a += Ix * Ix;
            b += Ix * Iy;
            c += Iy * Iy;
        }
    }

    double det = a * c - b * b;
    double trace = a + c;
    double f = det - k * trace * trace;

    return f;
}

double Image::convolutionForPoint(const double *kernel, int x, int y) const
{
    double result = 0;
    int n = 8;
    for(int i = -1 ; i <= 1; i++) {
        for(int j = -1; j <= 1; j++, n--){
            result += (kernel[n] * getPixel(x + i, y + j));
        }
    }
    return result;
}

QImage Image::getInputImage() {
    QImage image(getWidth(),getHeight(),format_);
    for(int y = 0; y < getHeight(); y++){
        for(int x = 0; x < getWidth(); x++){
            double currentBright = getPixel(x, y) * 255;
            image.setPixel(x, y, qRgb(currentBright, currentBright, currentBright));
        }
    }

    return image;
}
