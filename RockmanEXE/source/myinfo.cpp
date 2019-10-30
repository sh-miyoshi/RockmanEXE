#include "include.h"
#include "myinfo.h"
#include "playerMgr.h"
#include "drawCharacter.h"
#include "enemy.h"

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

	DrawCharacter::GetInst()->DrawString(def::FMX / 2 + 40, 30, "---戦績---", WHITE);
	DrawCharacter::GetInst()->DrawString(def::FMX / 2 + 20, 70, ToString("%s x体", EnemyMgr::GetData(EnemyMgr::ID_メットール)->GetName().c_str()), WHITE);
}

bool MyInfo::Process() {
	if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ) {
		return true;
	}
	return false;
}
