#pragma once

#include "state.h"

class BattleChipSelect: public StateBase {
	int imgSelectFrame;
	class Battle* obj;
public:
	BattleChipSelect(class Battle* obj);
	~BattleChipSelect();

	void Draw();
	void Process();
};