#include <algorithm>
#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "playerMgr.h"
#include "drawCharacter.h"
#include "battleFieldMgr.h"

bool Battle::StateChipSelect::PlayerHandMgr::CanSelect(unsigned int no) {
	if( selectedIndexes.size() < 1 ) {// まだ何も選択されてないなら
		return true;
	}

	auto res = std::find(selectedIndexes.begin(), selectedIndexes.end(),no);
	if( res != selectedIndexes.end() ) {// すでに選択されているなら
		return false;
	}

	// すべての名前が一致なら選択できる
	std::string name = ChipMgr::GetInst()->GetChipData(handValue[no]).name;
	bool isSameName = true;
	for( auto i : selectedIndexes ) {
		ChipData c = ChipMgr::GetInst()->GetChipData(handValue[i]);
		if( name != c.name ) {
			isSameName = false;
			break;
		}
	}
	if( isSameName ) {
		return true;
	}

	// コードが一致していれば選択できる
	char code = handValue[no].code;
	bool isSameCode = true;
	for( auto i : selectedIndexes ) {
		char c = handValue[i].code;
		if( code == '*' ) {
			code = c;
		} else if( code != c && c!='*' ) {
			isSameCode = false;
			break;
		}
	}
	if( isSameCode ) {
		return true;
	}

	return false;
}

void Battle::StateChipSelect::PlayerHandMgr::Init(std::vector<ChipInfo> handValue) {
	this->handValue = handValue;
	selectedIndexes.clear();
}

bool Battle::StateChipSelect::PlayerHandMgr::Select(unsigned int no) {
	if( CanSelect(no) ) {// そのチップが選択可能なら
		selectedIndexes.push_back(no);
		return true;
	}
	return false;
}

void Battle::StateChipSelect::PlayerHandMgr::Draw() {
	BattleFieldMgr::GetInst()->DrawBaseFrame(BattleFieldMgr::eSTATE_CHIP_SELECT);

	if( handValue.size() > 5 ) {
		AppLogger::Error("[未実装]Hand数 %dは描画を考慮できてない", handValue.size());// TODO
		exit(1);
	}

	for( size_t i = 0; i < handValue.size(); i++ ) {
		auto res = std::find(selectedIndexes.begin(), selectedIndexes.end(), i);
		if( res == selectedIndexes.end() ) {// チップが存在し、選択されてなければ
			int x = i * 32 + 17;
			ChipData c = ChipMgr::GetInst()->GetChipData(handValue[i]);
			if( CanSelect(i) ) {
				DrawGraph(x, 210, c.imgIcon, TRUE);
			} else {
				DrawGraph(x, 210, c.imgIconMono, TRUE);// 選択できないものには灰色で
			}
			// チップコードの描画
			DrawCharacter::GetInst()->DrawChipCode(x + 10, 240, c.code, 50);
		}
	}

	// 送信するチップ一覧の表示
	for( size_t i = 0; i < selectedIndexes.size(); i++ ) {
		ChipData c = ChipMgr::GetInst()->GetChipData(handValue[selectedIndexes[i]]);
		DrawGraph(194, 50 + i * 32, c.imgIcon, TRUE);
	}
}

ChipData Battle::StateChipSelect::PlayerHandMgr::GetChipData(unsigned int no){
	if(no>=handValue.size()){
		AppLogger::Error("選択しようとしている場所が異常です. size: %d, no: %d", handValue.size(), no);
	}
	return ChipMgr::GetInst()->GetChipData(handValue[no]);
}

void Battle::StateChipSelect::PlayerHandMgr::EraseLastSelect() {
	if( !selectedIndexes.empty() ) {
		selectedIndexes.pop_back();
	}
}

void Battle::StateChipSelect::PlayerHandMgr::SetSendChipList() {
	PlayerMgr::GetInst()->GetBattleChar()->SetSendChipList(selectedIndexes);
}

