#include "explicitMethod.h"

#define MAXTEMP 100
#define START 10
#define LENGTHX 8.0
#define LENGTHY 6.0

explicitMethod::explicitMethod(int tx, int ty, double tt, double ta) {
    x = tx;
    y = ty;
    dx = LENGTHX / x;
    dy = LENGTHY / y;
    dn = sqrt(dx * dx + dy * dy);
    kx = ta * tt / dx / dx;
    ky = ta * tt / dy / dy;
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

explicitMethod::~explicitMethod() {
    delete matrix;
    delete prev_solution;
}

double* explicitMethod::iteration() {
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            int count = i * y + j;
            if(i == 0) {
                if(j == 0) {
                    matrix -> into_matrix(count, count, - 1 - dn);
                    matrix -> into_matrix(count, count + y + 1, 1);
                } else
                if(j == y - 1) {
                    matrix -> into_matrix(count, count, - 1 - dn);
                    matrix -> into_matrix(count, count + y - 1, 1);
                } else {
                    matrix -> into_matrix(count, count, - 1 - dx);
                    matrix -> into_matrix(count, count + y, 1);
                }
                continue;
            }
            if(i == x - 1) {
                if(j == 0) {
                    matrix -> into_matrix(count, count, - 1 - dn);
                    matrix -> into_matrix(count, count - y + 1, 1);
                } else
                if(j == y - 1) {
                    matrix -> into_matrix(count, count, - 1 - dn);
                    matrix -> into_matrix(count, count - y - 1, 1);
                } else {
                    matrix -> into_matrix(count, count, - 1 - dx);
                    matrix -> into_matrix(count, count - y, 1);
                }
                continue;
            }
            if(j == 0) {
                matrix -> into_matrix(count, count, - 1 - dy);
                matrix -> into_matrix(count, count + 1, 1);
                continue;
            }
            if(j == y - 1) {
                matrix -> into_matrix(count, count, - 1 - dy);
                matrix -> into_matrix(count, count - 1, 1);
                continue;
            }
            if(i >= x / 4 && i <= x / 4 * 3 && j >= y / 3 && j <= y / 3 * 2) {
                matrix -> into_constants(count, MAXTEMP);
                matrix -> into_matrix(count, count, 1);
                continue;
            }
            matrix -> into_matrix(count, count, 1);
            matrix -> into_constants(count, kx * (prev_solution[count - y] - 2 * prev_solution[count] + prev_solution[count + y]));
            matrix -> into_constants(count, ky * (prev_solution[count - 1] - 2 * prev_solution[count] + prev_solution[count + 1]));
            matrix -> into_constants(count, prev_solution[count]);
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

double** explicitMethod::share(double& min, double &max) {
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

void explicitMethod::sharePrint(double **shara) {
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
