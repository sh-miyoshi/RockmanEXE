#include "include.h"
#include "battleMain.h"
#include "battle.h"
#include "playerMgr.h"

BattleMain::BattleMain(Battle* obj):obj(obj) {
}

BattleMain::~BattleMain() {
}

void BattleMain::Draw() {
	PlayerMgr::GetInst()->GetBattleChar()->Draw();
}

void BattleMain::Process() {
}
