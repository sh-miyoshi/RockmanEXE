#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"

Battle::BattleChipSelect::BattleChipSelect(Battle* obj) :obj(obj), imgSelectFrame(-1){
	const std::string position = "Battle::StateChipSelect::StateChipSelect";

	std::string fname = def::IMAGE_FILE_PATH + "battle_chip_select.png";
	imgSelectFrame = LoadGraphWithErrorCheck(fname, position);
}

Battle::BattleChipSelect::~BattleChipSelect() {
	DeleteGraph(imgSelectFrame);
}

void Battle::BattleChipSelect::Draw() {
	BattleCharMgr::GetInst()->Draw();

	// todo(�J�n���ɉ�����X���C�h���Ă���悤��)
	// �I���G���A�̕`��
	DrawGraph(0, 0, imgSelectFrame, TRUE);
}

void Battle::BattleChipSelect::Process() {
}
