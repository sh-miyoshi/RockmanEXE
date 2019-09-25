#pragma once

#include <list>
#include <vector>
#include <memory>
#include <queue>
#include "define.h"
#include "point.h"

enum CharType {
	eCHAR_PLAYER = 0x1,
	eCHAR_ENEMY = 0x2,
	eCHAR_OBJECT = 0x4,

	eCHAR_ALL = eCHAR_PLAYER | eCHAR_ENEMY | eCHAR_OBJECT,
};

class Animation {
	static const unsigned int ANIM_NUM_MAX = 50;

	unsigned int count;
	int image[ANIM_NUM_MAX];
	unsigned int imageNum;
	unsigned int imageDelay;
	std::string imageFileName;
public:
	Animation();
	Animation(const Animation& data);
	~Animation();

	void LoadData(std::string fname, CPoint<unsigned int> size, CPoint<unsigned int> num, unsigned int imageDelay = 1);
	void DeleteData();
	
	virtual void Begin() {}
	virtual void End() {}
	void Draw(int x, int y);
	virtual void Process() {}
};

class BattleCharBase {
protected:
	std::string name;
	unsigned int hp, hpMax;
	CPoint<int> pos;
	CharType myCharType;
	std::vector<std::shared_ptr<Animation>> animData;
	std::shared_ptr<Animation> defaultAnim;
	std::queue<std::shared_ptr<Animation>> animQueue;

	void AnimProcess();
public:
	BattleCharBase(std::string name, unsigned int hp, unsigned int hpMax, CharType myCharType, std::shared_ptr<Animation> defaultAnim);
	~BattleCharBase() {}

	unsigned int AddAnimDefine(std::shared_ptr<Animation> anim);

	// virtual bool DamageProc(std::list<DamageData>& damageList);// TODO(������)
	void Draw();
	virtual void Process();// �L�����̍s��

	void SetPos(int x, int y) { pos.x = x; pos.y = y; }
	void AttachAnim(unsigned int animNo, bool forceRun = false);
};

/* TODO

anim�pqueue��p��
�L�[�������ꂽ�Ƃ�
  �ʏ�s���Ȃ�L���[�ɒǉ�
  ���荞�ݍs���Ȃ�L���[����ɂ��Č��ݎ��s���Ă���̂����f

�L���[�������o��
���s��Ԃɂ���
���s�I���Ȃ�L���[����܂����o��

�L���[����Ȃ�default�����s(ANIM_STAND)
*/