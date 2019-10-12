#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "battleSkillMgr.h"
#include "battleFieldMgr.h"

Battle::StateMain::StateMain(Battle* obj):obj(obj) {
	AppLogger::Info("Change Battle State to StateMain");
}

Battle::StateMain::~StateMain() {
}

void Battle::StateMain::Draw() {
	BattleFieldMgr::GetInst()->DrawBaseFrame(BattleFieldMgr::eSTATE_MAIN);
	BattleCharMgr::GetInst()->Draw();
	BattleSkillMgr::GetInst()->Draw();
}

void Battle::StateMain::Process() {
	obj->mainProcCount++;

	switch( BattleCharMgr::GetInst()->MainProcess() ) {
	case BattleCharMgr::eRTN_WIN:
		obj->stateMgr.ChangeNext(new StateEnemyDeleted(obj));
		return;
	case BattleCharMgr::eRTN_LOSE:
		AppLogger::Error("未実装");// TODO
		return;
	}
	BattleSkillMgr::GetInst()->Process();
	bool allowStateUpdate = BattleFieldMgr::GetInst()->GaugeProcess();
	if( allowStateUpdate ) {
		if( CKey::GetInst()->CheckKey(eKEY_DEV_L) == 1 || CKey::GetInst()->CheckKey(eKEY_DEV_R) == 1 ) {
			BattleFieldMgr::GetInst()->ResetGaugeCount();
			obj->stateMgr.ChangeNext(new StateChipSelect(obj));// チップ選択へ
		}
	}
}
