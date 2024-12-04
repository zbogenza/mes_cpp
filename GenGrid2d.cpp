#include "GlobData.h"
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
