#include "include.h"
#include "battle.h"
#include "main.h"
#include "battleMain.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"
#include "enemy.h"

Battle::Battle(Main* obj) :obj(obj){
	// TODO(引数でenemyListを取得する)
	std::list< std::shared_ptr<BattleCharBase>> enemyList;
	std::shared_ptr<BattleCharBase> enemy1 = EnemyMgr::GetData(EnemyMgr::ID_メットール);
	enemy1->SetPos(3,1);
	enemyList.push_back(enemy1);

	BattleCharMgr::GetInst()->BattleInit(enemyList);
	BattleFieldMgr::GetInst()->BattleInit();
	stateMgr.ChangeNext(new BattleMain(this));// debug(最初のstate)
}

Battle::~Battle() {
	BattleCharMgr::GetInst()->BattleEnd();
	BattleFieldMgr::GetInst()->BattleEnd();
}

void Battle::Draw() {
	BattleFieldMgr::GetInst()->Draw();
	stateMgr.Draw();
}

void Battle::Process() {
	BattleFieldMgr::GetInst()->Process();
	stateMgr.Process();
}
