#pragma once

#include "fade.h"
#include "selecter.h"

class TargetSelect {
public:
	enum RtnCode {
		eRTN_CONTINUE,
		eRTN_START_BATTLE,
		eRTN_MY_INFO
	};

	TargetSelect();
	~TargetSelect();

	void Draw();
	RtnCode Process();
	// TODO(GetEnemyInfo())
};