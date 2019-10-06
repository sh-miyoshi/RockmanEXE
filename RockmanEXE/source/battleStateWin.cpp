#include "include.h"
#include "battle.h"

Battle::StateWin::StateWin(Battle* obj):obj(obj), imgResultFrame(-1), count(0){
	std::string fname = def::IMAGE_FILE_PATH + "battle/�t���[��/battle_result_frame.png";
	imgResultFrame = LoadGraphWithErrorCheck(fname, "Battle::StateWin::StateWin");
}

Battle::StateWin::~StateWin() {
	DeleteGraph(imgResultFrame);
}

void Battle::StateWin::Draw() {
	// ������t���[�����o�Ă���
	int x = count * 5;
	if( x > 40 )
		x = 40;
	DrawGraph(x, 45, imgResultFrame, TRUE);
}

void Battle::StateWin::Process() {
	count++;
}
