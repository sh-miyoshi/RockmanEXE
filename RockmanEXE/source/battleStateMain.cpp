#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "battleSkillMgr.h"
#include "battleFieldMgr.h"

Battle::StateMain::StateMain(Battle* obj):obj(obj) {
}

Battle::StateMain::~StateMain() {
}

void Battle::StateMain::Draw() {
	BattleFieldMgr::GetInst()->DrawBaseFrame(BattleFieldMgr::eSTATE_MAIN);
	BattleCharMgr::GetInst()->Draw();
	BattleSkillMgr::GetInst()->Draw();
}

void Battle::StateMain::Process() {
	switch( BattleCharMgr::GetInst()->MainProcess() ) {
	case BattleCharMgr::eRTN_WIN:
		AppLogger::Info("Change Battle State to StateWin");
		obj->stateMgr.ChangeNext(new StateWin(obj));
		return;
	case BattleCharMgr::eRTN_LOSE:
		AppLogger::Error("–¢ŽÀ‘•");// TODO
		return;
	}
	BattleSkillMgr::GetInst()->Process();
	BattleFieldMgr::GetInst()->GaugeProcess();
}
