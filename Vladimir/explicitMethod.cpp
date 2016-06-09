#include "explicitMethod.h"

#define MAXTEMP 100
#define START 10
#define LENGTHX 8.0
#define LENGTHY 6.0
#define TROUBLE 0.01

explicitMethod::explicitMethod(int tx, int ty, double tT, double ta) {
    x = tx;
    y = ty;
    dx = LENGTHX / x;
    dy = LENGTHY / y;
    dn = sqrt(dx * dx + dy * dy);
    kx = ta * dx / dx;
    ky = ta * dy / dy;
    maxDT = tT;
    x += 1;
    y += 1;
    matrix = new double*[x];
    prev_solution = new double*[x];
    for(int i = 0; i < x; i++) {
        matrix[i] = new double[y];
        prev_solution[i] = new double[y];
    }
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            if(i >= x / 4 && i <= x / 4 * 3 && j >= y / 3 && j <= y / 3 * 2)
                prev_solution[i][j] = MAXTEMP;
            else
                prev_solution[i][j] = START;
        }
    }
}

explicitMethod::~explicitMethod() {
    for(int i = 0; i < x; i++) {
        delete[] matrix[i];
        delete[] prev_solution[i];
    }
    delete[] matrix;
    delete[] prev_solution;
}

double explicitMethod::iteration() {
    double tTemp = 0;
    for(int i = 1; i < x - 1; i++) {
        for(int j = 1; j < y - 1; j++) {
            if((i < x / 4 || i > x / 4 * 3) || (j < y / 3 || j > y / 3 * 2)) {//считаем производные в точках
                double temp = kx * (prev_solution[i - 1][j] - 2 * prev_solution[i][j] + prev_solution[i + 1][j]) + ky * (prev_solution[i][j - 1] - 2 * prev_solution[i][j] + prev_solution[i][j + 1]);//производная по времени
                if(fabs(temp) > tTemp) //находится макс произвольная по времени и записывается в temp
                    tTemp = temp;
            }
        }
    }
    if(tTemp < TROUBLE) return 0;//если макс произвольная меньше то выход
    tTemp = maxDT / tTemp;//считаем шаг по времени
    for(int i = 1; i < x - 1; i++) {
        for(int j = 1; j < y - 1; j++) {
            if((i < x / 4 || i > x / 4 * 3) || (j < y / 3 || j > y / 3 * 2)) {
                matrix[i][j] = prev_solution[i][j] + tTemp * (kx * (prev_solution[i - 1][j] - 2 * prev_solution[i][j] + prev_solution[i + 1][j]) + ky * (prev_solution[i][j - 1] - 2 * prev_solution[i][j] + prev_solution[i][j + 1]));
            }
        }
    }
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            if(i == 0) {
                if(j == 0) {
                    matrix[i][j] = matrix[1][1] / (1 + dn);
                } else
                if(j == y - 1) {
                    matrix[i][j] = matrix[1][y - 2] / (1 + dn);
                } else {
                    matrix[i][j] = matrix[1][j] / (1 + dx);
                }
                continue;
            }
            if(i == x - 1) {
                if(j == 0) {
                    matrix[i][j] = matrix[i - 1][1] / (1 + dn);
                } else
                if(j == y - 1) {
                    matrix[i][j] = matrix[i - 1][j - 1] / (1 + dn);
                } else {
                    matrix[i][j] = matrix[i - 1][j] / (1 + dx);
                }
                continue;
            }
            if(j == 0) {
                matrix[i][j] = matrix[i][1] / (1 + dy);
                continue;
            }
            if(j == y - 1) {
                matrix[i][j] = matrix[i][j - 1] / (1 + dy);
                continue;
            }
            if(i >= x / 4 && i <= x / 4 * 3 && j >= y / 3 && j <= y / 3 * 2) {
                matrix[i][j] = MAXTEMP;
                continue;
            }
        }
    }
    double** temp = prev_solution;
    prev_solution = matrix;
    matrix = temp;
    return tTemp;
}

double** explicitMethod::sharePrint(double& min, double &max) {
    min = max = MAXTEMP;
    for(int j = 0; j < y; j++) {
       for(int i = 0; i < x; i++) {
           double temp = prev_solution[i][j];
           if(temp < min)
               min = temp;
           if(temp > max)
               max = temp;
           std::cout << temp << '\t';
       }
       std::cout << '\n';
    }
    std::cout << "---------------------------------" << std::endl;
    return prev_solution;
}

void explicitMethod::reset() {
    for(int i = 0; i < x; i++) {
       for(int j = 0; j < y; j++) {
           matrix[i][j] = 0;
       }
    }
}
