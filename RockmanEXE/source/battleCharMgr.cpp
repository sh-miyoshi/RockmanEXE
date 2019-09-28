#include "include.h"
#include "battleCharMgr.h"
#include "playerMgr.h"
#include "battleField.h"

void BattleCharMgr::BattleInit() {
	PlayerMgr::GetInst()->InitBattleChar();
	player = PlayerMgr::GetInst()->GetBattleChar();
}

void BattleCharMgr::BattleEnd() {
}

void BattleCharMgr::Draw() {
	player->Draw();
}

void BattleCharMgr::MainProcess() {
	player->Process();
}

CPoint<int> BattleCharMgr::GetClosestCharPos(CPoint<int> myPos, CharType charType) {
	// ç≈ëÂãóó£(x*x+y*y)+1
	static const int DIST_MAX
		= BattleField::FIELD_NUM_X * BattleField::FIELD_NUM_X
		+ BattleField::FIELD_NUM_Y * BattleField::FIELD_NUM_Y
		+ 1;

	CPoint<int> resPos(BattleField::FIELD_NUM_X+1, BattleField::FIELD_NUM_Y+1);
	int distance = DIST_MAX;
	if( charType & eCHAR_PLAYER ) {
		CPoint<int> pos = player->GetPos();
		int d = ( pos.x - myPos.x ) * ( pos.x - myPos.x ) + ( pos.y - myPos.y ) * ( pos.y - myPos.y );
		if( distance > d ) {
			resPos.x = pos.x;
			resPos.y = pos.y;
			distance = d;
		}
	}
	if( charType & eCHAR_ENEMY ) {
		AppLogger::Error("ñ¢é¿ëï");
		exit(1);
	}
	if( charType & eCHAR_OBJECT ) {
		AppLogger::Error("ñ¢é¿ëï");
	}
	return resPos;
}
