#include "GlobData.h"
#include <iostream>
#include <cmath>
extern GlobData data;

bool SolveSquareMatrix(std::vector<std::vector<double>>& A, std::vector<double>& B, std::vector<double>& X) {
    int n = A.size();
    for (int i = 0; i < n; ++i) {
        // Szukamy największego elementu w kolumnie (pivoting)
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (abs(A[k][i]) > abs(A[maxRow][i])) {
                maxRow = k;
            }
        }

        // Zamiana wierszy
        std::swap(A[i], A[maxRow]);
        std::swap(B[i], B[maxRow]);

        // Eliminacja Gaussa
        for (int k = i + 1; k < n; ++k) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j < n; ++j) {
                A[k][j] -= A[i][j] * factor;
            }
            B[k] -= B[i] * factor;
        }
    }

    // Rozwiązanie układu równań
    for (int i = n - 1; i >= 0; --i) {  
        X[i] = B[i];
        for (int j = i + 1; j < n; ++j) {
            X[i] -= A[i][j] * X[j];
        }
        X[i] /= A[i][i];
    }

    return true;
}
