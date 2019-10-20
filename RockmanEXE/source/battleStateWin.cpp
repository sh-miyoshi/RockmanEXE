#include "include.h"
#include "battle.h"
#include "fps.h"
#include "drawCharacter.h"

Battle::StateWin::StateWin(Battle* obj):obj(obj), imgResultFrame(-1), count(0), bustingLv(0) {
	AppLogger::Info("Change Battle State to StateWin");

	std::string fname = def::IMAGE_FILE_PATH + "battle/�t���[��/battle_result_frame.png";
	imgResultFrame = LoadGraphWithErrorCheck(fname, "Battle::StateWin::StateWin");

	// �o�X�e�B���O���x���̌���
	// �E�B���X��
	//   �` 5�b:	7point
	//   �`12�b:	6point
	//   �`36�b:	5point
	//   ����ȍ~:	4point
	// �i�r��
	//   �`30�b:	10point
	//   �`40�b:	 8point
	//   �`50�b:	 6point
	//   ����ȍ~:	 4point
	// �U�����󂯂���(�̂���������)
	//   0��:		+1point
	//   1��:		+0point
	//   2��:		-1point
	//   3��:		-2point
	//   4��ȏ�:	-3point
	// �ړ������}�X
	//   �`2�}�X:	1point
	//   3�}�X�ȏ�:	0point
	// �����ɓ|��
	//   2�̓���:	2point
	//   3�̓���:	4point

	// �|�������Ԃɂ��|�C���g���Z
	int time = obj->mainProcCount / Fps::FPS;
	bustingLv = 4;
	if( obj->isBoss ) {
		const int deadline[4] = { 50,40,30,-1 };
		for( int i = 0; i < 4; i++ ) {
			if( time > deadline[i] ) {
				bustingLv += ( i * 2 );
				break;
			}
		}
	} else {
		const int deadline[4] = { 36,12,5,-1 };
		for( int i = 0; i < 4; i++ ) {
			if( time > deadline[i] ) {
				bustingLv += i;
				break;
			}
		}
	}
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

	if( count >= VIEW_ITEM_COUNT ) {
		// �f���[�g���Ԃ̕`��
		int time = obj->mainProcCount / Fps::FPS;
		int min = time / 60;
		int sec = time % 60;
		if( min > 99 )
			min = 99;
		DrawCharacter::GetInst()->DrawNumberPadding(305, 92, min, 2);
		DrawCharacter::GetInst()->DrawString(340, 95, "�F", WHITE);
		DrawCharacter::GetInst()->DrawNumberPadding(355, 92, sec, 2);

		// �o�X�e�B���O���x���̕`��
		DrawCharacter::GetInst()->DrawNumber(365, 140, bustingLv);

		// TODO(�擾�A�C�e��)
	}
}

void Battle::StateWin::Process() {
	count++;
	if( count >= VIEW_ITEM_COUNT ) {
		// TODO(���U�C�N����)
		if( count >= VIEW_ITEM_COUNT + 50 ) {
			if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 )
				obj->rtnCode = Battle::eRTN_WIN;
		}
	}
}
