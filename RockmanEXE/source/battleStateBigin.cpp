#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"

Battle::StateBegin::StateBegin(Battle* obj):obj(obj), count(0), drawNo(0){

}

Battle::StateBegin::~StateBegin() {
}

void Battle::StateBegin::Draw() {
	BattleCharMgr::GetInst()->DrawAppearScene(drawNo, count);
}

void Battle::StateBegin::Process() {
	count += BEGIN_COUNTUP_VALUE;

	if( count >= 256 ) {
		count = 0;
		drawNo++;
		if( drawNo > BattleCharMgr::GetInst()->GetEnemyNum() ) {
			AppLogger::Info("Change Battle State to Chip Select");
			obj->stateMgr.ChangeNext(new Battle::StateChipSelect(obj));
		}
	}
}
