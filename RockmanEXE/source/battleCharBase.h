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

//typedef struct AnimData {
//	static const int ANIM_NUM_MAX = 50;
//
//	int image[ANIM_NUM_MAX];
//	int imageNum;
//	int imageDelay;

//	// std::shared_ptr<AnimProcess> animProc;

//	AnimData();
//	AnimData(const AnimData& data);
//}AnimData;

class BattleCharBase {
protected:
	class AnimData {
		static const int ANIM_NUM_MAX = 50;
		
		int count;
		int image[ANIM_NUM_MAX];
		int imageNum;
		int imageDelay;
		std::string imageFileName;
	public:
		AnimData();
		AnimData(const AnimData& data);
		~AnimData();

		void ResetCount() { count = 0; }
		void LoadData(std::string fname, CPoint<int> size, CPoint<int> num, int imageNum, int imageDelay=1);
		void DeleteData();
		void Draw(int x, int y);
		// TODO(未実装) AnimProcess();
	};

	int attachedAnimNo;
	AnimData anim[ANIM_PTN_MAX];

	std::string name;
	int hp, hpMax;
	CPoint<int> pos;
	CharType myCharType;
public:
	BattleCharBase(std::string name, int hp, int hpMax, CharType myCharType);
	~BattleCharBase() {}

	virtual void LoadAnim() = 0;
	void DeleteAnim();

	// virtual bool DamageProc(std::list<DamageData>& damageList);// TODO(未実装)
	void Draw();
	virtual void Process();

	void SetPos(int x, int y) { pos.x = x; pos.y = y; }
};