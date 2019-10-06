#pragma once

#include <list>
#include <memory>
#include "point.h"

class BattleField{
public:
	enum FieldLayer{
		LAYER_USER,// ���̃p�l�����v���C���[�̂��̂��G�̂��̂�
		LAYER_STATE,// �Ђъ���Ȃǂ̏�Ԃ�

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
	static const unsigned int FIELD_NUM_X=6;
	static const unsigned int FIELD_NUM_Y=3;
	static const CPoint<unsigned int> PANEL_SIZE;
	static const int BATTLE_PANEL_OFFSET_Y;// �p�l���̍ŏ��̈ʒu

	static CPoint<int> GetPixelPos(CPoint<int> pos);
private:
	int imgPanel[2];
	int panelInfo[LAYER_MAX][FIELD_NUM_X][FIELD_NUM_Y];
public:
	BattleField();
	~BattleField();

	void LoadImages();
	void DeleteImages();
	void InitInBattleBegin();
	void Draw();
	void GetPanelInfo(int result[LAYER_MAX],int x,int y);
};
