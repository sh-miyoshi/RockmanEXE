#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"
#include "enemy.h"
#include "effectMgr.h"

Battle::Battle(std::vector<EnemyInfo> enemies):rtnCode(eRTN_CONTINUE), mainProcCount(0), isBoss(false) {
	BattleCharMgr::GetInst()->BattleInit(enemies);
	BattleFieldMgr::GetInst()->BattleInit();

//	stateMgr.ChangeNext(new StateBattleStart(this));// debug(Å‰‚Ìstate)
	stateMgr.ChangeNext(new StateBegin(this));
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
