#pragma once

#include "state.h"

class BattleMain : public StateBase{
	class Battle* obj;
public:
	BattleMain(class Battle *obj);
	~BattleMain();

	void Draw();
	void Process();
};