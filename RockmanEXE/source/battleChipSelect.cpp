#include "include.h"
#include "battle.h"
#include "battleChipSelect.h"
#include "battleCharMgr.h"

BattleChipSelect::BattleChipSelect(Battle* obj) :obj(obj), imgSelectFrame(-1){
	const std::string position = "Battle::StateChipSelect::StateChipSelect";

	std::string fname = def::IMAGE_FILE_PATH + "battle_chip_select.png";
	imgSelectFrame = LoadGraphWithErrorCheck(fname, position);
}

BattleChipSelect::~BattleChipSelect() {
	DeleteGraph(imgSelectFrame);
}

void BattleChipSelect::Draw() {
	BattleCharMgr::GetInst()->Draw();

	// todo(開始時に横からスライドしてくるように)
	// 選択エリアの描画
	DrawGraph(0, 0, imgSelectFrame, TRUE);
}

void BattleChipSelect::Process() {
}
