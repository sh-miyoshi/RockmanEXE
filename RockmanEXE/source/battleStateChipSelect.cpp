#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"

Battle::StateChipSelect::StateChipSelect(Battle* obj) :obj(obj), imgSelectFrame(-1){
	const std::string position = "Battle::StateChipSelect::StateChipSelect";

	std::string fname = def::IMAGE_FILE_PATH + "battle/フレーム/battle_chip_select.png";
	imgSelectFrame = LoadGraphWithErrorCheck(fname, position);
}

Battle::StateChipSelect::~StateChipSelect() {
	DeleteGraph(imgSelectFrame);
}

void Battle::StateChipSelect::Draw() {
	BattleCharMgr::GetInst()->Draw();

	// todo(開始時に横からスライドしてくるように)
	// 選択エリアの描画
	DrawGraph(0, 0, imgSelectFrame, TRUE);
}

void Battle::StateChipSelect::Process() {
}
