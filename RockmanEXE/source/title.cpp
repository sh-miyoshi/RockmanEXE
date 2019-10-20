#include "include.h"
#include "title.h"
#include "drawCharacter.h"
#include "playerMgr.h"

Title::Title():imgBack(-1), isFinish(false) {
	std::string fname = def::IMAGE_FILE_PATH + "title/title_back.png";
	imgBack = LoadGraphWithErrorCheck(fname, "Main::StateTitle::StateTitle");

	stateMgr.ChangeNext(new StateTitleBegin(this));
}

Title::~Title() {
	DeleteGraph(imgBack);
}

void Title::Draw() {
	DrawGraph(0, 0, imgBack, FALSE);
	stateMgr.Draw();
}

bool Title::Process() {
	stateMgr.Process();
	return isFinish;
}

Title::StateTitleBegin::StateTitleBegin(Title* obj):obj(obj),count(0) {
	fade.SetFadeIn(40);
}

Title::StateTitleBegin::~StateTitleBegin() {
}

void Title::StateTitleBegin::Draw() {
	fade.Draw();

	if( count > ALLOW_SELECT_COUNT ) {
		if( ( count / 40 ) % 2 == 0 ) {
			DrawCharacter::GetInst()->DrawStringCenter(def::FMX / 2, 260, "PLEASE PRESS ENTER", WHITE);
		}
	}
}

void Title::StateTitleBegin::Process() {
	fade.Process();
	count++;
	if( count > ALLOW_SELECT_COUNT ) {
		if( CKey::GetInst()->CheckKey(eKEY_ENTER)==1 ) {
			obj->stateMgr.ChangeNext(new StateTitleSelect(obj));
		}
	}
}


Title::StateTitleSelect::StateTitleSelect(Title* obj):obj(obj) {
	std::vector<std::string> data;
	data.push_back("はじめから");
	data.push_back("つづきから");
	selecter.SetData(CPoint<int>(180,230),data,WHITE);
	if( !PlayerMgr::GetInst()->IsContinueOK() ) {
		selecter.SetUnSelectable(1);// 続きからをできなくする
	} else {
		selecter.SetPointer(1);// デフォルトでつづきからを選択
	}
}

Title::StateTitleSelect::~StateTitleSelect() {
}

void Title::StateTitleSelect::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);// アルファブレンド
	DrawBox(0, 0, def::FMX, def::FMY, BLACK, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);// ノーマルブレンド

	selecter.Draw();
}

void Title::StateTitleSelect::Process() {
	switch(selecter.Process()) {
	case 0:// はじめから
		PlayerMgr::GetInst()->CreateNewPlayer();
		obj->isFinish = true;
		break;
	case 1:// つづきから
		AppLogger::Error("未実装");
		break;
	}
}
