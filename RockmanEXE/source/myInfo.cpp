#include "include.h"
#include "myInfo.h"
#include "player.h"
#include "drawCharacter.h"

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
	selectMgr.Create(50, 50, SELECTS);
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
	int t[4];
	LoadDivGraphWithCheckError(t, def::IMAGE_FILE_PATH + "player_move.png", "MyInfo::StateRockman::StateRockman", 4, 1, 100, 100);
	image = t[0];
	for( int i = 1; i < 4; i++ )
		DeleteGraph(t[i]);
}

MyInfo::StateRockman::~StateRockman(){
	DeleteGraph(image);
}

void MyInfo::StateRockman::Draw(){
	int hp = PlayerMgr::GetInst()->GetHp();
	int hpMax = PlayerMgr::GetInst()->GetHpMax();
	int zenny = PlayerMgr::GetInst()->GetMoney();

	const std::string DATA[] = {
		ToString("ロックマン"),
		ToString("HP   %4d / %4d", hp, hpMax),
		ToString("ZENNY    %7d", zenny),
	};

	DrawBox(300, 50, 400, 150, WHITE, FALSE);
	DrawRotaGraph(350, 100, 1, 0, image, TRUE);

	for( unsigned i = 0; i < sizeof(DATA) / sizeof(DATA[0]); i++ ){
		DrawCharacter::GetInst()->DrawString(100, 50 + i * 30, DATA[i], WHITE);
	}
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
	DrawCharacter::GetInst()->DrawStringCenter(def::FMX / 2, 50, "--戦績--", WHITE);
	DrawCharacter::GetInst()->DrawString(250, 80, "勝利回数  敗北回数", WHITE);
	for( int i = 0; i < sizeof(SELECT_NAMES) / sizeof(SELECT_NAMES[0]); i++ ){
		DrawCharacter::GetInst()->DrawString(50, 110 + i * 30, SELECT_NAMES[i].c_str(), WHITE);
		int win = PlayerMgr::GetInst()->GetBattleResult(PlayerMgr::eBT_RTN_WIN, i);
		int lose = PlayerMgr::GetInst()->GetBattleResult(PlayerMgr::eBT_RTN_LOSE, i);
		DrawCharacter::GetInst()->DrawString(280, 110 + i * 30, ToString(" % 4d", win), WHITE);
		DrawCharacter::GetInst()->DrawString(365, 110 + i * 30, ToString(" % 4d", lose), WHITE);
	}
}

void MyInfo::StateLog::Process(){
	if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ||
		CKey::GetInst()->CheckKey(eKEY_ENTER) == 1
		){
		obj->stateMgr.ChangeNext(new StateTop(obj));
	}
}

