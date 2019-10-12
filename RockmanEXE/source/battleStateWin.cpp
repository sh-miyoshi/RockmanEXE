#include "include.h"
#include "battle.h"
#include "fps.h"
#include "drawCharacter.h"

Battle::StateWin::StateWin(Battle* obj):obj(obj), imgResultFrame(-1), count(0), bustingLv(0) {
	AppLogger::Info("Change Battle State to StateWin");

	std::string fname = def::IMAGE_FILE_PATH + "battle/フレーム/battle_result_frame.png";
	imgResultFrame = LoadGraphWithErrorCheck(fname, "Battle::StateWin::StateWin");

	// バスティングレベルの決定
	// ウィルス戦
	//   〜 5秒:	7point
	//   〜12秒:	6point
	//   〜36秒:	5point
	//   それ以降:	4point
	// ナビ戦
	//   〜30秒:	10point
	//   〜40秒:	 8point
	//   〜50秒:	 6point
	//   それ以降:	 4point
	// 攻撃を受けた回数(のけぞった回数)
	//   0回:		+1point
	//   1回:		+0point
	//   2回:		-1point
	//   3回:		-2point
	//   4回以上:	-3point
	// 移動したマス
	//   〜2マス:	1point
	//   3マス以上:	0point
	// 同時に倒す
	//   2体同時:	2point
	//   3体同時:	4point

	// 倒した時間によるポイント加算
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
	// 横からフレームが出てくる
	int x = count * 5;
	if( x > 40 )
		x = 40;
	DrawGraph(x, 45, imgResultFrame, TRUE);

	if( count >= VIEW_ITEM_COUNT ) {
		// デリート時間の描画
		int time = obj->mainProcCount / Fps::FPS;
		int min = time / 60;
		int sec = time % 60;
		if( min > 99 )
			min = 99;
		DrawCharacter::GetInst()->DrawNumberPadding(305, 92, min, 2);
		DrawCharacter::GetInst()->DrawString(340, 95, "：", WHITE);
		DrawCharacter::GetInst()->DrawNumberPadding(355, 92, sec, 2);

		// バスティングレベルの描画
		DrawCharacter::GetInst()->DrawNumber(365, 140, bustingLv);

		// TODO(取得アイテム)
	}
}

void Battle::StateWin::Process() {
	count++;
}
