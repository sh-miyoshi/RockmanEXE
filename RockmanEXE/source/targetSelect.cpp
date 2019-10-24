#include "include.h"
#include "targetSelect.h"

TargetSelect::TargetSelect() {
}

TargetSelect::~TargetSelect() {
}

void TargetSelect::Draw() {
	// TODO(ŽÀˆ—‚Í–¢ŽÀ‘•)
	DrawString(100, 100, "TargetSelect", WHITE);
	DrawString(120, 140, "A: MyInfo", WHITE);
	DrawString(120, 120, "S: Battle", WHITE);
}

TargetSelect::RtnCode TargetSelect::Process() {
	// TODO(ŽÀˆ—‚Í–¢ŽÀ‘•)
	if( CKey::GetInst()->CheckKey(eKEY_DEV_L) == 1 ) {
		return eRTN_MY_INFO;
	} else if( CKey::GetInst()->CheckKey(eKEY_DEV_R) == 1 ) {
		return eRTN_START_BATTLE;
	}
	return eRTN_CONTINUE;
}
