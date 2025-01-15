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

void ALLOCATE_Matrix() {
    int NEL, i, j, ii, jj, jB;
    int NeMaxB = 0;

    data.mLDA = 0;

    // Przechodzimy przez wszystkie elementy siatki
    for (NEL = 0; NEL < data.mGr.ne; NEL++) {
        std::vector<int> nk(4); // Numery węzłów w elemencie
        for (i = 0; i < 4; ++i) {
            nk[i] = data.mGr.EL[NEL].nop[i]; // Pobieramy numery węzłów z elementu
        }

        // Przechodzimy po węzłach w elemencie
        for (i = 0; i < data.mGr.nbn; i++) {
            ii = nk[i]; // Numer węzła (wiersz)
            for (j = 0; j < data.mGr.nbn; j++) {
                jj = nk[j]; // Numer węzła (kolumna)
                jB = jj - ii + 1; // Różnica między numerami węzłów

                if (jB >= data.mLDA) {
                    data.mLDA = jB;
                    NeMaxB = NEL;
                }
            }
        }
    }

    // Alokacja pamięci dla macierzy i wektorów
    data.mA.resize(data.mGr.nh, std::vector<double>(data.mGr.nh, 0.0));
    data.mB.resize(data.mGr.nh, 0.0);
    data.mX.resize(data.mGr.nh, 0.0);

    //std::cout << "Rozmiar mA: " << data.mGr.nh << " x " << data.mGr.nh << std::endl;
    //std::cout << "Rozmiar mB: " << data.mGr.nh << std::endl;
    //std::cout << "Rozmiar mX: " << data.mGr.nh << std::endl;
}