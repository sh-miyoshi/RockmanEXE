#pragma once

#include "battleCharBase.h"

class PlayerAnimMove: public Animation{
	int moveDirect;
public:
	PlayerAnimMove();
	~PlayerAnimMove();

	void SetMoveDirect(int direct);
	virtual void Process();
}

class PlayerAnimShot: public Animation{
public:
	PlayerAnimShot();
	~PlayerAnimShot();

	// Run damage registe, ...
	virtual void Process();
}
