#include "include.h"
#include "targetSelect.h"
#include "enemy.h"

TargetSelect::TargetSelect() {

	// �����Z�b�g
	std::vector<CharInfo> targets;
	targets.push_back(CharInfo(EnemyMgr::ID_���b�g�[��, CPoint<int>(345, 120), CPoint<int>(4, 1)));
	targetInfo.push_back(TargetInfo("���b�g�[�� x1", targets));

	std::vector<std::string> names;
	for( auto t : targetInfo ) {
		names.push_back(t.name);
	}

/*
	enemies.push_back("���b�g�[�� x3");
	enemies.push_back("�r���[ x1");
	enemies.push_back("���b�g�[�� and �r���[");
	enemies.push_back("�t�H���e");
	enemies.push_back("My���");
*/

	selecter.SetData(CPoint<int>(15, 30), names, WHITE);
}

TargetSelect::~TargetSelect() {
}

void TargetSelect::Draw() {
	selecter.Draw();

	// TODO(�I�����Ă���L�����N�^�̕`��)
	DrawBox(270, 30, 420, 180, WHITE, FALSE);// �t���[���`��
	int pointer = selecter.GetPointer();
	for( auto c : targetInfo[pointer].info ) {
		c.Draw();
	}
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

std::vector<Battle::EnemyInfo> TargetSelect::GetEnemyInfo() {
	return std::vector<Battle::EnemyInfo>();// TODO
}

TargetSelect::CharInfo::CharInfo(EnemyMgr::EnemyID id, CPoint<int> viewPos, CPoint<int> battleInitPos)
	:id(id), viewPos(viewPos), battleInitPos(battleInitPos) {
}

TargetSelect::CharInfo::~CharInfo() {
}

void TargetSelect::CharInfo::Draw() {
	auto data = EnemyMgr::GetData(id);
	data->Draw(viewPos.x, viewPos.y);
}

Battle::EnemyInfo TargetSelect::CharInfo::GetInfo() {
	Battle::EnemyInfo res;
	res.id = id;
	res.pos = battleInitPos;
	return res;
}
