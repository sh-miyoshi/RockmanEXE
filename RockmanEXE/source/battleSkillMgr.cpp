#include "battleSkillMgr.h"

void BattleSkillMgr::Draw() {
	for( auto s : skillList ) {
		s->Draw();
	}
}

void BattleSkillMgr::Process() {
	for( std::list<std::shared_ptr<SkillData>>::iterator it = skillList.begin(); it != skillList.end(); ) {
		if( ( *it )->Process() ) {
			it = skillList.erase(it);
		} else {
			it++;
		}
	}
}

void BattleSkillMgr::Register(std::shared_ptr<SkillData> skill) {
	skillList.push_back(skill);
}
