#ifndef GLOB_DATA_H
#define GLOB_DATA_H

#include <vector>
#include "MyType.h"

class GlobData {
public:
    double mTbegin, mTime, mdTime, mTau, mT_otoczenia, mAlfa;
    double mH0, mB0, mC, mK, mR;
    int mNhH, mNhB, mLDA;
    int mContrPoints[9];
    double mcpX[9], mcpY[9];

    Gr2d mGr;     // Siatka MES
    Elem mEL4;    // Element skończony
    double est[4][4]; // Lokalna macierz
    double r[4];      // Lokalny wektor
    std::vector<double> mB, mX; // Wektory globalne
    std::vector<std::vector<double>> mA; // Macierz globalna
};

extern GlobData data;

#endif
