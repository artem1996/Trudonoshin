#include "gauss.h"

gauss::gauss(int n) {
    capacity = n;
    matrix = new double*[capacity];
    for(int i = 0; i < capacity; i++) {
        matrix[i] = new double[capacity];
    }
    constants = new double[capacity];
    solution = new double[capacity];
}

gauss::~gauss() {
    for(int i = 0; i < capacity; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] constants;
    delete[] solution;
}

int gauss::into_matrix(int str, int column, double value) {
    matrix[str][column] += value;
}

int gauss::into_constants(int str, double value) {
    constants[str] += value;
}

double* gauss::get_solution() {
    double* answer = new double[capacity];
    for(int i = 0; i < capacity; i++) {
        answer[i] = solution[i];
    }
    return answer;
}

int gauss::triangle() {
    for(int i = 0; i < capacity; i++) {
        if(fabs(matrix[i][i]) < TROUBLE) {
            int j;
            for(j = i + 1; j < capacity; j++) {
                if(fabs(matrix[j][i]) >= TROUBLE) {
                    std::swap(matrix[i], matrix[j]);
                    std::swap(constants[i], constants[j]);
                    break;
                }
            }
            if(j == capacity) {
                cout << "Матрица похожа на вырожденную в строке: " << i + 1 << endl;
                return -1;
            }
        }
        for(int j = i + 1; j < capacity; j++) {
            if (fabs(matrix[j][i]) >= TROUBLE) {
                double mult = matrix[j][i] / matrix[i][i];
                constants[j] -= constants[i] * mult;
                matrix[j][i] = 0;
                for(int k = i + 1; k < capacity; k++) {
                    matrix[j][k] -= matrix[i][k] * mult;
                }
            }
        }
    }
    return 0;
}

int gauss::do_solution() {
    for(int i = capacity - 1; i > -1; i--) {
        double only_con = constants[i];
        for(int j = capacity - 1; j > i; j--) {
            only_con -= matrix[i][j] * solution[j];
        }
        solution[i] = only_con / matrix[i][i];
    }
}

int gauss::reset() {
    for(int i = 0; i < capacity; i++) {
        for(int j = 0; j < capacity; j++) {
            matrix[i][j] = 0;
        }
        constants[i] = 0;
        solution[i] = 0;
    }
}

int gauss::print_system() {
    for(int i = 0; i < capacity; i++) {
        for(int j = 0; j < capacity; j++) {
            cout << '|' << matrix[i][j] << '\t';
        }
        cerr << '|' << constants[i] << endl;
    }
    cout << ".................." << endl;
}
