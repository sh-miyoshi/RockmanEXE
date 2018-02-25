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


	// �I�u�W�F�N�g�ŗL�̃A�j���[�V����
	enum AnimPtn{
		// debug(ANIM_STAND�����肻���ȋC�z)
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
	int actCount;// �s���֐��p�ėp�J�E���^
	CPoint<int> drawPosOffset;
	bool delFlag;
	def::charType damagedType;
	int hp, hpMax;
	CPoint<int> pos;
	def::charType myCharType;

	// animSetMaxFlag: �U����������~����܂ŃA�j���[�V�����������邩
	void SetAnim(int animNo, bool animSetMaxFlag = true);
	bool AnimProcess();
	virtual void DamageMutekiFunc(){}// �_���[�W���󂯂����̏���(���G�����Ƃ�)
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