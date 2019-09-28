#pragma once

#include <string>
#include "battleCharBase.h"

class BattlePlayer:public BattleCharBase {
	unsigned int chargeCount;
	unsigned int chargeMaxTime;
	int imgCharge[16];

	std::shared_ptr<Animation> animMove;
	std::shared_ptr<Animation> animShot;
public:
	BattlePlayer(std::string name, unsigned int hp, unsigned int hpMax);
	~BattlePlayer();

	virtual void Draw();
	virtual void Process();
};

class PlayerMgr {
	std::string name;
	unsigned int hp, hpMax;

	BattlePlayer* battlePlayer;

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
