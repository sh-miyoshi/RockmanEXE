#pragma once

#include <string>
#include "battleCharBase.h"

class BattlePlayer :public BattleCharBase {
public:
	BattlePlayer(std::string name, int hp, int hpMax);
	~BattlePlayer();
};

class PlayerMgr {
	std::string name;
	int hp, hpMax;

	BattlePlayer*battlePlayer;

	PlayerMgr();
	~PlayerMgr();
public:
	static PlayerMgr* GetInst() {
		static PlayerMgr inst;
		return &inst;
	}

	BattlePlayer* GetBattleChar() { return battlePlayer; }
	void InitBattleChar();
};