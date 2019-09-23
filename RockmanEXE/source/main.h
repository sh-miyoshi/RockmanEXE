#pragma once

#include "fps.h"

class Main {
	Fps fps;
	StateMgr stateMgr;
public:
	Main();
	~Main();

	void Process();
};