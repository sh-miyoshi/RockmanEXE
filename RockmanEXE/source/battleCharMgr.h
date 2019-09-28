#pragma once

#include "battleCharBase.h"

class BattleCharMgr {
	class BattlePlayer* player;

	BattleCharMgr():player(nullptr) {}
	~BattleCharMgr(){}
public:
	static BattleCharMgr* GetInst() {
		static BattleCharMgr inst;
		return &inst;
	}

	void BattleInit();
	void BattleEnd();
	void Draw();
	void MainProcess();// TODO(ñﬂÇËílÇÕçlÇ¶ÇÈ)

	CPoint<int> GetClosestCharPos(CPoint<int> myPos, CharType charType);
};