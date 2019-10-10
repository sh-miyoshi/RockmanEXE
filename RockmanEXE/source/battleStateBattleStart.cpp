#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"

Battle::StateBattleStart::StateBattleStart(Battle* obj):obj(obj),count(0),imgTitle() {
	const std::string position = "Battle::StateBattleStart::StateBattleStart";
	std::string fname = def::IMAGE_FILE_PATH + "battle/フレーム/battle_begin.png";
	LoadDivGraphWithErrorCheck(imgTitle, fname, position, 1, 3, 274, 32);
}

Battle::StateBattleStart::~StateBattleStart() {
	for( int i = 0; i < 3; i++ )
		DeleteGraph(imgTitle[i]);
}

void Battle::StateBattleStart::Draw() {
	BattleFieldMgr::GetInst()->DrawBaseFrame(BattleFieldMgr::eSTATE_BATTLE_START);
	BattleCharMgr::GetInst()->Draw();

	/*
		仕様
			0  - n1 : 背景のみ描画
			n1 - n2 : BATTLE START文字の登場描画
			n2 - n3 : BATTLE START文字の退出描画
	*/

	int n = count / DRAW_DELAY;
	if( n > 2 )
		n = 2;
	DrawGraph(105, 125, imgTitle[n], TRUE);
}

void Battle::StateBattleStart::Process() {
	// TODO(ソウルユニゾン)
	// TODO(プログラムアドバンス)
	count++;
	if( count >= 3 * DRAW_DELAY + 20 )
		obj->stateMgr.ChangeNext(new Battle::StateMain(obj));
}

