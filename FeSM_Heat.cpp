#include "GlobData.h"
#include <cmath>
#include <algorithm>
extern GlobData data;

// Deklaracje funkcji pomocniczych
void Jacob_2d(double J_[2][2], double J_inv[2][2], int P, int N_p, int nbn,
    const std::vector<std::vector<double>>& N1,
    const std::vector<std::vector<double>>& N2,
    const double X[], const double Y[], double& DetJ);

void PRE_heat_mat(int NEL);
void PRE_heat_pov_mat(int NEL);

void FeSM_heat(int NEL) {
    // Resetowanie lokalnej macierzy i wektora
    std::fill(&data.est[0][0], &data.est[0][0] + 4 * 4, 0.0);
    std::fill(&data.r[0], &data.r[0] + 4, 0.0);

    // Generowanie macierzy i wektorów lokalnych
    PRE_heat_mat(NEL);
    PRE_heat_pov_mat(NEL);
}

void PRE_heat_mat(int NEL) {
    double X[4], Y[4], Temp_0[4];
    double Ndx[4], Ndy[4];
    double DetJ, T0p;

    // Pobranie współrzędnych węzłów elementu i temperatur
    for (int i = 0; i < data.mEL4.nbn; ++i) {
        int id = std::abs(data.mGr.EL[NEL].nop[i]);
        X[i] = data.mGr.ND[id].x;
        Y[i] = data.mGr.ND[id].y;
        Temp_0[i] = data.mGr.ND[id].t;
    }

    // Iteracja po punktach Gaussa
    for (int P = 0; P < data.mEL4.N_p; ++P) {
        double J_[2][2] = { {0, 0}, {0, 0} };
        double J_inv[2][2] = { {0, 0}, {0, 0} };
        Jacob_2d(J_, J_inv, P, data.mEL4.N_p, data.mEL4.nbn,
            data.mEL4.N1, data.mEL4.N2, X, Y, DetJ);

        T0p = 0.0;
        for (int i = 0; i < data.mEL4.nbn; ++i) {
            Ndx[i] = data.mEL4.N1[i][P] * J_inv[0][0] + data.mEL4.N2[i][P] * J_inv[0][1];
            Ndy[i] = data.mEL4.N1[i][P] * J_inv[1][0] + data.mEL4.N2[i][P] * J_inv[1][1];
            double Ni = data.mEL4.Nf[i][P];
            T0p += Temp_0[i] * Ni;
        }

        DetJ = std::abs(DetJ) * data.mEL4.W[P];

        // Montaż macierzy i wektorów lokalnych
        for (int n = 0; n < data.mEL4.nbn; ++n) {
            for (int i = 0; i < data.mEL4.nbn; ++i) {
                double Ni = data.mEL4.Nf[i][P];
                double Nn = data.mEL4.Nf[n][P];
                double Hin = data.mK * (Ndx[n] * Ndx[i] + Ndy[n] * Ndy[i]) * DetJ;
                double Cin = data.mC * data.mR * Nn * Ni * DetJ;
                data.est[n][i] += Hin + Cin / data.mdTime;
                data.r[n] += (Cin / data.mdTime) * T0p;
            }
        }
    }
}

void PRE_heat_pov_mat(int NEL) {
    double X[4], Y[4];
    double DetJ;

    // Pobranie współrzędnych węzłów elementu
    for (int i = 0; i < data.mEL4.nbn; ++i) {
        int id = std::abs(data.mGr.EL[NEL].nop[i]);
        X[i] = data.mGr.ND[id].x;
        Y[i] = data.mGr.ND[id].y;
    }

    // Warunki brzegowe dla każdej powierzchni
    for (int iPov = 0; iPov < data.mGr.EL[NEL].Npov; ++iPov) {
        int id = data.mGr.EL[NEL].aPov[iPov];
        switch (id) {
        case 0:
            DetJ = std::sqrt(std::pow(X[3] - X[0], 2) + std::pow(Y[3] - Y[0], 2));
            break;
        case 1:
            DetJ = std::sqrt(std::pow(X[0] - X[1], 2) + std::pow(Y[0] - Y[1], 2));
            break;
        case 2:
            DetJ = std::sqrt(std::pow(X[1] - X[2], 2) + std::pow(Y[1] - Y[2], 2));
            break;
        case 3:
            DetJ = std::sqrt(std::pow(X[2] - X[3], 2) + std::pow(Y[2] - Y[3], 2));
            break;
        }

        for (int P = 0; P < 2; ++P) {
            for (int n = 0; n < 4; ++n) {
                for (int i = 0; i < 4; ++i) {
                    double Ni = data.mEL4.Sf[id].Nf[i][P];
                    double Nn = data.mEL4.Sf[id].Nf[n][P];
                    data.est[n][i] += data.mAlfa * Nn * Ni * DetJ;
                }
                double Pn = data.mAlfa * data.mT_otoczenia * data.mEL4.Sf[id].Nf[n][P] * DetJ;
                data.r[n] += Pn;
            }
        }
    }
}

void Jacob_2d(double J_[2][2], double J_inv[2][2], int P, int N_p, int nbn,
    const std::vector<std::vector<double>>& N1,
    const std::vector<std::vector<double>>& N2,
    const double X[], const double Y[], double& DetJ) {
    // Obliczenia macierzy Jacobiego
    J_[0][0] = J_[0][1] = J_[1][0] = J_[1][1] = 0.0;
    for (int i = 0; i < nbn; ++i) {
        J_[0][0] += N1[i][P] * X[i];
        J_[0][1] += N1[i][P] * Y[i];
        J_[1][0] += N2[i][P] * X[i];
        J_[1][1] += N2[i][P] * Y[i];
    }
    DetJ = J_[0][0] * J_[1][1] - J_[0][1] * J_[1][0];

    // Obliczenia odwrotności macierzy Jacobiego
    J_inv[0][0] = J_[1][1] / DetJ;
    J_inv[0][1] = -J_[0][1] / DetJ;
    J_inv[1][0] = -J_[1][0] / DetJ;
    J_inv[1][1] = J_[0][0] / DetJ;
}
