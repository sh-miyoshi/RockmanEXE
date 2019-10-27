#include "include.h"
#include "targetSelect.h"
#include "enemy.h"
#include "drawCharacter.h"

TargetSelect::TargetSelect():selectMaxNum(0),selectNo(-1){

	// 情報をセット
	std::vector<CharInfo> targets;

	targets.push_back(CharInfo(EnemyMgr::ID_メットール, CPoint<int>(345, 120), CPoint<int>(4, 1)));
	targetInfo.push_back(TargetInfo("メットール x1", targets));
	targets.clear();

	targets.push_back(CharInfo(EnemyMgr::ID_メットール, CPoint<int>(345, 90), CPoint<int>(5, 1)));
	targets.push_back(CharInfo(EnemyMgr::ID_メットール, CPoint<int>(315, 120), CPoint<int>(3, 0)));
	targets.push_back(CharInfo(EnemyMgr::ID_メットール, CPoint<int>(375, 120), CPoint<int>(3, 2)));
	targetInfo.push_back(TargetInfo("メットール x3", targets));
	targets.clear();

	std::vector<std::string> names;
	for( auto t : targetInfo ) {
		names.push_back(t.name);
	}
	names.push_back("My情報");

/*
	enemies.push_back("ビリー x1");
	enemies.push_back("メットール and ビリー");
	enemies.push_back("フォルテ");
	enemies.push_back("My情報");
*/

	selectMaxNum = ( int ) names.size();
	selecter.SetData(CPoint<int>(15, 30), names, WHITE);
}

TargetSelect::~TargetSelect() {
}

void TargetSelect::Draw() {
	selecter.Draw();

	//選択しているキャラクタの描画
	DrawBox(270, 30, 420, 180, WHITE, FALSE);// フレーム描画
	int pointer = selecter.GetPointer();
	if( pointer < selectMaxNum - 1 ) {
		for( auto c : targetInfo[pointer].info ) {
			c.Draw();
		}
	} else {// 最後は"My情報"
		DrawCharacter::GetInst()->DrawStringCenter(345,90,"My情報",WHITE);
	}
}

TargetSelect::RtnCode TargetSelect::Process() {
	int s=selecter.Process();
	if( s >= 0 ) {
		if( s == ( selectMaxNum - 1 ) ) {
			return eRTN_MY_INFO;
		} else {
			selectNo = s;
			return eRTN_START_BATTLE;
		}
	}
	return eRTN_CONTINUE;
}

std::vector<Battle::EnemyInfo> TargetSelect::GetEnemyInfo() {
	// 選択している敵情報を返す
	std::vector<Battle::EnemyInfo> res;
	if( selectNo <= 0 ) {
		AppLogger::Warn("まだターゲットが選択されていないのに敵情報を取得しようとしました");
		return res;
	}
	for( auto c : targetInfo[selectNo].info ) {
		res.push_back(c.GetInfo());
	}
	return res;
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
