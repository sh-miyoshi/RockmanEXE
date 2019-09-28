#pragma once

#include <memory>
#include "battleCharBase.h"

class EnemyMgr {
public:
	enum EnemyID {
		ID_���b�g�[��,

		ID_MAX
	};

	static std::shared_ptr<BattleCharBase> GetData(int id);
};
