#include "include.h"
#include "battle.h"
#include "fps.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"
#include "drawCharacter.h"

Battle::Battle(std::list<BattleEnemyInit_t> enemyInfo, bool isBoss)
	:rtnCode(eBATTLE_RTN_CONTINUE), mainActionTimeCount(0), isBoss(isBoss)
{
	BattleFieldMgr::GetInst()->Init();
	BattleCharMgr::GetInst()->Init(enemyInfo);

	stateMgr.ChangeNext(new StateBegin(this));
}

Battle::~Battle(){
	BattleCharMgr::GetInst()->End();
	BattleFieldMgr::GetInst()->End();
}

void Battle::Draw(){
	BattleFieldMgr::GetInst()->Draw();
	stateMgr.Draw();
}

BattleRtnCode Battle::Process(){
	BattleFieldMgr::GetInst()->Process();
	stateMgr.Process();
	return rtnCode;
}

//---------------------------------------------
// 各状態ごとの処理
//---------------------------------------------
Battle::StateBegin::StateBegin(Battle *obj):obj(obj), actCount(0), drawNum(1){
}

Battle::StateBegin::~StateBegin(){
}

void Battle::StateBegin::Draw(){
	// 敵描画(開始時はアニメーションさせるため描画しない)
	BattleCharMgr::GetInst()->DrawObjects(drawNum, actCount);
}

void Battle::StateBegin::Process(){
	actCount += BEGIN_COUNTUP_VALUE;
	if( actCount >= 256 ){
		actCount = 0;
		drawNum++;
		if( drawNum > ( int ) BattleCharMgr::GetInst()->GetEnemyNum() ){
			obj->stateMgr.ChangeNext(new Battle::StateChipSelect(obj));
		}
	}
}

Battle::StateChipSelect::StateChipSelect(Battle * obj):obj(obj), pointer(0){
	int HAND_NUM = 5;// debug(ナビカスとかで変えられるように)
	std::vector<ChipInfo> t = BattleCharMgr::GetInst()->GetPlayerHand(HAND_NUM);
	if( t.empty() )
		pointer = BT_SEND_NO;
	else{
		playerHand.InitData(t);
	}

	const std::string position = "Battle::StateChipSelect::StateChipSelect";
	std::string fname = def::IMAGE_FILE_PATH + "battle_chip_select.png";
	imgFrame = LoadGraphWithCheckError(fname, position);

	fname = def::IMAGE_FILE_PATH + "selector.png";
	LoadDivGraphWithCheckError(imgSelector, fname, position, 2, 1, 44, 44);

	obj->sendChip.clear();
}

Battle::StateChipSelect::~StateChipSelect(){
	DeleteGraph(imgFrame);
	for( int i = 0; i < sizeof(imgSelector) / sizeof(imgSelector[0]); i++ )
		DeleteGraph(imgSelector[i]);
}

void Battle::StateChipSelect::Draw(){
	BattleCharMgr::GetInst()->DrawObjects();
	BattleFieldMgr::GetInst()->DrawBaseFrame(true);

	// todo(開始時に横からスライドしてくるように)
	// 選択エリアの描画
	DrawGraph(0, 0, imgFrame, TRUE);

	playerHand.Draw();

	// 選択位置の描画
	if( pointer == BT_SEND_NO )
		DrawGraph(180, 225, imgSelector[1], TRUE);
	else{
		int x = ( pointer % 5 ) * 32 + 12;
		int y = ( pointer / 5 ) * 20 + 200;
		DrawGraph(x, y, imgSelector[0], TRUE);
	}

	// チップ詳細情報
	if( pointer != BT_SEND_NO ){
		ChipData sc = ChipMgr::GetInst()->GetChipData(playerHand.GetChip(pointer));
		DrawGraph(31, 64, sc.imgInfo, TRUE);
		DrawGraph(52, 161, ChipMgr::GetInst()->GetTypeData(sc.type).image, TRUE);
		DrawCharacter::GetInst()->DrawChipCode(30, 163, sc.code);
		DrawCharacter::GetInst()->DrawString(20, 25, sc.name, BLACK);
		if( sc.power > 0 ){
			// 攻撃力も描画
			DrawCharacter::GetInst()->DrawNumber(100, 163, sc.power, DrawCharacter::COL_WHITE, 3);
		}
	}
}

