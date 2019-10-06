#include "include.h"
#include "battleField.h"

const CPoint<unsigned int> BattleField::PANEL_SIZE = CPoint<unsigned int>(80, 50);
const int BattleField::BATTLE_PANEL_OFFSET_Y = def::FMY - ( PANEL_SIZE.y * 3 ) - 30;

CPoint<int> BattleField::GetPixelPos(CPoint<int> pos) {
	CPoint<int> res;
	res.x = BattleField::PANEL_SIZE.x * pos.x + BattleField::PANEL_SIZE.x / 2;
	res.y = BattleField::BATTLE_PANEL_OFFSET_Y + BattleField::PANEL_SIZE.y * pos.y - 10;// ふちの分だけ上げる
	return res;
}

BattleField::BattleField():imgPanel(), panelInfo(){
}

BattleField::~BattleField() {
}

void BattleField::LoadImages() {
	// データの読み込み
	const std::string position = "BattleField::LoadImages";

	std::string fname = def::IMAGE_FILE_PATH + "battle/フレーム/battle_panel_plyr.png";
	imgPanel[USER_PLAYER] = LoadGraphWithErrorCheck(fname, position);

	fname = def::IMAGE_FILE_PATH + "battle/フレーム/battle_panel_enemy.png";
	imgPanel[USER_ENEMY] = LoadGraphWithErrorCheck(fname, position);

	AppLogger::Info("Successfully loaded battle field images");
}

void BattleField::DeleteImages() {
	for( auto i = 0; i < USER_MAX; i++ )
		DeleteGraph(imgPanel[i]);
}

void BattleField::InitInBattleBegin() {
	// フィールド情報の初期化
	for( int y = 0; y < FIELD_NUM_Y; y++ ) {
		for( int x = 0; x < FIELD_NUM_X / 2; x++ ) {
			panelInfo[LAYER_USER][x][y] = USER_PLAYER;
			panelInfo[LAYER_USER][x + FIELD_NUM_Y][y] = USER_ENEMY;
		}
	}

	for( int y = 0; y < FIELD_NUM_Y; y++ )
		for( int x = 0; x < FIELD_NUM_X; x++ )
			panelInfo[LAYER_STATE][x][y] = ST_NULL;
}

void BattleField::Draw() {
	// パネル描画
	for( auto x = 0; x < FIELD_NUM_X; x++ ) {
		for( auto y = 0; y < FIELD_NUM_Y; y++ ) {
			int image = imgPanel[panelInfo[LAYER_USER][x][y]];
			DrawGraph(PANEL_SIZE.x * x, BATTLE_PANEL_OFFSET_Y + PANEL_SIZE.y * y, image, TRUE);
		}
	}
}

void BattleField::GetPanelInfo(int result[LAYER_MAX], int x, int y) {
	if( x < 0 || x >= FIELD_NUM_X || y < 0 || y >= FIELD_NUM_Y ) {
		AppLogger::Error("BattleField::GetPanelInfo invalied field position x: %d, y: %d)", x, y);
		exit(1);
	}

	for( int i = 0; i < LAYER_MAX; i++ )
		result[i] = panelInfo[i][x][y];
}
