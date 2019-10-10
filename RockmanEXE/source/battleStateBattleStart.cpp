#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"

Battle::StateBattleStart::StateBattleStart(Battle* obj):obj(obj),count(0),imgTitle() {
	const std::string position = "Battle::StateBattleStart::StateBattleStart";
	std::string fname = def::IMAGE_FILE_PATH + "battle/�t���[��/battle_begin.png";
	LoadDivGraphWithErrorCheck(imgTitle, fname, position, 1, 3, 274, 32);
}

Battle::StateBattleStart::~StateBattleStart() {
	for( int i = 0; i < 3; i++ )
		DeleteGraph(imgTitle[i]);
}

void Battle::StateBattleStart::Draw() {
	BattleFieldMgr::GetInst()->DrawBaseFrame(BattleFieldMgr::eSTATE_BATTLE_START);
	BattleCharMgr::GetInst()->Draw();

	
	// �d�l
	//    0  - n1 : �w�i�̂ݕ`��
	//    n1 - n2 : BATTLE START�����̓o��`��
	//    n2 - n3 : BATTLE START�����̑ޏo�`��
	

	if( count > WAIT_TIME ) {
		int n = (count- WAIT_TIME ) / DRAW_DELAY;
		if( n >= 6 ) {
			return;
		}else if( n > 2 ) {
			n = 2-(n % 3);
		}
		DrawGraph(105, 125, imgTitle[n], TRUE);
	}
}

void Battle::StateBattleStart::Process() {
	// TODO(�\�E�����j�]��)
	// TODO(�v���O�����A�h�o���X)
	count++;
	if( count >= STATE_END_TIME ) {
		obj->stateMgr.ChangeNext(new Battle::StateMain(obj));
	}
}

