#pragma once

#include <memory>
#include <list>
#include "battleCharBase.h"

class BattleCharMgr {
	class BattlePlayer* player;
	std::list< std::shared_ptr<BattleCharBase>> enemyList;

	BattleCharMgr():player(nullptr) {}
	~BattleCharMgr(){}
public:
	static BattleCharMgr* GetInst() {
		static BattleCharMgr inst;
		return &inst;
	}

	void BattleInit(std::list< std::shared_ptr<BattleCharBase>> enemyList);
	void BattleEnd();
	void Draw();
	void MainProcess();// TODO(ñﬂÇËílÇÕçlÇ¶ÇÈ)

	CPoint<int> GetClosestCharPos(CPoint<int> myPos, CharType charType);
};