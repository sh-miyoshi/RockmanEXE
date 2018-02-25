#pragma once

#include "point.h"
#include "define.h"
#include "animProcess.h"
#include "skill.h"

typedef struct AnimData{
	static const int EACH_ANIM_MAX = 50;

	int image[EACH_ANIM_MAX];
	int imageNum;
	int imageDelay;

	std::shared_ptr<AnimProcess> animProc;

	AnimData(){}
	AnimData(const AnimData &data);
}AnimData;

class BattleObject{
protected:


	// オブジェクト固有のアニメーション
	enum AnimPtn{
		// debug(ANIM_STANDがいりそうな気配)
		ANIM_MOVE,
		ANIM_ATK1,
		ANIM_ATK2,
		ANIM_ATK3,
		ANIM_ATK4,
		ANIM_DAMAGE,

		ANIM_PTN_MAX
	};

	int objectID;
	AnimData anim[ANIM_PTN_MAX];
	int animNum;
	int atachedAnimNo;
	int actCount;// 行動関数用汎用カウンタ
	CPoint<int> drawPosOffset;
	bool delFlag;
	def::charType damagedType;
	int hp, hpMax;
	CPoint<int> pos;
	def::charType myCharType;

	// animSetMaxFlag: 攻撃を強制停止するまでアニメーションし続けるか
	void SetAnim(int animNo, bool animSetMaxFlag = true);
	bool AnimProcess();
	virtual void DamageMutekiFunc(){}// ダメージを受けた時の処理(無敵処理とか)
public:
	BattleObject(int hp, CPoint<int> pos, def::charType damagedType, def::charType myCharType);
	BattleObject(const BattleObject &data);
	~BattleObject(){}

	bool isDelete()const{ return delFlag; }
	bool isAnim()const{ return atachedAnimNo != -1; }

	void StopAnim();

	virtual void Draw();
	void DrawAnim(int x, int y);
	virtual void Process();
	virtual bool DamageProc(std::list<DamageData> &damageList);

	virtual void LoadAnim() = 0;
	virtual void DeleteAnim();

	CPoint<int> GetPos()const{ return pos; }
	void SetPos(CPoint<int> pos){ this->pos = pos; }
	int GetHP()const{ return hp; }
	void SetHP(int hp);
	void AddHP(int addVal);
	int GetHPMax()const{ return hpMax; }
	def::charType GetCharType()const{ return myCharType; }
};