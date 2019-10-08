#pragma once

#include <string>
#include "battleCharBase.h"
#include "chip.h"
#include "animation.h"

class BattlePlayer:public BattleCharBase {
	unsigned int chargeCount;
	unsigned int chargeMaxTime;
	unsigned int busterPower;
	int imgCharge[16];
	std::vector<ChipInfo> chipFolder;
	std::list<ChipInfo> sendChipList;

	std::shared_ptr<Animation> anim[eANIM_MAX];
public:
	BattlePlayer(std::string name, unsigned int hp, unsigned int hpMax, unsigned int busterPower, std::vector<ChipInfo> chipFolder);
	~BattlePlayer();

	virtual void Draw();
	virtual void Process();

	std::vector<ChipInfo> GetHandData(unsigned max);
	void SetSendChipList(std::list<ChipInfo> chipList);
};

class PlayerMgr {
	static const int FOLDER_NUM = 30;

	std::string name;
	unsigned int hp, hpMax;
	unsigned int busterPower;
	std::vector<ChipInfo> chipFolder;

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
