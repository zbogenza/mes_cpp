#ifndef MY_TYPE_H
#define MY_TYPE_H

#include <vector>

struct Node {
    double x, y, t, CR;
    int status;
};

struct Element {
    int nop[4];
    int Npov;
    int aPov[2];
};

struct Gr2d {
    int nh, ne, nbn, ncn, nhPov;
    std::vector<Element> EL;
    std::vector<Node> ND;
};

struct CorL {
    double N, E;
};

struct Gran {
    int N_p;
    std::vector<CorL> P;
    std::vector<double> W;
    std::vector<std::vector<double>> N1, N2, Nf;
    std::vector<char> UZEL;
};

struct Elem {
    int nbn, nbnp, N_p;
    std::vector<std::vector<double>> N1, N2, Nf;
    std::vector<CorL> P, L;
    std::vector<double> W;
    Gran Sf[4];
};

#endif
