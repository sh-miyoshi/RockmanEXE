#pragma once

#include <string>
#include <random>

#include "DxLib.h"
#include "define.h"
#include "appLogger.h"
#include "util.h"
#include "utilDxLib.h"
#include "keyConfig.h"

extern bool gExitFlag;
extern unsigned long long gGameCount;
extern std::mt19937_64 rnd_generator;