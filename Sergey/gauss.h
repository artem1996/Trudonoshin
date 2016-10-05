#ifndef GAUSS_H
#define GAUSS_H
#include <iostream>
#include <cmath>
#include <utility>
//#define capacity 3
#define TROUBLE 0.000001
using namespace std;

class gauss {
    double** matrix;
    double* constants;
    double* solution;
    //double TROUBLE;
    int capacity;
public:
    gauss(int n);
    ~gauss();
    int into_matrix(int, int, double);
    int into_constants(int, double);
    int triangle();
    int do_solution();
    double* get_solution();
    int reset();
    int print_system();
};
#endif // GAUSS_H
