#include "include.h"
#include "battleMain.h"
#include "battle.h"
#include "battleCharMgr.h"

BattleMain::BattleMain(Battle* obj):obj(obj) {
}

BattleMain::~BattleMain() {
}

void BattleMain::Draw() {
	BattleCharMgr::GetInst()->Draw();
}

void BattleMain::Process() {
	BattleCharMgr::GetInst()->MainProcess();
}
