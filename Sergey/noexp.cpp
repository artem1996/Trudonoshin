#include "noexp.h"

#define MAXTEMP 100
#define START 0
#define LENGTHX 3.0
#define LENGTXY 7.0

noExp::noExp(int tx, int ty, double tt, double ta)
{
    x = tx;
    y = ty;
    ddx = (LENGTHX / x);
    dx = ta * tt / (LENGTHX * LENGTHX / x / x);
    dy = ta * tt / (LENGTXY * LENGTXY / y / y);
    //ax = LENGTHX / x + 1;
    x += 1;
    y += 1;
    matrix = new gauss(x * y);
    matrix -> reset();
    prev_solution = new double[x * y];
    for(int i = 0; i < x * y; i++) {
        prev_solution[i] = START;
    }
    for(int i = 0; i < x; i++)
        prev_solution[i * y] = MAXTEMP;
    for(int i = 0; i < y; i++)
        prev_solution[(x - 1) * y + i] = MAXTEMP;
}

noExp::~noExp() {
    delete matrix;
    delete prev_solution;
}

double* noExp::iteration() {
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            int count = i * y + j;
            if(i == 0 && j == y / 2) {
                matrix -> into_matrix(count, count + y, 2);
                matrix -> into_matrix(count, count, -2 - 3*ddx);
                matrix -> into_constants(count, -30*ddx);
                continue;
            }
            if( i > 0 && i < x - 1 && j > 0 && j < y - 1) {
                matrix -> into_constants(count, prev_solution[count]);
                matrix -> into_matrix(count, count, 1 + 2 * dx + 2 * dy);
                matrix -> into_matrix(count, count + y, -dx);
                matrix -> into_matrix(count, count - y, -dx);
                matrix -> into_matrix(count, count + 1, -dy);
                matrix -> into_matrix(count, count - 1, -dy);
            }
            if(j == 0 || i == x - 1) {
                matrix -> into_constants(count, MAXTEMP);
                matrix -> into_matrix(count, count, 1);
            }
            if(i == 0) {
                matrix -> into_matrix(count, count, 1);
                matrix -> into_matrix(count, count + y, -1);
            }
            if(j == y - 1 && i != 0){
                matrix -> into_matrix(count, count, 1);
                matrix -> into_matrix(count, count - 1, -1);
            }
        }
    }
//    matrix -> print_system();

    delete[] prev_solution;
    if(matrix->triangle()) {
        matrix->print_system();
        return NULL;
    };
    //matrix -> print_system();
    matrix->do_solution();
    prev_solution = matrix->get_solution();
    matrix->reset();
    return prev_solution;
}

double** noExp::share(double& min, double &max) {
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

void noExp::sharePrint(double **shara) {
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
