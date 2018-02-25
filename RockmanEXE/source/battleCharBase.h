#pragma once

#include <queue>
#include <list>
#include "skill.h"
#include "battleObject.h"
#include "define.h"

class BattleCharBase:public BattleObject{
	def::charType GetDamagedType(def::charType myCharType);
	int GetMyPanel(def::charType myCharType);
protected:
	static const int DEFAULT_MUTEKI_TIME = 2 * 60;// 無敵の時間

	std::string name;
	int mutekiCnt;
	int mutekiEndCnt;// 無敵を終了させるカウント
	std::queue<int> reserveAnim;// 次に行うべきアニメーション
	int deleteCount;// キャラのHPが0になってからのモーション用カウント

	bool Move(int direct, bool isMove);
	bool MoveCheck(int x, int y, bool checkPanel = true);
public:
	BattleCharBase(std::string name, int hp,def::charType myCharType);
	~BattleCharBase(){}

	virtual bool DamageProc(std::list<DamageData> &damageList);
	void Draw();
	virtual void Process();
};
