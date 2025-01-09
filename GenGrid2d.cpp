#include <cmath>
#include "GlobData.h"
#include <iostream>
extern GlobData data;

void GenGrid2d() {
    // Obliczenia liczby węzłów i elementów
    data.mGr.nh = data.mNhH * data.mNhB; // Liczba węzłów
    data.mGr.ne = (data.mNhH - 1) * (data.mNhB - 1); // Liczba elementów
    data.mGr.nbn = 4;  // Liczba węzłów na element
    data.mGr.ncn = 4;  // Liczba punktów interpolacji (powiązane z N1, N2, Nf, itp.)
    data.mGr.nhPov = data.mNhB;  // Liczba węzłów po szerokości

    // Inicjalizacja tablic
    data.mGr.ND.resize(data.mGr.nh); // Węzły
    data.mGr.EL.resize(data.mGr.ne); // Elementy

    // Obliczenie rozmiarów siatki
    double dx = data.mB0 / (data.mNhB - 1); // Odstęp wzdłuż osi X
    double dy = data.mH0 / (data.mNhH - 1); // Odstęp wzdłuż osi Y

    double x = 0, y = 0;
    int inh = 0;
    // Tworzenie węzłów
    for (int i = 0; i < data.mNhB; i++) {
        for (int j = 0; j < data.mNhH; j++) {
            inh++;
            data.mGr.ND[inh - 1].x = x;
            data.mGr.ND[inh - 1].y = y;
            data.mGr.ND[inh - 1].status = 0;
            data.mGr.ND[inh - 1].t = data.mTbegin; // Temperatura początkowa

            y += dy;
        }
        x += dx;
        y = 0;  // Resetowanie y po zakończeniu jednej kolumny
    }

    // Tworzenie elementów
    int ine = 0;
    for (int i = 0; i < data.mNhB - 1; i++) {
        for (int j = 0; j < data.mNhH - 1; j++) {
            ine++;
            int i1 = i * data.mNhH + j;
            int i2 = (i + 1) * data.mNhH + j;
            int i3 = (i + 1) * data.mNhH + j + 1;
            int i4 = i * data.mNhH + j + 1;
            
            // Ustawianie numerów węzłów w elemencie
            data.mGr.EL[ine - 1].nop[0] = i1;
            data.mGr.EL[ine - 1].nop[1] = i2;
            data.mGr.EL[ine - 1].nop[2] = i3;
            data.mGr.EL[ine - 1].nop[3] = i4;
        }
    }

    // Ustawianie statusu węzłów (czy są na brzegu)
    for (int i = 0; i < data.mGr.nh; i++) {
        x = data.mGr.ND[i].x;
        y = data.mGr.ND[i].y;
        data.mGr.ND[i].status = 0;

        // Warunki na powierzchnie brzegowe
        if (x >= (data.mB0 - 0.00001)) {
            data.mGr.ND[i].status = 1; // Boczna powierzchnia
        }
        if (x <= 0.00001) {
            data.mGr.ND[i].status = 1; // Boczna powierzchnia
        }
        if (y >= (data.mH0 - 0.00001)) {
            data.mGr.ND[i].status = 1; // Górna powierzchnia
        }
        if (y <= 0.00001) {
            data.mGr.ND[i].status = 1; // Dolna powierzchnia
        }
    }

    // Ustawianie parametrów elementów
    for (int ine = 0; ine < data.mGr.ne; ine++) {
        int St[4];
        bool St_OK[4];

        // Sprawdzanie statusów węzłów w elemencie
        for (int i = 0; i < 4; i++) {
            int Id = std::abs(data.mGr.EL[ine].nop[i]);
            St[i] = data.mGr.ND[Id].status;
        }

        // Sprawdzanie poprawności sąsiedztw w elementach
        St_OK[1] = (St[0] >= 1) && (St[1] >= 1);
        St_OK[2] = (St[1] >= 1) && (St[2] >= 1);
        St_OK[3] = (St[2] >= 1) && (St[3] >= 1);
        St_OK[0] = (St[3] >= 1) && (St[0] >= 1);

        data.mGr.EL[ine].Npov = 0;
        for (int i = 0; i < 4; i++) {
            if (St_OK[i]) {
                data.mGr.EL[ine].Npov++;
                data.mGr.EL[ine].aPov[data.mGr.EL[ine].Npov - 1] = i;
            }
        }
    }
}

