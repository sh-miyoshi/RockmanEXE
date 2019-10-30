#pragma once

#include <memory>
#include "battleCharBase.h"

class EnemyMgr {
public:
	enum EnemyID {
		ID_�I,
		ID_���b�g�[��,
		ID_�r���[,

		ID_MAX
	};

	static std::shared_ptr<BattleCharBase> GetData(int id);
};
