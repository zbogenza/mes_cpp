#include "GlobData.h"
#include <iostream>
#include <cmath>
extern GlobData data;

bool SolveBandMatrix(
    const std::vector<std::vector<double>>& bandMatrix,
    const std::vector<double>& rhs,
    std::vector<double>& solution,
    int bandwidth
) {
    int n = rhs.size();
    solution.resize(n);
    std::vector<std::vector<double>> A = bandMatrix;
    std::vector<double> B = rhs;

    // Forward Elimination
    for (int i = 0; i < n; ++i) {
        // Skalowanie wiersza
        if (fabs(A[bandwidth][i]) < 1e-12) {
            std::cerr << "Macierz jest osobliwa lub bliska osobliwości.\n";
            return false;
        }
        double diag = A[bandwidth][i];
        for (int k = 0; k <= 2 * bandwidth; ++k) {
            if (i + k - bandwidth >= 0 && i + k - bandwidth < n) {
                A[k][i] /= diag;
            }
        }
        B[i] /= diag;

        // Zerowanie elementów poniżej przekątnej w obrębie pasma
        for (int j = i + 1; j < n && j <= i + bandwidth; ++j) {
            double factor = A[bandwidth + j - i][i];
            for (int k = 0; k <= 2 * bandwidth; ++k) {
                if (j + k - bandwidth >= 0 && j + k - bandwidth < n) {
                    A[k][j] -= factor * A[k][i];
                }
            }
            B[j] -= factor * B[i];
        }
    }

    // Back Substitution
    for (int i = n - 1; i >= 0; --i) {
        solution[i] = B[i];
        for (int j = i + 1; j < n && j <= i + bandwidth; ++j) {
            solution[i] -= A[bandwidth + j - i][i] * solution[j];
        }
    }

    return true;
}

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

    return true;  // Zwracamy true, jeśli udało się rozwiązać układ
}
