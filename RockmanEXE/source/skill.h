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
	bool isPenetrate;// 攻撃ヒット時も貫通して処理を続行するか
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
		eID_バスター,
		eID_キャノン,
		eID_ハイキャノン,
		eID_ショックウェーブ,
		eID_サンダーボール,

		eID_MAX
	};

	static std::shared_ptr<SkillData> GetData(int id, SkillArg args);
	static std::shared_ptr<SkillData> GetData(ChipData c, SkillArg args);// Chip情報から技を取得
};
