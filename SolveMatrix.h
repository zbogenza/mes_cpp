#ifndef SOLVE_BAND_MATRIX_H
#define SOLVE_BAND_MATRIX_H

#include <vector>

bool SolveBandMatrix(const std::vector<std::vector<double>>& bandMatrix,
    const std::vector<double>& rhs,
    std::vector<double>& solution,
    int bandwidth);
bool SolveSquareMatrix(std::vector<std::vector<double>>& A, std::vector<double>& B, std::vector<double>& X);


#endif
