#include "include.h"
#include "battle.h"
#include "main.h"
#include "battleMain.h"
#include "playerMgr.h"
#include "battleFieldMgr.h"

Battle::Battle(Main* obj) :obj(obj){
	PlayerMgr::GetInst()->InitBattleChar();
	stateMgr.ChangeNext(new BattleMain(this));
	BattleFieldMgr::GetInst()->BattleInit();
}

Battle::~Battle() {
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
