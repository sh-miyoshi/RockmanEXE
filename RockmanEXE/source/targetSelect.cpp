#include "include.h"
#include "targetSelect.h"
#include "enemy.h"

TargetSelect::TargetSelect() {

	// 情報をセット
	std::vector<CharInfo> targets;
	targets.push_back(CharInfo(EnemyMgr::ID_メットール, CPoint<int>(345, 120), CPoint<int>(4, 1)));
	targetInfo.push_back(TargetInfo("メットール x1", targets));

	std::vector<std::string> names;
	for( auto t : targetInfo ) {
		names.push_back(t.name);
	}

/*
	enemies.push_back("メットール x3");
	enemies.push_back("ビリー x1");
	enemies.push_back("メットール and ビリー");
	enemies.push_back("フォルテ");
	enemies.push_back("My情報");
*/

	selecter.SetData(CPoint<int>(15, 30), names, WHITE);
}

TargetSelect::~TargetSelect() {
}

void TargetSelect::Draw() {
	selecter.Draw();

	// TODO(選択しているキャラクタの描画)
	DrawBox(270, 30, 420, 180, WHITE, FALSE);// フレーム描画
	int pointer = selecter.GetPointer();
	for( auto c : targetInfo[pointer].info ) {
		c.Draw();
	}
}

TargetSelect::RtnCode TargetSelect::Process() {
	selecter.Process();

	// TODO(実処理は未実装)
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
