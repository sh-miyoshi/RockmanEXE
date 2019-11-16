#pragma once

#include <vector>
#include "point.h"
#include "define.h"

class GameOver {
	static const int OFFSET_UP = 8;
	static const int BACK_BOX_SIZE_X = 16;
	static const int BACK_BOX_SIZE_Y = 16;
	static const int ABS_BOX_RANGE_SIZE = def::FMX * 2;

	std::vector<CPoint<int>> back_box_pos[19][3];
	unsigned int count;
	int backImage;

	void ReshapePosition();// À•W‚ª”ÍˆÍ“à‚Éû‚Ü‚é‚æ‚¤‚É®Œ`
	void BoxDraw();// ”ÍˆÍ“à‚Ìbox‚¾‚¯•`‰æ‚·‚é
public:
	GameOver();
	~GameOver();

	void Draw();
	bool Process();
};