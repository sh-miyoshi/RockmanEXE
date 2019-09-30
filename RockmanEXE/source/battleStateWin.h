#pragma once

#include "state.h"

class BattleWin: public StateBase {
	class Battle* obj;
public:
	BattleWin(class Battle* obj);
	~BattleWin();

	void Draw();
	void Process();
};