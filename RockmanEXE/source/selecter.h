#pragma once

#include <string>
#include <vector>
#include "point.h"

class Selecter {
	typedef struct {
		std::string msg;
		bool allowSelect;
	}SelectData;

	CPoint<int> firstPos;
	int color;
	std::vector<SelectData> data;
	int pointer;
public:
	Selecter();
	~Selecter();

	void SetData(CPoint<int> firstPos, std::vector<std::string> data, int color);

	void Draw();
	int Process();

	void SetUnSelectable(int index);
	void SetPointer(int pointer);
};