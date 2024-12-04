#include "InpData.h"
#include "GlobData.h"
#include <fstream>
#include <iostream>

void InpData() {
    std::ifstream infile("indata");
    if (!infile) {
        std::cerr << "File error" << std::endl;
        return;
    }
    infile >> data.mTbegin >> data.mTime >> data.mdTime >> data.mT_otoczenia >> data.mAlfa
        >> data.mH0 >> data.mB0 >> data.mNhH >> data.mNhB >> data.mC >> data.mK >> data.mR;
    infile.close();

    std::cout << "Tbegin: " << data.mTbegin << ", Time: " << data.mTime << std::endl;
}
