#pragma once

#include <memory>
#include <list>
#include "battleCharBase.h"
#include "chip.h"

class EnemyMgr {
public:
	enum EnemyID {
		ID_的,
		ID_メットール,
		ID_ビリー,

		ID_MAX
	};

	static std::shared_ptr<BattleCharBase> GetData(int id);
	static std::list<ChipInfo> GetChips(int id, unsigned int bustingLevel);
};
