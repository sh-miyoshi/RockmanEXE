#pragma once

#include <memory>
#include <list>
#include "battleCharBase.h"
#include "chip.h"

class EnemyMgr {
public:
	enum EnemyID {
		ID_�I,
		ID_���b�g�[��,
		ID_�r���[,

		ID_MAX
	};

	static std::shared_ptr<BattleCharBase> GetData(int id);
	static std::list<ChipInfo> GetChips(int id, unsigned int bustingLevel);
};
