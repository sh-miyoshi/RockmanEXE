#pragma once

#include <string>
#include "point.h"

class Animation {
protected:
	static const unsigned int ANIM_NUM_MAX = 50;

	unsigned int count;
	unsigned int endCount;
	int image[ANIM_NUM_MAX];
	unsigned int imageNum;
	unsigned int imageDelay;
public:
	Animation();
	Animation(const Animation& data);
	~Animation();

	void LoadData(std::string fname, CPoint<unsigned int> size, CPoint<unsigned int> num, unsigned int imageDelay = 1, unsigned int extraImageNum = 0);
	void DeleteData();
	void ResetCount() { count = 0; }

	virtual void Begin() {}
	virtual void End() {}
	void Draw(int x, int y);
	bool Process();
};

enum PlayerAnimType {
	eANIM_NONE,
	eANIM_MOVE,
	eANIM_SHOT,
	eANIM_CANNON,
	eANIM_SWORD,
	eANIM_BOMB,

	eANIM_MAX
};