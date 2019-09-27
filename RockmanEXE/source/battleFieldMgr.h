#pragma once

#include "battleBack.h"
#include "battleField.h"

class BattleFieldMgr{
	static const int GAUGE_MAX_COUNT = 1200;

	BattleField fieldData;
	BattleBack backData;

	BattleFieldMgr(){}
	~BattleFieldMgr(){}
public:
	static BattleFieldMgr *GetInst(){
		static BattleFieldMgr inst;
		return &inst;
	}

	void BattleInit();
	void BattleEnd();
	void Draw();
	void Process();

	void GetPanelInfo(int result[BattleField::LAYER_MAX], int x, int y);
};