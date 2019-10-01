#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "battleSkillMgr.h"

Battle::StateMain::StateMain(Battle* obj):obj(obj) {
}

Battle::StateMain::~StateMain() {
}

void Battle::StateMain::Draw() {
	BattleCharMgr::GetInst()->Draw();
	BattleSkillMgr::GetInst()->Draw();
}

void Battle::StateMain::Process() {
	switch( BattleCharMgr::GetInst()->MainProcess() ) {
	case BattleCharMgr::eRTN_WIN:
		obj->stateMgr.ChangeNext(new StateWin(obj));
		return;
	case BattleCharMgr::eRTN_LOSE:
		AppLogger::Error("������");// TODO
		return;
	}
	BattleSkillMgr::GetInst()->Process();
}
