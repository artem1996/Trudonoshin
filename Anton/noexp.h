#ifndef NOEXP_H
#define NOEXP_H
#include "gauss.h"

class noExp
{
    int capacity, y;
    double d, normal, step;
    gauss *matrix;
    double* prev_solution;
public:
    noExp(int step, double tt, double ta);
    ~noExp();
    double* iteration();
    double** share(double& min, double &max);
    void sharePrint(double** shara);
};

#endif // NOEXP_H
