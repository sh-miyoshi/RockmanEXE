#include "include.h"
#include "battle.h"

Battle::BattleWin::BattleWin(Battle* obj):obj(obj) {
}

Battle::BattleWin::~BattleWin() {
}

void Battle::BattleWin::Draw() {
	DrawString(0,0,"You win",RED);
}

void Battle::BattleWin::Process() {
}
