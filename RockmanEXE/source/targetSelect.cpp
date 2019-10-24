#include "include.h"
#include "targetSelect.h"

TargetSelect::TargetSelect() {
	std::vector<std::string> enemies;
	enemies.push_back("メットール x1");
	enemies.push_back("メットール x3");
	enemies.push_back("ビリー x1");
	enemies.push_back("メットール and ビリー");
	enemies.push_back("フォルテ");
	enemies.push_back("My情報");

	selecter.SetData(CPoint<int>(15,30),enemies,WHITE);
}

TargetSelect::~TargetSelect() {
}

void TargetSelect::Draw() {
	selecter.Draw();

	// TODO(選択しているキャラクタの描画)
	DrawBox(270, 30, 420, 180, WHITE, FALSE);// フレーム描画
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
