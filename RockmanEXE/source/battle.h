#pragma once

#include "state.h"

class Battle :public StateBase{
	class Main* obj;
	StateMgr stateMgr;
public:
	Battle(class Main *obj);
	~Battle();

	void Draw();
	void Process();
};