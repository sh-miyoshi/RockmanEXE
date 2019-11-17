#pragma once

#include <memory>
#include <list>
#include "battleCharBase.h"
#include "battle.h"

class DamageData {
public:
	CPoint<int> pos;
	int power;
	int targetType;
	unsigned int endCount;
	int skillObjectID;

	DamageData();
	DamageData(CPoint<int> pos, int power, int targetType, unsigned int endCount, int skillObjectID);
	~DamageData();
};

class BattleCharMgr {
	class BattlePlayer* player;
	std::list< std::shared_ptr<BattleCharBase>> enemyList;
	std::list<int> initEnemyList;
	std::list<DamageData> damageList;

	BattleCharMgr():player(nullptr) {}
	~BattleCharMgr() {}
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

	void BattleInit(std::vector<Battle::EnemyInfo> enemies);
	void BattleEnd();

	void Draw();
	void DrawAppearScene(unsigned int nowCharNo, unsigned int count);
	RtnCode MainProcess();
	void RunAnimProc();// アニメーション処理だけ実施する

	CPoint<int> GetClosestCharPos(CPoint<int> myPos, int charType);
	CPoint<int> GetClosestCharPosWithSameLine(CPoint<int> myPos, int charType);
	std::vector<CPoint<int>> GetAllCharPos(int charType);
	unsigned int GetEnemyNum()const { return enemyList.size(); }
	std::list<int> GetInitEnemyList()const { return initEnemyList; }

	void RegisterDamage(DamageData data);
};