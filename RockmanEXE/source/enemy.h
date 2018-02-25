#pragma once

#include <memory>
#include <list>
#include "battleCharBase.h"
#include "chip.h"

class EnemyMgr{
public:
	enum EnemyID{
		ID_���b�g�[��,
		ID_�r���[,
		ID_�t�H���e,

		ID_MAX
	};

	typedef struct tag_EnemyAttackChip{
		int bustingLevel;// �K�v�Œ���̃o�X�e�B���O���x��
		int charID;
		ChipInfo chip;

		bool operator<(const struct tag_EnemyAttackChip& right) const{
			return charID<right.charID;
		}
	}EnemyAttackChip_t;

	static std::shared_ptr<BattleCharBase> GetData(int id);
	static std::list<EnemyAttackChip_t> GetEnemyChip(int id);
};
