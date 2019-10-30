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
		���b�N�}��
		HP
		Zenny

		---���---
		���b�g�[�� n��
		�r���[     n��
		�t�H���e   n��
	*/

	DrawCharacter::GetInst()->DrawString(def::FMX / 2 + 40, 30, "---���---", WHITE);
	DrawCharacter::GetInst()->DrawString(def::FMX / 2 + 20, 70, ToString("%s x��", EnemyMgr::GetData(EnemyMgr::ID_���b�g�[��)->GetName().c_str()), WHITE);
}

bool MyInfo::Process() {
	if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ) {
		return true;
	}
	return false;
}
