#pragma once

#include <string>
#include "battleCharBase.h"

class BattlePlayer :public BattleCharBase {
	std::shared_ptr<Animation> animMove;
public:
	BattlePlayer(std::string name, unsigned int hp, unsigned int hpMax, std::shared_ptr<Animation> defaultAnim);
	~BattlePlayer();

	virtual void Process();
};

class PlayerMgr {
	std::string name;
	unsigned int hp, hpMax;

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

	void InitPlayer();
	// TODO void LoadSaveFile();
	// TODO Save()
};
