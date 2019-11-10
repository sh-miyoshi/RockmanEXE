#pragma once

#include "point.h"
#include "battleCharBase.h"
#include "chip.h"
#include "idManager.h"

class SkillArg {
public:
	CPoint<int> charPos;
	int power;
	CharType myCharType;
	unsigned int ariveTime;
};

class SkillData {
	int objectID;
	bool isPenetrate;// �U���q�b�g�����ђʂ��ď����𑱍s���邩
public:
	SkillData(bool isPenetrate):objectID(IDManager::CreateUniqueID()),isPenetrate(isPenetrate) {}
	~SkillData() {}

	virtual bool Process() = 0;
	virtual void Draw() {}

	int GetObjectID()const { return objectID; }
	bool GetPenetrate()const { return isPenetrate; }
};

class SkillMgr {
public:
	enum SkillID {
		eID_�o�X�^�[,
		eID_�L���m��,
		eID_�n�C�L���m��,
		eID_�V���b�N�E�F�[�u,
		eID_�T���_�[�{�[��,

		eID_MAX
	};

	static std::shared_ptr<SkillData> GetData(int id, SkillArg args);
	static std::shared_ptr<SkillData> GetData(ChipData c, SkillArg args);// Chip��񂩂�Z���擾
};
