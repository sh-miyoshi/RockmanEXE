#pragma once

#include <list>
#include <memory>
#include "point.h"

class BattleField{
public:
	enum FieldLayer{
		LAYER_USER,// そのパネルがプレイヤーのものか敵のものか
		LAYER_STATE,// ひび割れなどの状態か

		LAYER_MAX
	};

	enum FieldState{
		ST_NULL,
	};

	enum FieldUser{
		USER_PLAYER,
		USER_ENEMY,

		USER_MAX
	};
	static const int FIELD_NUM_X=6;
	static const int FIELD_NUM_Y=3;
	static const CPoint<int> PANEL_SIZE;
	static const int BATTLE_PANEL_TOP;// パネルの最初の位置
private:
	int imgPanel[2];
	int panelInfo[LAYER_MAX][FIELD_NUM_X][FIELD_NUM_Y];
	std::list<CPoint<int>> hitPanels;
public:
	BattleField();
	~BattleField();

	void LoadImages();
	void DeleteImages();
	void Init();
	void Draw();
	void GetPanelInfo(int result[LAYER_MAX],int x,int y);
	void AddDrawHitArea(CPoint<int> pos);
};
