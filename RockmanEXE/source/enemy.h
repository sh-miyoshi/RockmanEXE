#pragma once

#include <memory>
#include "battleCharBase.h"

class EnemyMgr {
public:
	enum EnemyID {
		ID_メットール,

		ID_MAX
	};

	static std::shared_ptr<BattleCharBase> GetData(int id);
};
