#ifndef FESM_HEAT_H
#define FESM_HEAT_H
#include <vector>

void Jacob_2d(double J_[2][2], double J_inv[2][2], int P, int N_p, int nbn,
    const std::vector<std::vector<double>>& N1,
    const std::vector<std::vector<double>>& N2,
    const double X[], const double Y[], double& DetJ);
void FeSM_heat(int NEL);
void PRE_heat_mat(int NEL);
void PRE_heat_pov_mat(int NEL);

#endif