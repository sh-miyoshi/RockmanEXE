#pragma once
#pragma once

#include <string>
#include "state.h"

// 自キャラの情報を表示するクラス
class MyInfo {
public:
	MyInfo();
	~MyInfo();

	void Draw();
	bool Process();
};