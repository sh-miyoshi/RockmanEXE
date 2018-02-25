#include "include.h"
#include "battleField.h"

const CPoint<int> BattleField::PANEL_SIZE = CPoint<int>(80, 50);
const int BattleField::BATTLE_PANEL_TOP = def::FMY - ( PANEL_SIZE.y * 3 ) - 30;

BattleField::BattleField(){
}

BattleField::~BattleField(){
}

void BattleField::LoadImages(){
	// データの読み込み
	const std::string position = "BattleField::LoadImages";

	std::string fname = def::IMAGE_FILE_PATH + "battle_panel_plyr.png";
	imgPanel[USER_PLAYER] = LoadGraphWithCheckError(fname, position);

	fname = def::IMAGE_FILE_PATH + "battle_panel_enemy.png";
	imgPanel[USER_ENEMY] = LoadGraphWithCheckError(fname, position);
}

void BattleField::DeleteImages(){
	for( int i = 0; i < 2; i++ )
		DeleteGraph(imgPanel[i]);
}

void BattleField::Init(){
	// フィールド情報の初期化
	for( int y = 0; y < FIELD_NUM_Y; y++ ){
		for( int x = 0; x < FIELD_NUM_X / 2; x++ ){
			panelInfo[LAYER_USER][x][y] = USER_PLAYER;
			panelInfo[LAYER_USER][x + FIELD_NUM_Y][y] = USER_ENEMY;
		}
	}

	for( int y = 0; y < FIELD_NUM_Y; y++ )
		for( int x = 0; x < FIELD_NUM_X; x++ )
			panelInfo[LAYER_STATE][x][y] = ST_NULL;
}

void BattleField::Draw(){
	// パネル描画
	for( int x = 0; x < FIELD_NUM_X; x++ ){
		for( int y = 0; y < FIELD_NUM_Y; y++ ){
			int image = imgPanel[panelInfo[LAYER_USER][x][y]];
			DrawGraph(PANEL_SIZE.x*x, BATTLE_PANEL_TOP + PANEL_SIZE.y*y, image, TRUE);
		}
	}

	// 攻撃のヒットエリアの描画
	for( std::list<CPoint<int>>::iterator it = hitPanels.begin(); it != hitPanels.end(); it++ ){
		int x1 = BattleField::PANEL_SIZE.x*it->x;
		int y1 = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*it->y;
		int x2 = BattleField::PANEL_SIZE.x*( it->x + 1 );
		int y2 = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*( it->y + 1 );
		const int s = 5;
		DrawBox(x1 + s, y1 + s, x2 - s, y2 - s, GetColor(0xff, 0xff, 0), TRUE);
	}
	hitPanels.clear();
}

void BattleField::GetPanelInfo(int result[LAYER_MAX], int x, int y){
	std::string msg = "BattleField::GetPanelInfo invalied field ";
	ASSERT(x >= 0 && x < FIELD_NUM_X, msg + ToString("x (%d)", x));
	ASSERT(y >= 0 && y < FIELD_NUM_Y, msg + ToString("y (%d)", y));

	for( int i = 0; i < LAYER_MAX; i++ )
		result[i] = panelInfo[i][x][y];
}

void BattleField::AddDrawHitArea(CPoint<int> pos){
	hitPanels.push_back(pos);
}
