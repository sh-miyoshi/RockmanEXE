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
		���b�N�}��
		HP
		Zenny

		---���---
		���b�g�[�� n��
		�r���[     n��
		�t�H���e   n��
	*/
}

bool MyInfo::Process() {
	if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ) {
		return true;
	}
	return false;
}
