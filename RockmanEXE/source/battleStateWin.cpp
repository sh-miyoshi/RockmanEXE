#include "include.h"
#include "battle.h"
#include "fps.h"
#include "drawCharacter.h"

Battle::StateWin::StateWin(Battle* obj):obj(obj), imgResultFrame(-1), count(0), bustingLv(0) {
	AppLogger::Info("Change Battle State to StateWin");

	std::string fname = def::IMAGE_FILE_PATH + "battle/ƒtƒŒ[ƒ€/battle_result_frame.png";
	imgResultFrame = LoadGraphWithErrorCheck(fname, "Battle::StateWin::StateWin");

	// ƒoƒXƒeƒBƒ“ƒOƒŒƒxƒ‹‚ÌŒˆ’è
	// ƒEƒBƒ‹ƒXí
	//   ` 5•b:	7point
	//   `12•b:	6point
	//   `36•b:	5point
	//   ‚»‚êˆÈ~:	4point
	// ƒiƒrí
	//   `30•b:	10point
	//   `40•b:	 8point
	//   `50•b:	 6point
	//   ‚»‚êˆÈ~:	 4point
	// UŒ‚‚ðŽó‚¯‚½‰ñ”(‚Ì‚¯‚¼‚Á‚½‰ñ”)
	//   0‰ñ:		+1point
	//   1‰ñ:		+0point
	//   2‰ñ:		-1point
	//   3‰ñ:		-2point
	//   4‰ñˆÈã:	-3point
	// ˆÚ“®‚µ‚½ƒ}ƒX
	//   `2ƒ}ƒX:	1point
	//   3ƒ}ƒXˆÈã:	0point
	// “¯Žž‚É“|‚·
	//   2‘Ì“¯Žž:	2point
	//   3‘Ì“¯Žž:	4point

	// “|‚µ‚½ŽžŠÔ‚É‚æ‚éƒ|ƒCƒ“ƒg‰ÁŽZ
	int time = obj->mainProcCount / Fps::FPS;
	bustingLv = 4;
	if( obj->isBoss ) {
		const int deadline[4] = { 50,40,30,-1 };
		for( int i = 0; i < 4; i++ ) {
			if( time > deadline[i] ) {
				bustingLv += ( i * 2 );
				break;
			}
		}
	} else {
		const int deadline[4] = { 36,12,5,-1 };
		for( int i = 0; i < 4; i++ ) {
			if( time > deadline[i] ) {
				bustingLv += i;
				break;
			}
		}
	}
}

Battle::StateWin::~StateWin() {
	DeleteGraph(imgResultFrame);
}

void Battle::StateWin::Draw() {
	// ‰¡‚©‚çƒtƒŒ[ƒ€‚ªo‚Ä‚­‚é
	int x = count * 5;
	if( x > 40 )
		x = 40;
	DrawGraph(x, 45, imgResultFrame, TRUE);

	if( count >= VIEW_ITEM_COUNT ) {
		// ƒfƒŠ[ƒgŽžŠÔ‚Ì•`‰æ
		int time = obj->mainProcCount / Fps::FPS;
		int min = time / 60;
		int sec = time % 60;
		if( min > 99 )
			min = 99;
		DrawCharacter::GetInst()->DrawNumberPadding(305, 92, min, 2);
		DrawCharacter::GetInst()->DrawString(340, 95, "F", WHITE);
		DrawCharacter::GetInst()->DrawNumberPadding(355, 92, sec, 2);

		// ƒoƒXƒeƒBƒ“ƒOƒŒƒxƒ‹‚Ì•`‰æ
		DrawCharacter::GetInst()->DrawNumber(365, 140, bustingLv);

		// TODO(Žæ“¾ƒAƒCƒeƒ€)
	}
}

void Battle::StateWin::Process() {
	count++;
	if( count >= VIEW_ITEM_COUNT ) {
		// TODO(ƒ‚ƒUƒCƒNˆ—)
		if( count >= VIEW_ITEM_COUNT + 50 ) {
			if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 )
				obj->rtnCode = Battle::eRTN_WIN;
		}
	}
}
