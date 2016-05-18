#ifndef KERNELS
#define KERNELS

double const blur[3][3] = {
    {1.0 / 9, 1.0 / 9, 1.0 / 9},
    {1.0 / 9, 1.0 / 9, 1.0 / 9},
    {1.0 / 9, 1.0 / 9, 1.0 / 9}
};

double const emboss[3][3] = {
    {-2, -1, 0},
    {-1, 1, 1},
    {0, 1, 2}
};

double const sharpness[3][3] = {
    {0, -1, 0},
    {-1, 5, -1},
    {0, -1, 0}
};

double const move[3][3] = {
    {0, 0, 0},
    {0, 0, 1},
    {0, 0, 0}
};

double const sobelX[3][3] = {
    {-1.0, 0.0, 1.0},
    {-2.0, 0.0, 2.0},
    {-1.0, 0.0, 1.0}
};

double const sobelY[3][3] = {
    {-1.0,-2.0,-1.0},
    { 0.0, 0.0, 0.0},
    { 1.0, 2.0, 1.0}
};


#endif // KERNELS

