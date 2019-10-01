#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "battleSkillMgr.h"

Battle::BattleMain::BattleMain(Battle* obj):obj(obj) {
}

Battle::BattleMain::~BattleMain() {
}

void Battle::BattleMain::Draw() {
	BattleCharMgr::GetInst()->Draw();
	BattleSkillMgr::GetInst()->Draw();
}

void Battle::BattleMain::Process() {
	switch( BattleCharMgr::GetInst()->MainProcess() ) {
	case BattleCharMgr::eRTN_WIN:
		obj->stateMgr.ChangeNext(new BattleWin(obj));
		return;
	case BattleCharMgr::eRTN_LOSE:
		AppLogger::Error("–¢ŽÀ‘•");// TODO
		return;
	}
	BattleSkillMgr::GetInst()->Process();
}
