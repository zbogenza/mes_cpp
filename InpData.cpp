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
    for (NEL = 0; NEL < data.mGr.ne; ++NEL) {
        std::vector<int> nk(4); // Numery węzłów w elemencie
        for (i = 0; i < 4; ++i) {
            nk[i] = data.mGr.EL[NEL].nop[i]; // Pobieramy numery węzłów z elementu
        }

        // Przechodzimy po węzłach w elemencie
        for (i = 0; i < 4; ++i) {
            ii = nk[i]; // Numer węzła (wiersz w pełnej macierzy)
            for (j = 0; j < 4; ++j) {
                jj = nk[j]; // Numer węzła (kolumna w pełnej macierzy)
                jB = jj - ii + 1; // Różnica między numerami węzłów

                // Aktualizacja rozmiaru macierzy, jeśli potrzebne
                if (jB >= data.mLDA) {
                    data.mLDA = jB;
                    NeMaxB = NEL;
                }
            }
        }
    }

    // Alokacja pamięci dla macierzy i wektorów
    data.mA.resize(data.mLDA, std::vector<double>(data.mGr.nh, 0.0)); // Macierz A (mLDA x mGr.nh)
    data.mB.resize(data.mGr.nh, 0.0); // Wektor B
    data.mX.resize(data.mGr.nh, 0.0); // Wektor X

    // Debugging: Sprawdzanie rozmiaru alokowanych macierzy
    std::cout << "Rozmiar mA: " << data.mLDA << " x " << data.mGr.nh << std::endl;
    std::cout << "Rozmiar mB: " << data.mGr.nh << std::endl;
    std::cout << "Rozmiar mX: " << data.mGr.nh << std::endl;
}