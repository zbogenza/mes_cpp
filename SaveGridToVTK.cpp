﻿#include <fstream>
#include <vector>
#include <iostream>
#include <filesystem>
#include "GlobData.h"
#include "SaveGridToVTK.h"
extern GlobData data;

void SaveGridToVTK(const std::string& filename) {
    std::ofstream file("wyniki/" + filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for writing: " << filename << std::endl;
        return;
    }

    // Nagłówek
    file << "# vtk DataFile Version 3.0" << std::endl;
    file << "2D Grid Data" << std::endl;
    file << "ASCII" << std::endl;
    file << "DATASET UNSTRUCTURED_GRID" << std::endl;

    // Węzły (POINTS)
    file << "POINTS " << data.mGr.ND.size() << " float" << std::endl;
    for (const auto& node : data.mGr.ND) {
        file << node.x << " " << node.y << " 0.0" << std::endl; // Zawsze 0 w osi Z dla 2D
    }

    // Elementy (CELLS)
    file << "CELLS " << data.mGr.EL.size() << " " << (data.mGr.EL.size() * 5) << std::endl;
    for (const auto& elem : data.mGr.EL) {
        file << "4 " // Typ elementu (4 węzły na element)
            << elem.nop[0] << " "
            << elem.nop[1] << " "
            << elem.nop[2] << " "
            << elem.nop[3] << std::endl;
    }

    // Typy elementów (CELL_TYPES)
    file << "CELL_TYPES " << data.mGr.EL.size() << std::endl;
    for (size_t i = 0; i < data.mGr.EL.size(); ++i) {
        file << "9" << std::endl; // Typ 9 oznacza kwadratowy element (VTK_QUAD)
    }

    // Opcjonalnie: zapis danych skalarowych (np. statusu węzłów)
    file << "POINT_DATA " << data.mGr.ND.size() << std::endl;
    file << "SCALARS Node_Status int 1" << std::endl;
    file << "LOOKUP_TABLE default" << std::endl;
    for (const auto& node : data.mGr.ND) {
        file << node.status << std::endl;
    }

    file.close();
}

void SaveResultToVTK(const std::string& filename) {
    std::ofstream file("wyniki/" + filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for writing: " << filename << std::endl;
        return;
    }

    // Nagłówek VTK
    file << "# vtk DataFile Version 3.0" << std::endl;
    file << "2D Temperature Field" << std::endl;
    file << "ASCII" << std::endl;
    file << "DATASET UNSTRUCTURED_GRID" << std::endl;

    // Węzły (POINTS)
    file << "POINTS " << data.mGr.ND.size() << " float" << std::endl;
    for (const auto& node : data.mGr.ND) {
        file << node.x << " " << node.y << " 0.0" << std::endl; // Z dla 2D zawsze 0
    }

    // Elementy (CELLS)
    file << "CELLS " << data.mGr.EL.size() << " " << (data.mGr.EL.size() * 5) << std::endl;
    for (const auto& elem : data.mGr.EL) {
        file << "4 " // Typ elementu (4 węzły na element)
            << elem.nop[0] << " "
            << elem.nop[1] << " "
            << elem.nop[2] << " "
            << elem.nop[3] << std::endl;
    }

    // Typy elementów (CELL_TYPES)
    file << "CELL_TYPES " << data.mGr.EL.size() << std::endl;
    for (size_t i = 0; i < data.mGr.EL.size(); ++i) {
        file << "9" << std::endl; // Typ 9 oznacza kwadratowy element (VTK_QUAD)
    }

    // Dane skalarne temperatury (POINT_DATA)
    file << "POINT_DATA " << data.mGr.ND.size() << std::endl;
    file << "SCALARS Temperature float 1" << std::endl;
    file << "LOOKUP_TABLE default" << std::endl;
    for (const auto& node : data.mGr.ND) {
        file << node.t << std::endl; // Wartość temperatury w węźle
    }

    file.close();
}

void RemoveGridStepFiles(const std::string& directory) {
    namespace fs = std::filesystem;

    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().filename().string().find("grid_step_") == 0) {
                fs::remove(entry.path());
                //std::cout << "Removed file: " << entry.path() << std::endl;
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}