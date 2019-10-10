#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"
#include "enemy.h"
#include "effectMgr.h"

Battle::Battle():rtnCode(eRTN_CONTINUE){
	// TODO(������enemyList���擾����)
	std::list< std::shared_ptr<BattleCharBase>> enemyList;

	std::shared_ptr<BattleCharBase> enemy1 = EnemyMgr::GetData(EnemyMgr::ID_�I);
	enemy1->SetPos(4, 1);
	enemyList.push_back(enemy1);
	/*std::shared_ptr<BattleCharBase> enemy1 = EnemyMgr::GetData(EnemyMgr::ID_���b�g�[��);
	enemy1->SetPos(3, 1);
	enemyList.push_back(enemy1);*/

	std::string infoMsg = "[ ";
	for(auto e : enemyList) {
		infoMsg += ToString("%s, ", e->GetName().c_str());
	}
	infoMsg += "]";
	AppLogger::Info("Battle In with Enemy List: %s", infoMsg.c_str());

	BattleCharMgr::GetInst()->BattleInit(enemyList);
	BattleFieldMgr::GetInst()->BattleInit();

	stateMgr.ChangeNext(new StateBattleStart(this));// debug(�ŏ���state)
//	stateMgr.ChangeNext(new StateBegin(this));
}

Battle::~Battle() {
	BattleCharMgr::GetInst()->BattleEnd();
	BattleFieldMgr::GetInst()->BattleEnd();
}

void Battle::Draw() {
	BattleFieldMgr::GetInst()->Draw();
	stateMgr.Draw();
	EffectMgr::GetInst()->Draw();
}

Battle::RtnCode Battle::Process() {
	BattleFieldMgr::GetInst()->Process();
	stateMgr.Process();
	EffectMgr::GetInst()->Process();
	return rtnCode;
}
