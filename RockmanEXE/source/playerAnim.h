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
