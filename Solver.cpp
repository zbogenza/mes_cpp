#include "GlobData.h"
#include "SolveMatrix.h"
#include "FeSM_Heat.h"
#include <iostream>
extern GlobData data;

void SOLVER() {
    int NEL;
    int nk[4];
    int i, j, ii, jj;

    // Pełna macierz globalna (kwadratowa)
    std::vector<std::vector<double>> globalMatrix(data.mGr.nh, std::vector<double>(data.mGr.nh, 0.0));
    std::vector<double> globalB(data.mGr.nh, 0.0);
    std::vector<double> solutionX(data.mGr.nh, 0.0);

    // Iteracja po elementach
    for (NEL = 0; NEL < data.mGr.ne; NEL++) {
        for (i = 0; i < data.mEL4.nbn; i++) {
            nk[i] = data.mGr.EL[NEL].nop[i];
        }

        // Obliczenie lokalnych macierzy i wektorów
        FeSM_heat(NEL);

        // Montaż lokalnych macierzy do globalnej (pełnej) macierzy
        for (i = 0; i < data.mGr.nbn; i++) {
            ii = nk[i];
            for (j = 0; j < data.mGr.nbn; j++) {
                jj = nk[j];
                globalMatrix[ii][jj] += data.est[i][j];
            }
            globalB[ii] += data.r[i];
        }
    }

    SolveSquareMatrix(globalMatrix, globalB, solutionX);

    // Aktualizacja wyników (temperatury)
    for (i = 0; i < data.mGr.nh; i++) {
        data.mGr.ND[i].CR = (data.mGr.ND[i].t - solutionX[i]) / data.mdTime;
        data.mGr.ND[i].t = solutionX[i];
    }

}
