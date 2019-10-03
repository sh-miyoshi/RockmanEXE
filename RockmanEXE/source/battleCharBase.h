#pragma once

#include <string>
#include <vector>
#include <memory>
#include <queue>
#include "define.h"
#include "point.h"
#include "animation.h"

enum CharType {
	eCHAR_PLAYER = 0x1,
	eCHAR_ENEMY = 0x2,
	eCHAR_OBJECT = 0x4,

	eCHAR_ALL = eCHAR_PLAYER | eCHAR_ENEMY | eCHAR_OBJECT,
};


class BattleCharBase {
protected:
	std::string name;
	unsigned int hp, hpMax;
	CPoint<int> pos;
	CharType myCharType;
	bool animInitialized;
	std::vector<std::shared_ptr<Animation>> animData;
	std::shared_ptr<Animation> defaultAnim;
	std::queue<std::shared_ptr<Animation>> animQueue;

	void AnimProcess();
	bool MoveCheck(int x, int y);
public:
	BattleCharBase(std::string name, unsigned int hp, unsigned int hpMax, CharType myCharType);
	~BattleCharBase() {}

	virtual void Draw();
	virtual void Process();// キャラの行動

	void SetDefaultAnim(std::shared_ptr<Animation> anim) { this->defaultAnim = anim; }
	void AttachAnim(std::shared_ptr<Animation> anim, bool forceRun = false);

	void SetPos(int x, int y) { pos.x = x; pos.y = y; }
	CPoint<int> GetPos()const { return pos; }

	unsigned int GetHP()const { return hp; }
	void SetHP(int hp);// 自身のHPをセットする(ただしマイナスの場合は0にする)

	std::string GetName() const { return name; }
};
