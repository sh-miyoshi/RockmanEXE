#include "include.h"
#include "myinfo.h"
#include "playerMgr.h"
#include "drawCharacter.h"

MyInfo::MyInfo() {
}

MyInfo::~MyInfo() {
}

void MyInfo::Draw() {
	DrawBox(20, 30, 170, 180, WHITE, FALSE);

	/* TODO
		ロックマン
		HP
		Zenny

		---戦績---
		メットール n体
		ビリー     n体
		フォルテ   n体
	*/
}

bool MyInfo::Process() {
	if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ) {
		return true;
	}
	return false;
}