Battle::StateChipSelect::StateChipSelect(Battle* obj)
	:obj(obj), imgSelectFrame(-1), pointer(0), imgPointer(), drawCount(0) {

	// 画像の読み込み
	const std::string position = "Battle::StateChipSelect::StateChipSelect";
	std::string fname = def::IMAGE_FILE_PATH + "battle/フレーム/battle_chip_select.png";
	imgSelectFrame = LoadGraphWithErrorCheck(fname, position);

	fname = def::IMAGE_FILE_PATH + "battle/フレーム/pointer.png";
	LoadDivGraphWithErrorCheck(imgPointer, fname, position, 2, 1, 44, 44);

	// pointerの初期位置セット
	int HAND_NUM = 5;// TODO(ナビカスなどで変更できるようにする)
	auto hands = PlayerMgr::GetInst()->GetBattleChar()->GetHandData(HAND_NUM);
	if( hands.size() == 0 ) {
		pointer = BT_SEND_NO;
	} else {
		// ハンドデータをセットする
		playerHand.Init(hands);
	}
}

Battle::StateChipSelect::~StateChipSelect() {
	DeleteGraph(imgSelectFrame);
	for( int i = 0; i < 2; i++ ) {
		DeleteGraph(imgPointer[i]);
	}
}

void Battle::StateChipSelect::Draw() {
	BattleCharMgr::GetInst()->Draw();

	// todo(開始時に横からスライドしてくるように)
	// 選択エリアの描画
	DrawGraph(0, 0, imgSelectFrame, TRUE);

	playerHand.Draw();

	// 選択位置の描画
	static const int DRAW_DELAY = 40;
	if( ( drawCount / DRAW_DELAY ) % 2 == 0 ) {
		if( pointer == BT_SEND_NO )
			DrawGraph(180, 225, imgPointer[1], TRUE);
		else {
			int x = ( pointer % 5 ) * 32 + 12;
			int y = ( pointer / 5 ) * 20 + 200;
			DrawGraph(x, y, imgPointer[0], TRUE);
		}
	}

	// チップ詳細情報の表示
	if( 0<=pointer && pointer < playerHand.GetHandNum() ){
		ChipData c = playerHand.GetChipData(pointer);
		DrawGraph(31, 64, c.imgInfo, TRUE);
		DrawGraph(52, 161, ChipMgr::GetInst()->GetTypeData(c.type).image, TRUE);
		DrawCharacter::GetInst()->DrawChipCode(30, 163, c.code);
		DrawCharacter::GetInst()->DrawString(20, 20, c.name, BLACK);
		if( c.power > 0 ){
			// 攻撃力も描画
			DrawCharacter::GetInst()->DrawNumber(100, 163, c.power, DrawCharacter::COL_WHITE, 3);
		}
	}
}

void Battle::StateChipSelect::Process() {
	if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 ) {
		if( pointer == BT_SEND_NO ) {// チップ選択完了
			playerHand.SetSendChipList();// 選んだチップをBattleStatemainへ引き渡す
			obj->stateMgr.ChangeNext(new Battle::StateMain(obj));
		} else {
			playerHand.Select(pointer);
		}
	} else if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ) {
		// 選択しているチップの最後の値を削除する
		playerHand.EraseLastSelect();
	}else if( playerHand.GetHandNum() > 0 ) {
		// カーソル移動処理
		if( CKey::GetInst()->CheckKey(eKEY_LEFT) == 1 ) {
			if( pointer == BT_SEND_NO )
				pointer = ( playerHand.GetHandNum() > 5 ) ? 4 : playerHand.GetHandNum() - 1;
			else if( ( pointer % 5 ) == 0 )
				pointer = BT_SEND_NO;
			else
				pointer--;
		} else if( CKey::GetInst()->CheckKey(eKEY_RIGHT) == 1 ) {
			if( pointer == BT_SEND_NO )
				pointer = 0;
			else if( pointer == playerHand.GetHandNum() - 1 )
				pointer = BT_SEND_NO;
			else if( playerHand.GetHandNum() > 5 && pointer == 4 )
				pointer = BT_SEND_NO;
			else
				pointer++;
		}
	}

	drawCount++;
}
