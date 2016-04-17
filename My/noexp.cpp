#include "noexp.h"

#define MAXTEMP 700
#define START 20
#define LENGTHX 5
#define LENGTXY 5

noExp::noExp(int tx, int ty, double tt, double ta)
{
    x = tx;
    y = ty;
    dx = ta * tt / (5 / x);
    dy = ta * tt / (5 / y);
    x -= 1;
    y -= 1;
    matrix = new gauss(x * y);
    matrix -> reset();
    prev_solution = new double[x * y];
    for (int i = 0; i < x * y; i++) {
        prev_solution[i] = START;
    }
}

noExp::~noExp() {
    delete matrix;
    delete prev_solution;
}

double* noExp::iteration() {
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            int count = i * y + j;
            matrix -> into_constants(count, prev_solution[count]);
            matrix -> into_matrix(count, count, 1 + 2 * dx + 2 * dy);
            if(i == 0) {
                matrix -> into_matrix(count, count, -dx);
                matrix -> into_matrix(count, count + y, -dx);
            } else {
                if(i == x - 1) {
                    matrix -> into_constants(count, dx * MAXTEMP);
                    matrix -> into_matrix(count, count - y, -dx);
                } else {
                    matrix -> into_matrix(count, count + y, -dx);
                    matrix -> into_matrix(count, count - y, -dx);
                }
            }
            if(j == 0) {
                matrix -> into_constants(count, dy * MAXTEMP);
                matrix -> into_matrix(count, count + 1, -dy);
            } else {
                if(j == y - 1) {
                    matrix -> into_matrix(count, count - 1, -dy);
                    if(i < x / 2) {
                        matrix -> into_matrix(count, count, -dy);
                    } else {
                        matrix -> into_constants(count, dy * MAXTEMP);
                    }
                } else {
                    matrix->into_matrix(count, count + 1, -dy);
                    matrix->into_matrix(count, count - 1, -dy);
                }
            }
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

double** noExp::share() {
    int tx = x + 2;
    int ty = y + 2;
    double **shareMatrix = new double*[tx];
    for(int i = 0; i < tx; i++)
        shareMatrix[i] = new double[ty];

    for(int i = 0; i < tx; i++)
        shareMatrix[i][0] = MAXTEMP;
    for(int i = 1; i < ty; i++)
        shareMatrix[tx - 1][i] = MAXTEMP;
    for(int i = 1; i < ty - 1; i++) {
        shareMatrix[0][i] = prev_solution[i - 1];
    }
    shareMatrix[0][ty - 1] = prev_solution[y - 1];
    int i = 1;
    for(; i < tx / 2; i++)
        shareMatrix[i][ty - 1] = prev_solution[i * y - 1];
    for(; i < tx; i++)
        shareMatrix[i][ty - 1] = MAXTEMP;
    for(i = 1; i < tx - 1; i++)
        for(int j = 1; j < ty - 1; j++)
            shareMatrix[i][j] = prev_solution[(i - 1) * y + j - 1];
    return shareMatrix;
}

void noExp::sharePrint(double **shara) {
    int tx = x + 2;
    int ty = y + 2;
    for(int j = 0; j < tx; j++) {
       for(int i = 0; i < ty; i++) {
           cerr << shara[i][j] << '\t';
       }
       delete[] shara[j];
       cerr << '\n';
    }
    delete[] shara;
    cerr << "---------------------------------" << endl;
}
