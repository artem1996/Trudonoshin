#include "MKR.h"

#define MAXTEMP 200
#define START 40
#define LENGTHX 10.0
#define LENGTHY 6.0

MKR::MKR(int tx, int ty) {
    x = tx;
    y = ty;
    dx = x / LENGTHX;
    dy = y / LENGTHY;;
    ddx = dx * dx;
    ddy = dy * dy;
    x += 1;
    y += 1;
    matrix = new gauss(x * y);
    matrix -> reset();
    prev_solution = new double[x * y];
    for(int i = 0; i < x * y; i++) {
        if(i / y >= x / 4 && i / y <= x / 4 * 3 && i % y >= y / 3 && i % y <= y / 3 * 2)
            prev_solution[i] = MAXTEMP;
        else
            prev_solution[i] = START;
    }
}

MKR::~MKR() {
    delete matrix;
    delete prev_solution;
}

double* MKR::iteration() {
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            int count = i * y + j;
            if(i == 0 && j == y/2){
                matrix -> into_matrix(count, count, -1-dx);
                matrix -> into_matrix(count, count+y, dx);
                matrix -> into_constants(count, 10);
                continue;
            }
            if(i == x - 1) {
                matrix -> into_matrix(count, count, -1);
                matrix -> into_matrix(count, count - y, 1);
                continue;
            }
            if(i == 0 || j == 0 || j == y - 1) {
                matrix -> into_matrix(count, count, 1);
                matrix -> into_constants(count, START);
                continue;
            }
            if(i >= x / 4 && i <= x / 4 * 3 && j >= y / 3 && j <= y / 3 * 2) {
                matrix -> into_constants(count, MAXTEMP);
                matrix -> into_matrix(count, count, 1);
                continue;
            }
            matrix -> into_matrix(count, count, -2 * ddx - 2 * ddy);
            matrix -> into_matrix(count, count - y, ddx);
            matrix -> into_matrix(count, count + y, ddx);
            matrix -> into_matrix(count, count - 1, ddy);
            matrix -> into_matrix(count, count + 1, ddy);
        }
    }
    delete[] prev_solution;
    if(matrix->triangle()) {
        matrix->print_system();
        return NULL;
    };
    matrix->do_solution();
    prev_solution = matrix->get_solution();
    matrix->reset();
    return prev_solution;
}

double** MKR::share(double& min, double &max) {
    min = max = MAXTEMP;
    double **shareMatrix = new double*[x];
    for(int i = 0; i < x; i++)
        shareMatrix[i] = new double[y];
    for(int i = 0; i < x; i++)
        for(int j = 0; j < y; j++) {
            double temp = prev_solution[i * y + j];
            shareMatrix[i][j] = temp;
            if(temp < min)
                min = temp;
            if(temp > max)
                max = temp;
        }
    return shareMatrix;
}

void MKR::sharePrint(double **shara) {
    for(int j = 0; j < y; j++) {
       for(int i = 0; i < x; i++) {
           cerr << shara[i][j] << '\t';
       }
       cerr << '\n';
    }
    for(int i = 0; i < x; i++)
        delete[] shara[i];
    delete[] shara;
    cerr << "---------------------------------" << endl;
}
