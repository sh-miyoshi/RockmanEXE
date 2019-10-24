#include "include.h"
#include "targetSelect.h"

TargetSelect::TargetSelect() {
	std::vector<std::string> enemies;
	enemies.push_back("���b�g�[�� x1");
	enemies.push_back("���b�g�[�� x3");
	enemies.push_back("�r���[ x1");
	enemies.push_back("���b�g�[�� and �r���[");
	enemies.push_back("�t�H���e");
	enemies.push_back("My���");

	selecter.SetData(CPoint<int>(15,30),enemies,WHITE);
}

TargetSelect::~TargetSelect() {
}

void TargetSelect::Draw() {
	selecter.Draw();

	// TODO(�I�����Ă���L�����N�^�̕`��)
	DrawBox(270, 30, 420, 180, WHITE, FALSE);// �t���[���`��
}

TargetSelect::RtnCode TargetSelect::Process() {
	selecter.Process();

	// TODO(�������͖�����)
	if( CKey::GetInst()->CheckKey(eKEY_DEV_L) == 1 ) {
		return eRTN_MY_INFO;
	} else if( CKey::GetInst()->CheckKey(eKEY_DEV_R) == 1 ) {
		return eRTN_START_BATTLE;
	}
	return eRTN_CONTINUE;
}
