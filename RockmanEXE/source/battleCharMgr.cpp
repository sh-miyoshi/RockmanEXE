#include "include.h"
#include "battleCharMgr.h"
#include "playerMgr.h"
#include "battleField.h"

DamageData::DamageData():power(0), targetType(0) {
}

DamageData::DamageData(CPoint<int> pos, int power, int targetType)
	: pos(pos), power(power), targetType(targetType) {
}

DamageData::~DamageData() {
}

void BattleCharMgr::BattleInit(std::list< std::shared_ptr<BattleCharBase>> enemyList) {
	PlayerMgr::GetInst()->InitBattleChar();
	player = PlayerMgr::GetInst()->GetBattleChar();

	this->enemyList = enemyList;
}

void BattleCharMgr::BattleEnd() {
}

void BattleCharMgr::Draw() {
	player->Draw();

	for( auto enemy : enemyList ) {
		enemy->Draw();
	}
}

void BattleCharMgr::MainProcess() {
	player->Process();

	for( auto enemy : enemyList ) {
		enemy->Process();
	}

	// ダメージ処理
	for( auto damage : damageList ) {
		if( damage.targetType & eCHAR_PLAYER ) {
			if( damage.pos == player->GetPos() ) {
				printfDx("Player damaged %d\n", damage.power);
			}
		}
		if( damage.targetType & eCHAR_ENEMY ) {
			for( auto enemy : enemyList ) {
				if( damage.pos == enemy->GetPos() ) {
					printfDx("Enemy damaged %d\n", damage.power);
				}
			}
		}
		// TODO(未実装)
		//if( damage.targetType & eCHAR_OBJECT ) {

		//}
	}
	damageList.clear();// 処理が終わればすべてクリア
}

CPoint<int> BattleCharMgr::GetClosestCharPos(CPoint<int> myPos, int charType) {
	// 最大距離(x*x+y*y)+1
	static const int DIST_MAX
		= BattleField::FIELD_NUM_X * BattleField::FIELD_NUM_X
		+ BattleField::FIELD_NUM_Y * BattleField::FIELD_NUM_Y
		+ 1;

	CPoint<int> resPos(BattleField::FIELD_NUM_X+1, BattleField::FIELD_NUM_Y+1);
	int distance = DIST_MAX;
	if( charType & eCHAR_PLAYER ) {
		CPoint<int> pos = player->GetPos();
		int d = ( pos.x - myPos.x ) * ( pos.x - myPos.x ) + ( pos.y - myPos.y ) * ( pos.y - myPos.y );
		if( distance > d ) {
			resPos.x = pos.x;
			resPos.y = pos.y;
			distance = d;
		}
	}
	if( charType & eCHAR_ENEMY ) {
		for( auto enemy : enemyList ) {
			CPoint<int> pos = enemy->GetPos();
			int d = ( pos.x - myPos.x ) * ( pos.x - myPos.x ) + ( pos.y - myPos.y ) * ( pos.y - myPos.y );
			if( distance > d ) {
				resPos.x = pos.x;
				resPos.y = pos.y;
				distance = d;
			}
		}
	}

	// TODO(未実装)
	//if( charType & eCHAR_OBJECT ) {
	//	AppLogger::Error("BattleCharMgr::GetClosestCharPos 未実装");
	//	exit(1);
	//}
	return resPos;
}

std::vector<CPoint<int>> BattleCharMgr::GetAllCharPos(int charType) {
	std::vector<CPoint<int>> res;
	if( charType & eCHAR_PLAYER ) {
		res.push_back(player->GetPos());
	}
	if( charType & eCHAR_ENEMY ) {
		for( auto enemy : enemyList ) {
			res.push_back(enemy->GetPos());
		}
	}
	// TODO(未実装)
	//if( charType & eCHAR_OBJECT ) {
	//	AppLogger::Error("BattleCharMgr::GetAllCharPos 未実装");
	//	exit(1);
	//}
	return res;
}

void BattleCharMgr::RegisterDamage(DamageData data) {
	damageList.push_back(data);
}
