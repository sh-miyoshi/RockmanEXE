#include "include.h"
#include "title.h"
#include "drawCharacter.h"

Title::Title():imgBack(-1),rtnCode(eRTN_CONTINUE) {
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

Title::RtnCode Title::Process() {
	stateMgr.Process();
	return rtnCode;
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
}

Title::StateTitleSelect::~StateTitleSelect() {
}

void Title::StateTitleSelect::Draw() {
}

void Title::StateTitleSelect::Process() {
}