void Battle::StateChipSelect::Process(){
	// チップ選択
	CKey *in = CKey::GetInst();
	if( in->CheckKey(eKEY_ENTER) == 1 ){// 決定キーが押されたら
		if( pointer == BT_SEND_NO ){
			BattleCharMgr::GetInst()->SendActionChips(playerHand.GetSelectedChipIndex());
			obj->stateMgr.ChangeNext(new StateBattleStart(obj));
		} else{
			playerHand.Select(pointer);
		}
	} else if( in->CheckKey(eKEY_CANCEL) == 1 ){// キャンセルキーが押されたら
		playerHand.EraseLastValue();
	} else{
		if( playerHand.GetHandNum() > 0 ){
			// カーソル移動処理
			if( in->CheckKey(eKEY_LEFT) == 1 ){
				if( pointer == BT_SEND_NO )
					pointer = ( playerHand.GetHandNum() > 5 ) ? 4 : playerHand.GetHandNum() - 1;
				else if( ( pointer % 5 ) == 0 )
					pointer = BT_SEND_NO;
				else
					pointer--;
			} else if( in->CheckKey(eKEY_RIGHT) == 1 ){
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
	}
}

char Battle::StateChipSelect::HandChip::GetCode(){
	char valid = '*';
	unsigned index = 0;
	for( ; index < codes.size(); index++ ){
		if( codes[index] != '*' ){
			valid = codes[index];
			break;
		}
	}
	for( ; index < codes.size(); index++ ){
		if( codes[index] != valid&&codes[index] != '*' )
			return '-';
	}
	return valid;// すべて同じコード(*も含む)
}

std::string Battle::StateChipSelect::HandChip::GetFixedName(){
	if( names.size() < 2 )
		return "";// 固定ネームはなし
	for( unsigned i = 1; i < names.size(); i++ )
		if( names[0] != names[i] )
			return "-";// 名前が異なる
	return names[0];// 固定ネーム
}

void Battle::StateChipSelect::HandChip::InitData(std::vector<ChipInfo> handValue){
	this->handValue = handValue;
	selectedIndex.clear();
	codes.clear();
	names.clear();
}

void Battle::StateChipSelect::HandChip::Draw(){
	// チップ描画
	std::string err_msg = "Battle::StateChipSelect::HandChip::Draw: ";
	err_msg += "(debug) hand数" + ToString<unsigned>(handValue.size()) + "は描画を考慮できない";
	ASSERT(handValue.size() <= 5, err_msg);
	for( unsigned i = 0; i < handValue.size(); i++ ){
		if( !find<unsigned>(selectedIndex, i) ){// チップが存在し、選択されてなければ
			int x = i * 32 + 17;
			ChipData c = ChipMgr::GetInst()->GetChipData(handValue[i]);
			if( CanSelect(i) ){
				DrawGraph(x, 210, c.imgIcon, TRUE);
			} else{
				DrawGraph(x, 210, c.imgIconMono, TRUE);// 選択できないものには灰色で
			}
			DrawCharacter::GetInst()->DrawChipCode(x + 10, 240, c.code, 50);
		}
	}

	// 送信するチップ一覧の表示
	for( unsigned i = 0; i < selectedIndex.size(); i++ ){
		ChipData c = ChipMgr::GetInst()->GetChipData(handValue[selectedIndex[i]]);
		DrawGraph(194, 50 + i * 32, c.imgIcon, TRUE);
	}
}

void Battle::StateChipSelect::HandChip::EraseLastValue(){
	if( !selectedIndex.empty() ){
		selectedIndex.pop_back();
		codes.pop_back();
		names.pop_back();
	}
}

bool Battle::StateChipSelect::HandChip::CanSelect(unsigned no){
	std::string err_msg = "Battle::StateChipSelect::HandChip::CanSelect: ";
	err_msg += "チェック番号が総数より大きいです " + ToString("no: %d, max: %d", no, handValue.size());
	ASSERT(no < handValue.size(), err_msg);

	if( !find<unsigned>(selectedIndex, no) ){// まだ選択されてなければ
		std::string name = ChipMgr::GetInst()->GetChipData(handValue[no].id).name;
		char code = handValue[no].code;

		// 固定nameがあるか？ あれば名前が違えばダメ
		// なければcodeが*が等しければok
		std::string fn = GetFixedName();
		if( !fn.empty() ){
			if( fn != "-"&&fn != name )
				return false;// 選択できない
			else if( fn == "-"&&code != '*'&&GetCode() != code )// 名前が異なるのでコードが一致するか
				return false;// 選択できない
		} else if( names.size() == 1 ){
			if( names[0] != name&&code != '*'&&GetCode() != code&&codes[0] != '*' )
				return false;// 選択できない
		}
		return true;// 選択できる
	}
	return false;// 選択できない
}

bool Battle::StateChipSelect::HandChip::Select(unsigned no){
	if( CanSelect(no) ){
		std::string name = ChipMgr::GetInst()->GetChipData(handValue[no].id).name;
		char code = handValue[no].code;

		names.push_back(name);
		codes.push_back(code);
		selectedIndex.push_back(no);
		return true;// 選択した
	}
	return false;// 選択に失敗
}

ChipInfo Battle::StateChipSelect::HandChip::GetChip(unsigned no){
	std::string err_msg = "Battle::StateChipSelect::HandChip::CanSelect: ";
	err_msg += "Get番号が総数より大きいです " + ToString("no: %d, max: %d", no, handValue.size());
	ASSERT(no < handValue.size(), err_msg);
	return handValue[no];
}

Battle::StateBattleStart::StateBattleStart(Battle * obj):obj(obj), count(0){
	const std::string position = "Battle::StateBattleStart::StateBattleStart";
	std::string fname = def::IMAGE_FILE_PATH + "battle_begin.png";
	LoadDivGraphWithCheckError(imageTitle, fname, position, 1, 3, 274, 32);
}

Battle::StateBattleStart::~StateBattleStart(){
	for( int i = 0; i < 3; i++ )
		DeleteGraph(imageTitle[i]);
}

void Battle::StateBattleStart::Draw(){
	BattleFieldMgr::GetInst()->DrawBaseFrame(false);
	BattleCharMgr::GetInst()->DrawObjects();

	int n = count / DRAW_DELAY;
	if( n > 2 )
		n = 2;
	DrawGraph(105, 125, imageTitle[n], TRUE);
}

void Battle::StateBattleStart::Process(){
	count++;
	if( count >= 3 * DRAW_DELAY + 20 )
		obj->stateMgr.ChangeNext(new StateMainAction(obj));
}


Battle::StateMainAction::StateMainAction(Battle * obj):obj(obj){
	BattleFieldMgr::GetInst()->ResetGaugeCount();
}

Battle::StateMainAction::~StateMainAction(){
}

void Battle::StateMainAction::Draw(){
	BattleCharMgr::GetInst()->DrawObjects();
	BattleFieldMgr::GetInst()->DrawBaseFrame(false);

	BattleCharMgr::GetInst()->DrawPlayerSendChipIcon();
}

void Battle::StateMainAction::Process(){
	obj->mainActionTimeCount++;

	BattleFieldMgr::GetInst()->GaugeProcess();

	switch( BattleCharMgr::GetInst()->Process() ){
	case eBATTLE_RTN_WIN:
		obj->stateMgr.ChangeNext(new StateWin(obj));
		return;
	case eBATTLE_RTN_LOSE:
		obj->rtnCode = eBATTLE_RTN_LOSE;
		return;
	}

	// カスタムゲージが満タンでL,Rキーが押されたら
	if( BattleFieldMgr::GetInst()->IsGaugeCharged() ){
		if( CKey::GetInst()->CheckKey(eKEY_DEV_L) == 1 || CKey::GetInst()->CheckKey(eKEY_DEV_R) == 1 ){
			obj->stateMgr.ChangeNext(new StateChipSelect(obj));
		}
	}
}

Battle::StateWin::StateWin(Battle * obj)
	:obj(obj), count(0), bustingLv(0),
	mosaic(CPoint<int>(268, 189), CPoint<int>(112, 96), CPoint<int>(16, 16))
{
	const std::string position = "Battle::StateWin::StateWin";
	std::string fname = def::IMAGE_FILE_PATH + "zenny.png";
	imgZenny = LoadGraphWithCheckError(fname, position);
	fname = def::IMAGE_FILE_PATH + "battle_result_frame.png";
	imgResultFrame = LoadGraphWithCheckError(fname, position);

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
	int time = obj->mainActionTimeCount / Fps::FPS;
	bustingLv = 4;
	if( obj->isBoss ){
		const int deadline[4] = { 50,40,30,-1 };
		for( int i = 0; i < 4; i++ ){
			if( time > deadline[i] ){
				bustingLv += ( i * 2 );
				break;
			}
		}
	} else{
		const int deadline[4] = { 36,12,5,-1 };
		for( int i = 0; i < 4; i++ ){
			if( time > deadline[i] ){
				bustingLv += i;
				break;
			}
		}
	}

	// debug(実装は時間のみ)

	std::vector<ResultView_t> getItems;

	// チップの決定
	std::list<EnemyMgr::EnemyAttackChip_t> enemyChips = BattleCharMgr::GetInst()->GetEnemyChips();
	for( std::list<EnemyMgr::EnemyAttackChip_t>::iterator it = enemyChips.begin(); it != enemyChips.end(); it++ ){
		if( bustingLv >= ( unsigned ) it->bustingLevel ){// 自身のバスティングレベルが必要レベルを超えていたら
			ResultView_t t;
			t.type = ResultView_t::eTYPE_CHIP;
			ChipData t2 = ChipMgr::GetInst()->GetChipData(it->chip.id, it->chip.code);
			t.name = t2.name;
			t.value = ( int ) t2.code;
			t.image = t2.imgInfo;
			getItems.push_back(t);
		}
	}

	// ゼニーの決定
	// 30,50,100,200,400,500,1000,2000
	ResultView_t t;
	t.type = ResultView_t::eTYPE_ZENNY;
	t.name = "ゼニー";
	t.image = imgZenny;
	switch( bustingLv ){
	case  4: t.value = 30; break;
	case  5: t.value = 50; break;
	case  6: t.value = 100; break;
	case  7: t.value = 200; break;
	case  8: t.value = 400; break;
	case  9: t.value = 500; break;
	case 10: t.value = 1000; break;
	default:
		t.value = 2000;
		break;
	}
	getItems.push_back(t);

	// todo(重みをもたせる)
	// ランダムで選択
	getItem = getItems[rand() % getItems.size()];
	for( auto d : getItems ){
		std::string msg = "Battle::StateWin::StateWin ";
		msg += d.name + " " + ToString<int>(d.value);
		AppLogger::Debug(msg);
	}
}

Battle::StateWin::~StateWin(){
	DeleteGraph(imgZenny);
	DeleteGraph(imgResultFrame);
}

void Battle::StateWin::Draw(){
	BattleFieldMgr::GetInst()->DrawBaseFrame(false);

	// フレームは横から出てくる
	int x = count * 5;
	if( x > 40 )
		x = 40;
	DrawGraph(x, 45, imgResultFrame, TRUE);

	if( count >= VIEW_ITEM_COUNT ){
		// デリート時間の描画
		int time = obj->mainActionTimeCount / Fps::FPS;
		int min = time / 60;
		int sec = time % 60;
		if( min > 99 )
			min = 99;
		DrawCharacter::GetInst()->DrawNumberPadding(305, 92, min, 2);
		DrawCharacter::GetInst()->DrawString(340, 95, "：", WHITE);
		DrawCharacter::GetInst()->DrawNumberPadding(355, 92, sec, 2);

		// バスティングレベルの描画
		DrawCharacter::GetInst()->DrawNumber(365, 140, bustingLv);

		DrawGraph(268, 189, getItem.image, TRUE);
		mosaic.Draw();
		// 名前表示
		if( count >= VIEW_ITEM_COUNT + 40 ){
			DrawCharacter::GetInst()->DrawString(100, 245, getItem.name, WHITE);
			switch( getItem.type ){
			case ResultView_t::eTYPE_CHIP:
				DrawCharacter::GetInst()->DrawString(230, 245, ToString("%c", getItem.value), WHITE);
				break;
			case ResultView_t::eTYPE_ZENNY:
				DrawCharacter::GetInst()->DrawString(100 + 90, 245, ToString("%4dZ", getItem.value), WHITE);
				break;
			}
		}
	}
}

void Battle::StateWin::Process(){
	if( count >= VIEW_ITEM_COUNT ){
		if( count % 5 == 0 )
			mosaic.Update(10);
		if( count >= VIEW_ITEM_COUNT + 50 ){
			if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 )
				obj->rtnCode = eBATTLE_RTN_WIN;
		}
	}
	count++;
}
