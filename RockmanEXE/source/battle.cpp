#include "include.h"
#include "battle.h"
#include "main.h"
#include "battleMain.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"

Battle::Battle(Main* obj) :obj(obj){
	BattleCharMgr::GetInst()->BattleInit();
	BattleFieldMgr::GetInst()->BattleInit();
	stateMgr.ChangeNext(new BattleMain(this));// debug(Å‰‚Ìstate)
}

Battle::~Battle() {
	BattleCharMgr::GetInst()->BattleEnd();
	BattleFieldMgr::GetInst()->BattleEnd();
}

void Battle::Draw() {
	BattleFieldMgr::GetInst()->Draw();
	stateMgr.Draw();
}

void Battle::Process() {
	BattleFieldMgr::GetInst()->Process();
	stateMgr.Process();
}
