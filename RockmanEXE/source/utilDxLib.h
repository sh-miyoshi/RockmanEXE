#pragma once

#include <string>
#include <windows.h>
#include "appLogger.h"

int LoadGraphWithCheckError(std::string fileName, std::string position);
void LoadDivGraphWithCheckError(int *handle, std::string fileName, std::string position, int xnum, int ynum, int xsize, int ysize);
int LoadSoundMemWithCheckError(std::string fileName, std::string position);

void DrawStringCenter(int x, int y, std::string val, int color);

// ゲームロジックに関するエラーをキャッチ
#if defined(_DEBUG) || defined(DEBUG)
#define ASSERT(cond,msg) { if( !(cond) ) DebugBreak(); }
#else
#define ASSERT(cond,msg) { if( !(cond) ) AppLogger::Error(msg); }
#endif