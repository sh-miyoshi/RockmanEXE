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
	void DrawAppearScene(unsigned int nowCharNo, unsigned int count);
	RtnCode MainProcess();
	void RunAnimProc();// アニメーション処理だけ実施する

	CPoint<int> GetClosestCharPos(CPoint<int> myPos, int charType);
	CPoint<int> GetClosestCharPosWithSameLine(CPoint<int> myPos, int charType);
	std::vector<CPoint<int>> GetAllCharPos(int charType);
	unsigned int GetEnemyNum()const { return enemyList.size(); }

	void RegisterDamage(DamageData data);
};