#ifndef NOEXP_H
#define NOEXP_H
#include "gauss.h"
#include "cmath"

class explicitMethod
{
    int x, y;
    double kx;
    double ky;
    double dx;
    double dy;
    double dn;
    gauss *matrix;
    double* prev_solution;
public:
    explicitMethod(int tx, int ty, double tt, double ta);
    ~explicitMethod();
    double* iteration();
    double** share(double& min, double &max);
    void sharePrint(double** shara);
};

#endif // NOEXP_H
