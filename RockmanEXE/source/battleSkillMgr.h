#pragma once

#include <list>
#include "skill.h"

class BattleSkillMgr {
	std::list<std::shared_ptr<SkillData>> skillList;
	std::list<int> hitList;

	BattleSkillMgr() {}
	~BattleSkillMgr() {}
public:
	static BattleSkillMgr* GetInst() {
		static BattleSkillMgr inst;
		return &inst;
	}

	void Draw();
	void Process();
	void Register(std::shared_ptr<SkillData> skill);
	void RegisterHitSkill(int objectID);
};