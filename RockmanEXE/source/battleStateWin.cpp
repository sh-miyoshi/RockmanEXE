#include "include.h"
#include "battle.h"

Battle::StateWin::StateWin(Battle* obj):obj(obj), imgResultFrame(-1), count(0){
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

	// TODO(�\���������: �f���[�g����, �o�X�e�B���O���x��, �擾�A�C�e��)
}

void Battle::StateWin::Process() {
	count++;
}
