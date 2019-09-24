#include "include.h"
#include "battle.h"
#include "main.h"
#include "battleMain.h"
#include "playerMgr.h"

Battle::Battle(Main* obj) :obj(obj){
	PlayerMgr::GetInst()->InitBattleChar();
	stateMgr.ChangeNext(new BattleMain(this));
}

Battle::~Battle() {
}

void Battle::Draw() {
	stateMgr.Draw();
}

void Battle::Process() {
	stateMgr.Process();
}
