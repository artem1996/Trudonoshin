#include "MKR.h"

#define MAXTEMP 200.0
#define START -20.0
#define LENGTHR 100.0
#define LENGTHFI 2*M_PI
#define RINSIDE 300.0

MKR::MKR(int tr, int tfi) {
    r = tr;
    fi = tfi;
    dr = LENGTHR / r;
    dfi = LENGTHFI / fi;
    r += 1;
    fi += 1;
    matrix = new gauss(r * fi);
    matrix -> reset();
    prev_solution = new double[r * fi];
}

MKR::~MKR() {
    delete matrix;
    delete prev_solution;
}

double* MKR::iteration() {
    for (int j = 0; j < fi; ++j) {
        int count = j;
        matrix ->into_constants(count, MAXTEMP);
        matrix ->into_matrix(count, count, 1);
    }
    for(int i = 1; i < r - 1; i++) {
        double rTemp = RINSIDE + dr*i;
        double dr2 = 1.0/(dr*dr);
        double rdr = 1.0/(dr*rTemp);
        double r2dfi2 = 1.0/(dfi*dfi*rTemp*rTemp);
        int j = 0;
        int count = i * fi + j;
        matrix ->into_matrix(count, count, -(2 * dr2 + rdr + 2 * r2dfi2));
        matrix ->into_matrix(count, count+fi, dr2 + rdr);
        matrix ->into_matrix(count, count-fi, dr2);
        matrix ->into_matrix(count, count+1, r2dfi2);
        matrix ->into_matrix(count, count-1 + fi, r2dfi2);
        for(j = 1; j < fi - 1; j++) {
            int count = i * fi + j;
            matrix ->into_matrix(count, count, -(2 * dr2 + rdr + 2 * r2dfi2));
            matrix ->into_matrix(count, count+fi, dr2 + rdr);
            matrix ->into_matrix(count, count-fi, dr2);
            matrix ->into_matrix(count, count+1, r2dfi2);
            matrix ->into_matrix(count, count-1, r2dfi2);
        }
        count = i * fi + j;
        matrix ->into_matrix(count, count, -(2 * dr2 + rdr + 2 * r2dfi2));
        matrix ->into_matrix(count, count+fi, dr2 + rdr);
        matrix ->into_matrix(count, count-fi, dr2);
        matrix ->into_matrix(count, count+1 - fi, r2dfi2);
        matrix ->into_matrix(count, count-1, r2dfi2);
    }
    for (int j = 0; j < fi; ++j) {
        int count = (r - 1) * fi + j;
        if (dfi * j <= (2.0/3.0)*LENGTHFI) {
            matrix ->into_matrix(count, count, 1);
            matrix ->into_matrix(count, count-fi, -1);
        } else {
            matrix ->into_constants(count, START);
            matrix ->into_matrix(count, count, 1);
        }
    }
    if(matrix->triangle()) {
        matrix->print_system();
        return NULL;
    };
    delete[] prev_solution;
    matrix->do_solution();
    prev_solution = matrix->get_solution();
    matrix->reset();
    return prev_solution;
}

double** MKR::share(double& min, double &max) {
    min = max = MAXTEMP;
    double **shareMatrix = new double*[r];
    for(int i = 0; i < r; i++)
        shareMatrix[i] = new double[fi];
    for(int i = 0; i < r; i++)
        for(int j = 0; j < fi; j++) {
            double temp = prev_solution[i * fi + j];
            shareMatrix[i][j] = temp;
            if(temp < min)
                min = temp;
            if(temp > max)
                max = temp;
        }
    return shareMatrix;
}

void MKR::sharePrint(double **shara) {
    for(int j = 0; j < fi; j++) {
       for(int i = 0; i < r; i++) {
           cerr << shara[i][j] << '\t';
       }
       cerr << '\n';
    }
    for(int i = 0; i < r; i++)
        delete[] shara[i];
    delete[] shara;
    cerr << "---------------------------------" << endl;
}

void MKR::approximation(MKR* another, int oneToTwo) {
    //int x2 = (x - 1) * 2 + 1;
    int y2 = (fi - 1) * 2 + 1;
    for(int i = 0; i < r; i++)
        for(int j = 0; j < fi; j++) {
            int count0 = i * fi + j;
            int count1 = i * oneToTwo * y2 + j * oneToTwo;
            //fprintf(stderr, "%d : %d = %d : %d ? %d : %d\n", i, j, i * oneToTwo, j * oneToTwo, count0, count1);
            prev_solution[count0] = (oneToTwo * another -> prev_solution[count1] - prev_solution[count0]) / (oneToTwo - 1);
        }
}
