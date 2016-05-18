#ifndef NOEXP_H
#define NOEXP_H
#include "gauss.h"
#include "cmath"

class MKR
{
    int x, y;
    double dx, ddx;
    double dy, ddy;
    gauss *matrix;
    double* prev_solution;
public:
    MKR(int tx, int ty);
    ~MKR();
    double* iteration();
    double** share(double& min, double &max);
    void sharePrint(double** shara);
    void approximation(double** stepX1, double** stepX2);
};

#endif // NOEXP_H
