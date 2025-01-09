#include <iostream>
#include <cmath>
#include <cstdlib>
#include "GlobData.h"
#include "InpData.h"
#include "IniEL4.h"
#include "GenGrid2d.h"
#include "SaveGridToVTK.h"
#include "Solver.h"

int main() {
    double dTauMax, TauP;
    RemoveGridStepFiles("wyniki");
    IniEL4();
    InpData();
    GenGrid2d();

    SetControlPoints();
    ALLOCATE_Matrix();
    WriteControlPointsBegin();

    double Asr = data.mK / (data.mC * data.mR);

    //std::string filename = "grid_step_0.vtk";
    //SaveResultToVTK(filename);
    WriteControlPoints();
    std::string filename = "grid_test.vtk";
    SaveGridToVTK(filename);
    int NTau = static_cast<int>(data.mTime / data.mdTime); //+1;

    data.mTau = 0.0;

    for (int n = 1; n <= NTau; ++n) {
        data.mTau += data.mdTime;
        SOLVER();
        WriteControlPoints();
        std::string filename = "grid_step_" + std::to_string(n) + ".vtk";
        SaveResultToVTK(filename);
    }

    return 0;
}
