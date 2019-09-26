#pragma once

#include <string>
#include "point.h"

class Animation {
protected:
	static const unsigned int ANIM_NUM_MAX = 50;

	unsigned int count;
	int image[ANIM_NUM_MAX];
	unsigned int imageNum;
	unsigned int imageDelay;
	std::string imageFileName;
public:
	Animation();
	Animation(const Animation& data);
	~Animation();

	void LoadData(std::string fname, CPoint<unsigned int> size, CPoint<unsigned int> num, unsigned int imageDelay = 1);
	void DeleteData();

	virtual void Begin() {}
	virtual void End() {}
	void Draw(int x, int y);
	virtual bool Process();
};

class AnimMove: public Animation {
	def::Muki moveDirect;
public:
	AnimMove();
	~AnimMove();

	void SetMoveDirect(def::Muki direct);
	virtual bool Process();
};

//class PlayerAnimShot: public Animation {
//public:
//	PlayerAnimShot();
//	~PlayerAnimShot();
//
//	// Run damage register, ...
//	virtual void Process();
//};
