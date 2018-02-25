#pragma once

#include <memory>
#include <list>
#include "battleCharBase.h"
#include "chip.h"

class EnemyMgr{
public:
	enum EnemyID{
		ID_メットール,
		ID_ビリー,
		ID_フォルテ,

		ID_MAX
	};

	typedef struct tag_EnemyAttackChip{
		int bustingLevel;// 必要最低限のバスティングレベル
		int charID;
		ChipInfo chip;

		bool operator<(const struct tag_EnemyAttackChip& right) const{
			return charID<right.charID;
		}
	}EnemyAttackChip_t;

	static std::shared_ptr<BattleCharBase> GetData(int id);
	static std::list<EnemyAttackChip_t> GetEnemyChip(int id);
};
