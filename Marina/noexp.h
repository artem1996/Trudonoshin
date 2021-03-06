#ifndef NOEXP_H
#define NOEXP_H
#include "gauss.h"

class noExp
{
    int x, y;
    double dx;
    double dy;
    double ax;
    gauss *matrix;
    double* prev_solution;
public:
    noExp(int tx, int ty, double tt, double ta);
    ~noExp();
    double* iteration();
    double** share(double& min, double &max);
    void sharePrint(double** shara);
};

#endif // NOEXP_H
