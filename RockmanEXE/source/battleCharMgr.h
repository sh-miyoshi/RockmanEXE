#pragma once

#include <memory>
#include <list>
#include "battleCharBase.h"

class DamageData {
public:
	CPoint<int> pos;
	int power;
	int targetType;

	DamageData();
	DamageData(CPoint<int> pos, int power, int targetType);
	~DamageData();
};

class BattleCharMgr {
	class BattlePlayer* player;
	std::list< std::shared_ptr<BattleCharBase>> enemyList;
	std::list<DamageData> damageList;

	BattleCharMgr():player(nullptr) {}
	~BattleCharMgr(){}
public:
	enum RtnCode {
		eRTN_CONTINUE,
		eRTN_WIN,
		eRTN_LOSE
	};

	static BattleCharMgr* GetInst() {
		static BattleCharMgr inst;
		return &inst;
	}

	void BattleInit(std::list< std::shared_ptr<BattleCharBase>> enemyList);
	void BattleEnd();

	void Draw();
	RtnCode MainProcess();

	CPoint<int> GetClosestCharPos(CPoint<int> myPos, int charType);
	std::vector<CPoint<int>> GetAllCharPos(int charType);

	void RegisterDamage(DamageData data);
};