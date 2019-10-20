#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"
#include "enemy.h"
#include "effectMgr.h"

Battle::Battle(std::vector<EnemyInfo> enemies):rtnCode(eRTN_CONTINUE), mainProcCount(0), isBoss(false) {
	std::list< std::shared_ptr<BattleCharBase>> enemyList;
	std::string infoMsg = "[ ";

	for( auto e : enemies ) {
		std::shared_ptr<BattleCharBase> enemy = EnemyMgr::GetData(e.id);
		infoMsg += ToString("%s, ", enemy->GetName().c_str());
		enemy->SetPos(e.pos.x, e.pos.y);
		enemyList.push_back(enemy);
	}
	infoMsg += "]";
	AppLogger::Info("Battle In with Enemy List: %s", infoMsg.c_str());

	BattleCharMgr::GetInst()->BattleInit(enemyList);
	BattleFieldMgr::GetInst()->BattleInit();

	stateMgr.ChangeNext(new StateBattleStart(this));// debug(Å‰‚Ìstate)
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
