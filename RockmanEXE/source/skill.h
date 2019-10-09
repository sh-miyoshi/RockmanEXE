#pragma once

#include "point.h"
#include "battleCharBase.h"
#include "chip.h"

class SkillArg {
public:
	CPoint<int> charPos;
	int power;
	CharType myCharType;
};

class SkillData {
public:
	SkillData() {}
	~SkillData() {}

	virtual bool Process() = 0;
	virtual void Draw() {}
};

class SkillMgr {
public:
	enum SkillID {
		eID_�o�X�^�[,
		eID_�L���m��,
		eID_�n�C�L���m��,

		eID_MAX
	};

	static std::shared_ptr<SkillData> GetData(int id, SkillArg args);
	static std::shared_ptr<SkillData> GetData(ChipData c, SkillArg args);// Chip��񂩂�Z���擾
};
