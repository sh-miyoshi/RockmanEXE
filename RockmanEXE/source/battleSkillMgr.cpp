#include <algorithm>
#include "include.h"
#include "battleSkillMgr.h"

void BattleSkillMgr::Draw() {
	for( auto s : skillList ) {
		s->Draw();
	}
}

void BattleSkillMgr::Process() {
	for( std::list<std::shared_ptr<SkillData>>::iterator it = skillList.begin(); it != skillList.end(); ) {
		// ‚à‚µƒXƒLƒ‹‚ªƒqƒbƒg‚µ‚Ä‚¢‚ÄŠÑ’ÊUŒ‚‚Å‚È‚¢‚È‚ç
		if( std::find(hitList.begin(), hitList.end(), ( *it )->GetObjectID())!=hitList.end() ) {
			if( !( *it )->GetPenetrate() ) {
				it = skillList.erase(it);
				continue;
			}
		}
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

void BattleSkillMgr::RegisterHitSkill(int objectID) {
	if( objectID >= 0 ) {
		hitList.push_back(objectID);
	} else {
		AppLogger::Error("Hit Skill ObjectID is not initialized");
	}
}
