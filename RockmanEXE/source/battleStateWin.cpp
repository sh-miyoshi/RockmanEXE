#include "include.h"
#include "battle.h"
#include "fps.h"
#include "drawCharacter.h"
#include "battleCharMgr.h"

Battle::StateWin::StateWin(Battle* obj):obj(obj), imgResultFrame(-1), count(0), bustingLv(0), imgZenny(-1) {
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

	// 倒した時間によるポイント加算
	int time = obj->mainProcCount / Fps::FPS;
	bustingLv = 4;
	if( obj->isBoss ) {
		const int deadline[4] = { 50, 40, 30, -1 };
		for( int i = 0; i < 4; i++ ) {
			if( time > deadline[i] ) {
				bustingLv += ( i * 2 );
				break;
			}
		}
	} else {
		const int deadline[4] = { 36, 12, 5, -1 };
		for( int i = 0; i < 4; i++ ) {
			if( time > deadline[i] ) {
				bustingLv += i;
				break;
			}
		}
	}

	// TODO(それ以外の加算)

	// 取得リソース(お金, チップなど)の設定
	std::vector<ChipInfo> chips;
	for( auto id : BattleCharMgr::GetInst()->GetInitEnemyList() ) {
		auto cs = EnemyMgr::GetChips(id, bustingLv);
		for( auto c : cs ) {
			chips.push_back(c);
		}
	}

	// debug(報酬の重みづけ)
	unsigned int rnd = rnd_generator() % ( chips.size() + 1 );
	if( rnd == chips.size() ) {	// 報酬はお金
		std::string fname = def::IMAGE_FILE_PATH + "battle/zenny.png";
		imgZenny = LoadGraphWithErrorCheck(fname, "Battle::StateWin::StateWin");

		getItem.type = ResultValue::eTYPE_MONEY;
		getItem.name = "ゼニー";
		getItem.image = imgZenny;
		switch( bustingLv ) {
		case  4: getItem.value = 30; break;
		case  5: getItem.value = 50; break;
		case  6: getItem.value = 100; break;
		case  7: getItem.value = 200; break;
		case  8: getItem.value = 400; break;
		case  9: getItem.value = 500; break;
		case 10: getItem.value = 1000; break;
		default:
			getItem.value = 2000;
			break;
		}
	} else {// 報酬はチップ
		getItem.type = ResultValue::eTYPE_CHIP;
		auto c = ChipMgr::GetInst()->GetChipData(chips[rnd].id);
		getItem.name = c.name;
		getItem.image = c.imgInfo;
		getItem.value = ( int ) chips[rnd].code;
	}

	if( getItem.name.empty() ) {
		AppLogger::Error("Logic Error: 報酬がセットされていません");
	}
}

Battle::StateWin::~StateWin() {
	DeleteGraph(imgResultFrame);
	DeleteGraph(imgZenny);
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

		// 取得アイテムの描画
		DrawGraph(268, 189, getItem.image, TRUE);
		// TODO(mosaic.Draw();)
		// 名前表示
		if( count >= VIEW_ITEM_COUNT + 40 ) {
			DrawCharacter::GetInst()->DrawString(100, 245, getItem.name, WHITE);
			switch( getItem.type ) {
			case ResultValue::eTYPE_CHIP:
				DrawCharacter::GetInst()->DrawString(230, 245, ToString("%c", ( char ) getItem.value), WHITE);
				break;
			case ResultValue::eTYPE_MONEY:
				DrawCharacter::GetInst()->DrawString(100 + 90, 245, ToString("%4dZ", getItem.value), WHITE);
				break;
			}
		}
	}
}

void Battle::StateWin::Process() {
	count++;
	if( count >= VIEW_ITEM_COUNT ) {
		// TODO(モザイク処理)
		if( count >= VIEW_ITEM_COUNT + 50 ) {
			if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 )
				obj->rtnCode = Battle::eRTN_WIN;
		}
	}
}
