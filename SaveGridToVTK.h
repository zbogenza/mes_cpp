#ifndef SAVE_GRID_TO_VTK_H
#define SAVE_GRID_TO_VTK_H

#include <string>

void SaveGridToVTK(const std::string& filename);
void SaveResultToVTK(const std::string& filename);
void RemoveGridStepFiles(const std::string& directory);

#endif
