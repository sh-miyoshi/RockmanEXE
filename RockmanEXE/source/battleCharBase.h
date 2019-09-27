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
	BattleCharBase(std::string name, unsigned int hp, unsigned int hpMax, CharType myCharType, std::shared_ptr<Animation> defaultAnim);
	~BattleCharBase() {}

	// virtual bool DamageProc(std::list<DamageData>& damageList);// TODO(ñ¢é¿ëï)
	void Draw();
	virtual void Process();// ÉLÉÉÉâÇÃçsìÆ

	void SetPos(int x, int y) { pos.x = x; pos.y = y; }
	void AttachAnim(std::shared_ptr<Animation> anim, bool forceRun = false);
};
