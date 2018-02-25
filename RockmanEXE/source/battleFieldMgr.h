#pragma once

#include "battleBack.h"
#include "battleField.h"

class BattleFieldMgr{
	static const int GAUGE_MAX_COUNT = 1200;

	BattleField fieldData;
	BattleBack backMgr;
	int gaugeCount;
	int gaugeUpSpeed;
	int imgGauge[5];
	int imgMindWindowFrame;
	int drawGaugeMaxCount;// ゲージが満タンになった時のアニメーションのためのカウント

	BattleFieldMgr(){}
	~BattleFieldMgr(){}
public:
	enum GaugeUpSpeed{
		GUSPD_SLOW = 1,
		GUSPD_NORMAL = 2,
		GUSPD_FAST = 4
	};

	static BattleFieldMgr *GetInst(){
		static BattleFieldMgr inst;
		return &inst;
	}

	void Init();
	void End();
	void Draw();
	void DrawBaseFrame(bool isShift);
	void Process();
	void GaugeProcess();

	void GetPanelInfo(int result[BattleField::LAYER_MAX], int x, int y);
	void SetGaugeSpeed(GaugeUpSpeed spd);
	void ResetGaugeCount(){ gaugeCount = 0; }
	bool IsGaugeCharged()const{ return gaugeCount >= GAUGE_MAX_COUNT; }
	void AddDrawHitArea(CPoint<int> pos);
};