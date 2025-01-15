#ifndef GLOB_DATA_H
#define GLOB_DATA_H

#include <vector>
#include "MyType.h"

class GlobData {
public:
    double mTbegin;             // Temperatura początkowa
    double mTime;               // Czas procesu
    double mdTime;              // Krok czasowy
    double mTau;                // Bieżący czas
    double mT_otoczenia;        // Temperatura otoczenia
    double mAlfa;               // Współczynnik wymiany ciepła
    double mH0;                 // Wysokość
    double mB0;                 // Szerokość
    double mC;                  // Ciepło właściwe
    double mK;                  // Współczynnik przewodzenia ciepła
    double mR;                  // Gęstość
    int mNhH, mNhB, mLDA;       // Ilość węzłów na wysokość, szerokość, szerokość pasma macierzy
    int mContrPoints[9];
    double mcpX[9], mcpY[9];

    Gr2d mGr;     // Siatka MES
    Elem mEL4;    // Element skończony
    double est[4][4]; // Lokalna macierz sztywności
    double r[4];      // Lokalny wektor obciążeń
    std::vector<double> mB, mX;
    std::vector<std::vector<double>> mA;
};

extern GlobData data;

#endif
