#pragma once

#include "battleBack.h"
#include "battleField.h"

class BattleFieldMgr {
	static const int GAUGE_MAX_COUNT = 1200;

	unsigned int gaugeCount;
	unsigned int drawGaugeMaxCount;// ゲージが満タンになった時のアニメーションのためのカウント
	int imgGauge[5];
	int imgMindWindowFrame;
	int imgHpFrame;

	BattleField fieldData;
	BattleBack backData;

	BattleFieldMgr()
		:gaugeCount(0), drawGaugeMaxCount(0), imgGauge(), imgMindWindowFrame(-1),
		imgHpFrame(-1) {
	}
	~BattleFieldMgr() {}
public:
	enum BattleState {
		eSTATE_CHIP_SELECT,
		eSTATE_MAIN,
		eSTATE_RESULT_WIN
	};

	static BattleFieldMgr* GetInst() {
		static BattleFieldMgr inst;
		return &inst;
	}

	void BattleInit();
	void BattleEnd();
	void Draw();
	void DrawBaseFrame(BattleState state);
	void Process();
	void GaugeProcess();

	void GetPanelInfo(int result[BattleField::LAYER_MAX], int x, int y);
};