void SetControlPoints() {
    // Przypisanie współrzędnych punktów kontrolnych
    data.mcpX[0] = 0.0;           data.mcpY[0] = 0.0;                   
    data.mcpX[1] = data.mB0 / 2;  data.mcpY[1] = 0.0;                   
    data.mcpX[2] = data.mB0;      data.mcpY[2] = 0.0;                   
    data.mcpX[3] = 0.0;           data.mcpY[3] = data.mH0 / 2.0;
    data.mcpX[4] = data.mB0 / 2;  data.mcpY[4] = data.mH0 / 2.0;
    data.mcpX[5] = data.mB0;      data.mcpY[5] = data.mH0 / 2.0;
    data.mcpX[6] = 0.0;           data.mcpY[6] = data.mH0;
    data.mcpX[7] = data.mB0 / 2;  data.mcpY[7] = data.mH0;
    data.mcpX[8] = data.mB0;      data.mcpY[8] = data.mH0;

    // Zmienna pomocnicza dla najmniejszej odległości
    double Rr, Rmin;

    // Wyszukiwanie najbliższego węzła do punktu kontrolnego
    for (int j = 0; j < 9; ++j) {
        Rmin = 1e10;  // Ustawienie dużej wartości początkowej dla minimalnej odległości
        for (int i = 0; i < data.mGr.nh; ++i) {
            // Obliczenie odległości euklidesowej
            Rr = std::sqrt(std::pow(data.mcpX[j] - data.mGr.ND[i].x, 2) +
                std::pow(data.mcpY[j] - data.mGr.ND[i].y, 2));
            if (Rr <= Rmin) {
                data.mContrPoints[j] = i;  // Zapisanie węzła jako punkt kontrolny
                Rmin = Rr;
            }
        }
    }
}

void WriteControlPoints() {
    // Zapis danych do pliku OutDataT.txt
    FILE* outDataT;
    if (fopen_s(&outDataT, "wyniki/OutDataT.txt", "a") != 0 || outDataT == nullptr) {
        std::cerr << "Nie można otworzyć pliku wyniki/OutDataT.txt!" << std::endl;
        return;
    }

    fprintf(outDataT, " mTau ");
    for (int i = 0; i < 9; i++) {
        fprintf(outDataT, " %7.1f", data.mGr.ND[data.mContrPoints[i]].t);
    }
    fprintf(outDataT, "\n");

    fclose(outDataT);

    // Zapis danych do pliku OutDataCR.txt
    FILE* outDataCR;
    if (fopen_s(&outDataCR, "wyniki/OutDataCR.txt", "a") != 0 || outDataCR == nullptr) {
        std::cerr << "Nie można otworzyć pliku wyniki/OutDataCR.txt!" << std::endl;
        return;
    }

    fprintf(outDataCR, " mTau ");
    for (int i = 0; i < 9; ++i) {
        fprintf(outDataCR, " %7.1f", data.mGr.ND[data.mContrPoints[i]].CR);
    }
    fprintf(outDataCR, "\n");

    fclose(outDataCR);

    // Zapis na ekran
    printf(" mTau ");
    for (int i = 0; i < 9; ++i) {
        printf(" %7.1f", data.mGr.ND[data.mContrPoints[i]].t);
    }
    printf("\n");
}

void WriteControlPointsBegin() {
    FILE* outDataT;
    FILE* outDataCR;

    if (fopen_s(&outDataT, "wyniki/OutDataT.txt", "w") != 0 || outDataT == nullptr) {
        std::cerr << "Nie można otworzyć pliku wyniki/OutDataT.txt!" << std::endl;
        return;
    }
    if (fopen_s(&outDataCR, "wyniki/OutDataCR.txt", "w") != 0 || outDataCR == nullptr) {
        std::cerr << "Nie można otworzyć pliku wyniki/OutDataCR.txt!" << std::endl;
        fclose(outDataT);
        return;
    }
    fprintf(outDataT, " ********** Coordinates of the control points ************\n");
    for (int i = 0; i < 9; ++i) {
        fprintf(outDataT, " No=%4d X=%8.2f Y=%8.2f\n", data.mContrPoints[i], data.mcpX[i], data.mcpY[i]);
    }
    fprintf(outDataT, " **********************************************************\n");

    fprintf(outDataCR, " ********** Coordinates of the control points ************\n");
    for (int i = 0; i < 9; ++i) {
        fprintf(outDataCR, " No=%4d X=%8.2f Y=%8.2f\n", data.mContrPoints[i], data.mcpX[i], data.mcpY[i]);
    }
    fprintf(outDataCR, " **********************************************************\n");

    fclose(outDataT);
    fclose(outDataCR);
}
