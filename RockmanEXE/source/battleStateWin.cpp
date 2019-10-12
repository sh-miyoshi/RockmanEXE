#include "include.h"
#include "battle.h"

Battle::StateWin::StateWin(Battle* obj):obj(obj), imgResultFrame(-1), count(0){
	AppLogger::Info("Change Battle State to StateWin");

	std::string fname = def::IMAGE_FILE_PATH + "battle/フレーム/battle_result_frame.png";
	imgResultFrame = LoadGraphWithErrorCheck(fname, "Battle::StateWin::StateWin");

	// バスティングレベルの決定
	// ウィルス戦
	//   ～ 5秒:	7point
	//   ～12秒:	6point
	//   ～36秒:	5point
	//   それ以降:	4point
	// ナビ戦
	//   ～30秒:	10point
	//   ～40秒:	 8point
	//   ～50秒:	 6point
	//   それ以降:	 4point
	// 攻撃を受けた回数(のけぞった回数)
	//   0回:		+1point
	//   1回:		+0point
	//   2回:		-1point
	//   3回:		-2point
	//   4回以上:	-3point
	// 移動したマス
	//   ～2マス:	1point
	//   3マス以上:	0point
	// 同時に倒す
	//   2体同時:	2point
	//   3体同時:	4point
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

	// TODO(表示するもの: デリート時間, バスティングレベル, 取得アイテム)
}

void Battle::StateWin::Process() {
	count++;
}
