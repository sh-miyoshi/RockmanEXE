#pragma once

#include <string>
#include "battleCharBase.h"

class BattlePlayer:public BattleCharBase {
	unsigned int chargeCount;
	unsigned int chargeMaxTime;
	unsigned int busterPower;
	int imgCharge[16];

	std::shared_ptr<Animation> animMove;
	std::shared_ptr<Animation> animShot;
	std::shared_ptr<Animation> animCannon;
	std::shared_ptr<Animation> animSword;
	std::shared_ptr<Animation> animBomb;
public:
	BattlePlayer(std::string name, unsigned int hp, unsigned int hpMax, unsigned int busterPower);
	~BattlePlayer();

	virtual void Draw();
	virtual void Process();
};

class PlayerMgr {
	std::string name;
	unsigned int hp, hpMax;
	unsigned int busterPower;

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
