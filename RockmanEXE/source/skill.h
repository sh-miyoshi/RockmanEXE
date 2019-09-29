#pragma once

#include "point.h"
#include "battleCharBase.h"

class SkillArg {
public:
	CPoint<int> charPos;
	int power;
	int targetType;
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

		eID_MAX
	};

	static std::shared_ptr<SkillData> GetData(int id, SkillArg args);
	//	static std::shared_ptr<SkillData> GetData(ChipData c, SkillArg args);// TODO(Chip��񂩂�Z���擾)
};
