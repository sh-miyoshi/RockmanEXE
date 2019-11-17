#pragma once

#include <vector>
#include "point.h"

class Mosaic {
	CPoint<int> size;
	std::vector< CPoint<int>> viewPos;
public:
	Mosaic(CPoint<int> leftUpPixel, CPoint<int> totalPixelSize, CPoint<int> separatePixelSize);
	~Mosaic();

	void Draw();
	void Update(unsigned int reduceNum);
};