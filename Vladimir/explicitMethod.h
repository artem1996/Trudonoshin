#ifndef NOEXP_H
#define NOEXP_H
#include "cmath"
#include "iostream"

class explicitMethod
{
    int x, y;
    double kx;
    double ky;
    double dx;
    double dy;
    double dn;
    double maxDT;
    double **matrix;
    double **prev_solution;
public:
    explicitMethod(int tx, int ty, double tt, double ta);
    ~explicitMethod();
    void reset();
    double iteration();
    double** sharePrint(double& min, double &max);
};

#endif // NOEXP_H
