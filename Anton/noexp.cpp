#include "noexp.h"
#include <cmath>

#define MAXTEMP 100.0
#define START 20.0
#define LENGTH 4.0

noExp::noExp(int ty) {
    y = ty + 1;
    step = LENGTH / ty;
    d = 1.0 / ( step * step);
    normal = (step * sqrt(2) + 1);
    step = 1.0 / (step + 1);
    capacity = 0;
    for(int i = 0; i < ty; capacity += ++i);
    matrix = new gauss(capacity);
    matrix -> reset();
    prev_solution = new double[capacity];
    for (int i = 0; i < capacity; i++) {
        prev_solution[i] = START;
    }
}

noExp::~noExp() {
    delete matrix;
    delete prev_solution;
}

double* noExp::iteration() {
    int control = 1;
    int counter = 1;
    matrix -> into_matrix(0, 0, 1);
    matrix -> into_constants(0, MAXTEMP);
    for(int i = 1; i < capacity - 1; i++) {
//        if(i == capacity - 3) {
//            matrix -> into_constants(i, 200);

//            matrix -> into_matrix(i, i, 1);
//            //matrix -> into_constants(i, 200);
//            continue;
//        }
        if(i == control - 1) {
//по бокам
            matrix -> into_matrix(i, i + counter - 1, 1);
            matrix -> into_matrix(i, i, - normal);
            continue;
        }
        if(i == control) {
            control += ++counter;
            matrix ->into_matrix(i,i,1);
            matrix -> into_constants(i, MAXTEMP);
            continue;
            //matrix -> into_constants(i, 200);
        }

        matrix -> into_matrix(i, i, 4 * d);
        matrix -> into_matrix(i, i - counter + 1, - d);
        matrix -> into_matrix(i, i + 1, - d);
        matrix -> into_matrix(i , i - 1, - d);
        if(control == capacity) {

            matrix -> into_matrix(i, i - counter + 1, - d);

        } else {
            matrix -> into_matrix(i, i + counter, - d);
        }
    }
    matrix -> into_matrix(capacity - 1, capacity - 1, 1);
    matrix -> into_matrix(capacity - 1, capacity - 2, - step);
    matrix -> print_system();
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

double** noExp::share(double& min, double &max) {
    min = max = MAXTEMP;
    int ty = y;
    int tx = y * 2 + 1;
    double **shareMatrix = new double*[tx];
    for(int i = 0; i < tx; i++) {
        shareMatrix[i] = new double[ty];
        for(int j = 0; j < ty; j++)
            shareMatrix[i][j] = 0;
    }
    int point = 0, counter = 1, j = y;
    for(int i = 0; i < capacity; i++) {
        double temp = prev_solution[i];
        if(temp < min)
            min = temp;
        if(temp > max)
            max = temp;
        shareMatrix[counter][--j] = temp;
        shareMatrix[y + y - counter][j] = temp;
        if(i == point) {
            point += ++counter;
            j = y;
        }
    }
    return shareMatrix;
}

void noExp::sharePrint(double **shara) {
    int ty = y;
    int tx = y * 2 + 1;
    for(int j = 0; j < ty; j++) {
       for(int i = 0; i < tx; i++) {
           if(i >= y - j && i <= y + j)
               cerr << shara[i][j];
           cerr << '\t';
       }
       cerr << '\n';
    }
    for(int i = 0; i < tx; i++)
        delete[] shara[i];
    delete[] shara;
    cerr << "---------------------------------" << endl;
}
