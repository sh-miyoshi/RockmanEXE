#include "include.h"
#include "battle.h"
#include "main.h"
#include "battleMain.h"

Battle::Battle(Main* obj) :obj(obj){
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
