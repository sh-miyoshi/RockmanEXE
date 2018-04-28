#include "include.h"
#include "myInfo.h"
#include "player.h"

MyInfo::MyInfo():isFinish(false){
	stateMgr.ChangeNext(new StateTop(this));
}

MyInfo::~MyInfo(){
}

void MyInfo::Draw(){
	stateMgr.Draw();
}

bool MyInfo::Process(){
	stateMgr.Process();
	return isFinish;
}

MyInfo::StateTop::StateTop(MyInfo * obj):obj(obj){
	const std::list<std::string> SELECTS = {
		"ロックマン",
		"戦績",
		"終了"
	};
	selectMgr.Create(0, 0, SELECTS);
	selectMgr.SetColor(SelectColor{ WHITE,WHITE });
}

MyInfo::StateTop::~StateTop(){
}

void MyInfo::StateTop::Draw(){
	selectMgr.Draw();
}

void MyInfo::StateTop::Process(){
	switch( selectMgr.Process() ){
	case 0:// ロックマン
		obj->stateMgr.ChangeNext(new StateRockman(obj));
		break;
	case 1:// 戦績
		obj->stateMgr.ChangeNext(new StateLog(obj));
		break;
	case 2:// 終了
		obj->isFinish = true;
		break;
	}
	if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ){
		obj->isFinish = true;
	}
}

MyInfo::StateRockman::StateRockman(MyInfo * obj):obj(obj){
}

MyInfo::StateRockman::~StateRockman(){
}

void MyInfo::StateRockman::Draw(){
	// todo
	DrawStringCenter(def::FMX / 2, def::FMY / 2 - 10, "This Function is not implemented now.", WHITE);
}

void MyInfo::StateRockman::Process(){
	if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ||
		CKey::GetInst()->CheckKey(eKEY_ENTER) == 1
		){
		obj->stateMgr.ChangeNext(new StateTop(obj));
	}
}

MyInfo::StateLog::StateLog(MyInfo * obj):obj(obj){
}

MyInfo::StateLog::~StateLog(){
}

void MyInfo::StateLog::Draw(){
	static const std::string SELECT_NAMES[def::eKIND_MAX - 1] = {
	"メットール x1        ",
	"メットール x3        ",
	"ビリー x1            ",
	"メットール and ビリー",
	"フォルテ             ",
	};

	// 戦績 (勝利回数・敗北回数)
	DrawStringCenter(def::FMX / 2, 50, "--戦績--", WHITE);
	DrawString(250, 80, "勝利回数  敗北回数", WHITE);
	for( int i = 0; i < sizeof(SELECT_NAMES) / sizeof(SELECT_NAMES[0]); i++ ){
		DrawString(50, 110 + i * 30, SELECT_NAMES[i].c_str(), WHITE);
		int win = PlayerMgr::GetInst()->GetBattleResult(PlayerMgr::eBT_RTN_WIN, i);
		int lose = PlayerMgr::GetInst()->GetBattleResult(PlayerMgr::eBT_RTN_LOSE, i);
		DrawFormatString(280, 110 + i * 30, WHITE, "%4d", win);
		DrawFormatString(365, 110 + i * 30, WHITE, "%4d", lose);
	}
}

void MyInfo::StateLog::Process(){
	if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ||
		CKey::GetInst()->CheckKey(eKEY_ENTER) == 1
		){
		obj->stateMgr.ChangeNext(new StateTop(obj));
	}
}

