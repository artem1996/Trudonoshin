#include <iostream>
#include <gauss.h>

int main(int argc, char *argv[])
{
    int n;
    std::cout << "Введите количество неизвестных\n";
    std::cin >> n;
    gauss matrix(n);
    std::cout << "Введите построчно марицу коэфициентов\n";
    for(int i = 0; i < n; i++) {
        std::cout << "Введите " << i + 1 <<"-ю строку\n";
        for(int j = 0; j < n; j++) {
            double value;
            std::cin >> value;
            matrix.into_matrix(i, j, value);
        }
    }
    std::cout << "Введите марицу констант\n";
    for(int i = 0; i < n; i++) {
        double value;
        std::cin >> value;
        matrix.into_constants(i, value);
    }
    matrix.triangle();
    matrix.do_solution();
    double* solution = matrix.get_solution();
    std::cout << "Ответ: ";
    for(int i = 0; i < n; i++) {
        std::cout << solution[i] << ", ";
    }
    std::cout << "null" << std::endl;
    delete solution;
    return 0;
}
