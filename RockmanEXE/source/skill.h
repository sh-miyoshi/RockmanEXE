#pragma once

#include <list>
#include <memory>
#include "point.h"
#include "chip.h"
#include "include.h"

class SkillArg{
public:
	CPoint<int> atkPos;
//	CPoint<int> charPos;
	int damage;
	int recover;
};

class DamageData{
public:
	def::charType myCharType;
	int damage;
	CPoint<int> pos;
	bool makeMuteki;
	int delCount;
	int skillID;
	bool forceDelFlag;
	bool isPenetrate;// ���G��Ԃ��ђʂ��邩

	DamageData(CPoint<int> pos, int damage, def::charType myCharType, bool makeMuteki, int delCount, int skillID, bool isPenetrate)
		:damage(damage), pos(pos), myCharType(myCharType), makeMuteki(makeMuteki)
		, delCount(delCount), skillID(skillID), forceDelFlag(false), isPenetrate(isPenetrate){}
};

class SkillData{
public:
	int id;
	bool delFlag;
	bool dependChar;// �X�L�����L�����N�^�̏�ԂɈˑ����邩(�̂����莞�X�g�b�v���邩)
	bool isPenetrate;// �U���q�b�g�����ђʂ��ď����𑱍s���邩
	int damagePower;// �_���[�W��^�����
	def::charType myCharType;
	std::list<DamageData> damageList;

	SkillData(bool dependChar, bool isPenetrate, def::charType myCharType, int damage)
		:delFlag(false), dependChar(dependChar), myCharType(myCharType)
		, isPenetrate(isPenetrate), id(IDManager::CreateUniqueID()), damagePower(damage){}
	~SkillData(){}

	virtual void Process() = 0;
	virtual void Draw(){}
};

class SkillMgr{
public:
	enum SkillID{
		eID_�o�X�^�[,
		eID_�V���b�N�E�F�[�u,
		eID_�L���m��,
		eID_�n�C�L���m��,
		eID_�u�[������_����,
		eID_�u�[������_����,
		eID_�T���_�[�{�[��,
		eID_�X�g�[���L���[�u,
		eID_�w���Y���[�����O_UP,
		eID_�w���Y���[�����O_DOWN,
		eID_�t���C�����C��_�Œ�,
		eID_�\�[�h,
		eID_���J�o���[,
		eID_�_�[�N�A�[���u���[�h_SHORT,
		eID_�_�[�N�A�[���u���[�h_LONG,
		eID_�~�j�{��,
		eID_���C�h�\�[�h,
		eID_�V���[�e�B���O�o�X�^�[,

		eID_MAX
	};

	static std::shared_ptr<SkillData> GetData(int id, SkillArg args, def::charType myCharType);
	static std::shared_ptr<SkillData> GetData(ChipData c, SkillArg args, def::charType myCharType);
};
