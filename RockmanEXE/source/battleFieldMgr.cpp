#include "include.h"
#include "battleFieldMgr.h"
#include "playerMgr.h"
#include "drawCharacter.h"

void BattleFieldMgr::BattleInit() {
	fieldData.LoadImages();
	fieldData.InitInBattleBegin();
	backData.LoadImages();
	gaugeCount = 0;
	drawGaugeMaxCount = 0;

	std::string position = "BattleFieldMgr::Init";
	std::string fname = def::IMAGE_FILE_PATH + "battle/�t���[��/gauge.png";
	imgGauge[0] = LoadGraphWithErrorCheck(fname, position);

	fname = def::IMAGE_FILE_PATH + "battle/�t���[��/gauge_max.png";
	LoadDivGraphWithErrorCheck(imgGauge + 1, fname, position, 1, 4, 288, 30);

	fname = def::IMAGE_FILE_PATH + "battle/�t���[��/mind_window_frame.png";
	imgMindWindowFrame = LoadGraphWithErrorCheck(fname, position);

	fname = def::IMAGE_FILE_PATH + "battle/�t���[��/hp_frame.png";
	imgHpFrame = LoadGraphWithErrorCheck(fname, position);

	int tmp[18];
	fname = def::IMAGE_FILE_PATH + "battle/�t���[��/mind_status.png";
	LoadDivGraphWithErrorCheck(tmp, fname, position, 6, 3, 88, 32);
	for( int i = 0; i < 17; i++ ) {
		imgMindStatus[i] = tmp[i];
	}
	DeleteGraph(tmp[17]);
}

void BattleFieldMgr::BattleEnd() {
	fieldData.DeleteImages();
	backData.DeleteImages();
	for( int i = 0; i < 5; i++ )
		DeleteGraph(imgGauge[i]);
	DeleteGraph(imgMindWindowFrame);
	DeleteGraph(imgHpFrame);
	for( int i = 0; i < 17; i++ ) {
		DeleteGraph(imgMindStatus[i]);
	}
}

void BattleFieldMgr::Draw() {
	backData.Draw(BattleBack::eTYPE_�H����);// debug(�}�b�v��񂩂�Ƃ��Ă���悤��)
	fieldData.Draw();
}

void BattleFieldMgr::DrawBaseFrame(BattleState state) {
	unsigned int hp = PlayerMgr::GetInst()->GetBattleChar()->GetHP();
	unsigned int hpMax = PlayerMgr::GetInst()->GetBattleChar()->GetHPMax();
	DrawCharacter::Color hp_col = ( hp < hpMax / 3 ) ? DrawCharacter::COL_RED : DrawCharacter::COL_WHITE;
	int x = 0;

	switch( state ) {
	case eSTATE_CHIP_SELECT:
		x = 242;
		break;
	case eSTATE_BATTLE_START:
	case eSTATE_MAIN:
		// �J�X�^���Q�[�W�̕`��
		if( gaugeCount < GAUGE_MAX_COUNT ) {
			DrawGraph(96, 5, imgGauge[0], TRUE);
			static const int GAUGE_MAX_SIZE = 256;
			int size = GAUGE_MAX_SIZE * gaugeCount / GAUGE_MAX_COUNT;
			DrawBox(112, 19, 112 + size, 21, GetColor(123, 154, 222), TRUE);
			DrawBox(112, 21, 112 + size, 29, GetColor(231, 235, 255), TRUE);
			DrawBox(112, 29, 112 + size, 31, GetColor(123, 154, 222), TRUE);
		} else {
			int c = ( ( drawGaugeMaxCount / 20 ) % 4 ) + 1;
			DrawGraph(96, 5, imgGauge[c], TRUE);
		}
		x = 7;
		break;
	default:
		AppLogger::Error("In DrawBaseFrame, State %d is not implemented yet.", state);
		break;
	}

	// �R�R���E�B���h�E�̕`��
	DrawGraph(x, 40, imgMindWindowFrame, TRUE);// �t���[���̕`��
	auto mind = PlayerMgr::GetInst()->GetBattleChar()->GetMindStatus();
	DrawGraph(x, 40, imgMindStatus[mind], TRUE);

	// HP�̕`��
	DrawGraph(x, 5, imgHpFrame, TRUE);
	DrawCharacter::GetInst()->DrawNumber(x + 14, 7, hp, hp_col, 4);
}

void BattleFieldMgr::Process() {
	backData.Process();
}

bool BattleFieldMgr::GaugeProcess() {
	if( gaugeCount < GAUGE_MAX_COUNT ) {
		gaugeCount += 2;// TODO(�J�X�^���Q�[�W�̏㏸���x�𒲐߂ł���悤�ɂ���)
		return false;// State���܂��X�V�ł��Ȃ�
	} else {
		drawGaugeMaxCount++;
		return true;// State���X�V�ł���
	}
}

void BattleFieldMgr::GetPanelInfo(int result[BattleField::LAYER_MAX], int x, int y) {
	fieldData.GetPanelInfo(result, x, y);
}