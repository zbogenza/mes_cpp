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

    for (int iy = 0; iy < data.mNhB; iy++) {
        data.mGr.ND[(iy + 1) * data.mNhB - iy - 1].t = data.mGr.ND[iy*data.mNhB+iy].t;
    }

    //for (int iy = 0; iy < data.mNhB / 2; iy++) {
        //for (int ix = 0; ix < data.mnhb / 2; ix++) {
        //    // indeks w ćwiartce b
        //    int indexb = iy * data.mnhb + (ix + data.mnhb / 2);

        //    // indeks w ćwiartce a
        //    int indexa = iy * data.mnhb + ix;

        //    // odwrócone względem osi pionowej
        //    int reversedindexa = iy * data.mnhb + (data.mnhb / 2 - 1 - ix);

        //    // kopiowanie danych
        //    data.mgr.nd[indexb].t = data.mgr.nd[reversedindexa].t;

        //    int indexc = (iy + data.mnhb / 2) * data.mnhb + ix;

        //    // indeks w ćwiartce d
        //    int indexd = (iy + data.mnhb / 2) * data.mnhb + (ix + data.mnhb / 2);

        //    // odwrócone względem osi pionowej
        //    int reversedindexd = (iy + data.mnhb / 2) * data.mnhb + (data.mnhb - 1 - ix);

        //    // kopiowanie danych d -> c (odwrócone względem pionowej osi)
        //    data.mgr.nd[indexc].t = data.mgr.nd[reversedindexd].t;
        //}
    //}


}
