#include "include.h"
#include "battleStateMain.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "battleSkillMgr.h"

BattleMain::BattleMain(Battle* obj):obj(obj) {
}

BattleMain::~BattleMain() {
}

void BattleMain::Draw() {
	BattleCharMgr::GetInst()->Draw();
	BattleSkillMgr::GetInst()->Draw();
}

void BattleMain::Process() {
	switch( BattleCharMgr::GetInst()->MainProcess() ) {
	case BattleCharMgr::eRTN_WIN:
		AppLogger::Error("–¢ŽÀ‘•");// TODO
		return;
	case BattleCharMgr::eRTN_LOSE:
		AppLogger::Error("–¢ŽÀ‘•");// TODO
		return;
	}
	BattleSkillMgr::GetInst()->Process();
}
