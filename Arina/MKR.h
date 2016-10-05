#ifndef NOEXP_H
#define NOEXP_H
#include "gauss.h"
#include "cmath"
#include "cstdio"

class MKR
{
    int r, fi;
    double dr, ddx;
    double dfi, ddy;
    double dn;
    gauss *matrix;
    double* prev_solution;
public:
    MKR(int tx, int ty);
    ~MKR();
    double* iteration();
    double** share(double& min, double &max);
    void sharePrint(double** shara);
    void approximation(MKR* another, int oneToTwo);
};

#endif // NOEXP_H
