#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"

unsigned int count;
Battle::StateLose::StateLose(Battle* obj):obj(obj),count(0) {
	AppLogger::Info("Change Battle State to StateLose");
}

Battle::StateLose::~StateLose() {
}

void Battle::StateLose::Draw() {
	BattleCharMgr::GetInst()->Draw();
}

void Battle::StateLose::Process() {
	BattleCharMgr::GetInst()->RunAnimProc();

	count++;
	if( count >= END_TIME ) {
		obj->rtnCode = Battle::eRTN_LOSE;
	}
}
