#pragma once

#include <list>
#include "define.h"
#include "point.h"

// オブジェクト固有のアニメーション
enum AnimPtn {
	ANIM_STAND,
	ANIM_MOVE,
	ANIM_ATK1,
	ANIM_ATK2,
	ANIM_ATK3,
	ANIM_ATK4,
	ANIM_DAMAGE,

	ANIM_PTN_MAX
};

enum CharType {
	eCHAR_PLAYER = 0x1,
	eCHAR_ENEMY = 0x2,
	eCHAR_OBJECT = 0x4,

	eCHAR_ALL = eCHAR_PLAYER | eCHAR_ENEMY | eCHAR_OBJECT,
};

typedef struct AnimData {
	static const int ANIM_NUM_MAX = 50;

	int image[ANIM_NUM_MAX];
	int imageNum;
	int imageDelay;

	// std::shared_ptr<AnimProcess> animProc;

	AnimData();
	AnimData(const AnimData& data);
}AnimData;

class BattleCharBase {
protected:
	AnimData anim[ANIM_PTN_MAX];

	std::string name;
	int hp, hpMax;
	CPoint<int> pos;
	CharType myCharType;
public:
	BattleCharBase(std::string name, int hp, int hpMax, CharType myCharType);
	~BattleCharBase() {}

	// virtual bool DamageProc(std::list<DamageData>& damageList);// TODO(未実装)
	void Draw();
	virtual void Process();
};