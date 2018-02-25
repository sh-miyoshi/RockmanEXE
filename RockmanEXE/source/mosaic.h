#pragma once

#include <vector>
#include "point.h"

class Mosaic{
	CPoint<int> pos;
	CPoint<int> size;
	CPoint<int> separateSize;
	std::vector<int> data;
public:
	Mosaic(CPoint<int> pos, CPoint<int> size, CPoint<int> separateSize);
	~Mosaic();

	void Draw();
	void Update(int reduceNum);
};