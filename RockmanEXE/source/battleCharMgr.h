#pragma once

#include <vector>
#include <list>
#include <memory>
#include <functional>
#include "player.h"
#include "enemy.h"
#include "battle.h"

class BattleCharMgr{
	int imgHpFrame;
	int imgMindStatus[MIND_MAX];
	std::vector<std::shared_ptr<BattleObject>> objects;
	std::vector<std::shared_ptr<BattleCharBase>> enemys;
	std::shared_ptr<BattlePlayer> player;// プレイヤー情報
	std::list<EnemyMgr::EnemyAttackChip_t> enemyChips;
	class BattleSkillMgr *skillMgr;

	BattleCharMgr();
	~BattleCharMgr();
public:
	static BattleCharMgr *GetInst(){
		static BattleCharMgr inst;
		return &inst;
	}

	void Init(std::list<BattleEnemyInit_t> enemyInfo);
	void End();
	void DrawObjects(int drawNum = -1, int count = 0);
	BattleRtnCode Process();

	bool IsAnyChar(CPoint<int> pos);
	int GetTargetX(int y, int target);// y座標が一致する最寄りのtarget objectのx座標
	CPoint<int> GetTargetPos(CPoint<int> mypos, def::charType target);
	int GetCondPos(CPoint<int> *result, int max, def::charType charType, std::function<bool(CPoint<int>)> cond);
	void RegisterObject(std::shared_ptr<BattleObject> object);
	void DrawPlayerState(CPoint<int> pos);
	std::vector<ChipInfo> GetPlayerHand(int max);
	void SendActionChips(std::vector<unsigned> selectedIndexes);
	void DrawPlayerSendChipIcon();
	unsigned GetEnemyNum()const{ return enemys.size(); }
	std::list<EnemyMgr::EnemyAttackChip_t> GetEnemyChips()const{ return enemyChips; }

	void RegisterAttack(std::shared_ptr<SkillData> skill);
};