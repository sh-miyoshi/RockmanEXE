#pragma once

#include <string>
#include <windows.h>
#include "appLogger.h"

int LoadGraphWithErrorCheck(std::string fileName, std::string position);
void LoadDivGraphWithErrorCheck(int *handle, std::string fileName, std::string position, int xnum, int ynum, int xsize, int ysize);
int LoadSoundMemWithErrorCheck(std::string fileName, std::string position);
