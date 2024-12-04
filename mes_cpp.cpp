#include <iostream>
#include <cmath>
#include "GlobData.h"
#include "InpData.h"
#include "IniEL4.h"
#include "GenGrid2d.h"
#include "SaveGridToVTK.h"

int main() {
    IniEL4();
    InpData();
    GenGrid2d();

    //std::cout << "Nodes:" << std::endl;
    //for (size_t i = 0; i < data.mGr.ND.size(); ++i) {
    //    std::cout << "Node " << i + 1 << ": ("
    //        << data.mGr.ND[i].x << ", "
    //        << data.mGr.ND[i].y << "), status: "
    //        << data.mGr.ND[i].status << std::endl;
    //}

    //// Wyświetlenie elementów
    //std::cout << "\nElements:" << std::endl;
    //for (size_t i = 0; i < data.mGr.EL.size(); ++i) {
    //    std::cout << "Element " << i + 1 << ": nodes ["
    //        << data.mGr.EL[i].nop[0] + 1 << ", "
    //        << data.mGr.EL[i].nop[1] + 1 << ", "
    //        << data.mGr.EL[i].nop[2] + 1 << ", "
    //        << data.mGr.EL[i].nop[3] + 1 << "], Npov: "
    //        << data.mGr.EL[i].Npov << std::endl;
    //}

    SaveGridToVTK("grid_output.vtk");

    return 0;
}
