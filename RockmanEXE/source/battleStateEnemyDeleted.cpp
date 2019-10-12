#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"

Battle::StateEnemyDeleted::StateEnemyDeleted(Battle* obj):obj(obj), count(0), imgTitle() {
	AppLogger::Info("Change Battle State to StateEnemyDeleted");

	const std::string position = "Battle::StateEnemyDeleted::StateEnemyDeleted";
	std::string fname = def::IMAGE_FILE_PATH + "battle/フレーム/enemy_deleted.png";
	LoadDivGraphWithErrorCheck(imgTitle, fname, position, 1, 3, 272, 32);
}

Battle::StateEnemyDeleted::~StateEnemyDeleted() {
	for( int i = 0; i < 3; i++ )
		DeleteGraph(imgTitle[i]);
}

void Battle::StateEnemyDeleted::Draw() {
	BattleFieldMgr::GetInst()->DrawBaseFrame(BattleFieldMgr::eSTATE_BATTLE_START);
	BattleCharMgr::GetInst()->Draw();

	if( count > INIT_WAIT_TIME ) {
		int n = ( count - INIT_WAIT_TIME ) / (DRAW_DELAY+3);
		if( n >= 6 ) {
			return;
		} else if( n > 2 ) {
			n = 2 - ( n % 3 );
		}
		DrawGraph(105, 125, imgTitle[n], TRUE);
	}
}

void Battle::StateEnemyDeleted::Process() {
	BattleCharMgr::GetInst()->RunAnimProc();// 仕掛中のアニメーションだけは終わらせる(ダメージ計算などはなし)

	count++;
	if( count == STATE_END_TIME ) {
		obj->stateMgr.ChangeNext(new Battle::StateWin(obj));
	}
}

