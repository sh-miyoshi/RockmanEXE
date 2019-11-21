#include "include.h"
#include "battleCharMgr.h"
#include "playerMgr.h"
#include "battleField.h"
#include "effectMgr.h"
#include "battleSkillMgr.h"

DamageData::DamageData():power(0), targetType(0),endCount(1),skillObjectID(-1) {
}

DamageData::DamageData(CPoint<int> pos, int power, int targetType,unsigned int endCount, int skillObjectID)
	: pos(pos), power(power), targetType(targetType),endCount(endCount), skillObjectID(skillObjectID) {
}

DamageData::~DamageData() {
}

void BattleCharMgr::BattleInit(std::vector<Battle::EnemyInfo> enemies) {
	PlayerMgr::GetInst()->InitBattleChar();
	player = PlayerMgr::GetInst()->GetBattleChar();

	std::string infoMsg = "[ ";
	for( auto e : enemies ) {
		std::shared_ptr<BattleCharBase> enemy = EnemyMgr::GetData(e.id);
		infoMsg += ToString("%s, ", enemy->GetName().c_str());
		enemy->SetPos(e.pos.x, e.pos.y);
		enemyList.push_back(enemy);
		initEnemyList.push_back(e.id);
	}
	infoMsg += "]";
	AppLogger::Info("Battle In with Enemy List: %s", infoMsg.c_str());
}

void BattleCharMgr::BattleEnd() {
}

void BattleCharMgr::Draw() {
	// プレイヤーはデリートしない
	if( !player->IsDelete() ) {
		player->Draw();
	}

	for( auto enemy : enemyList ) {
		enemy->Draw();
	}
}

void BattleCharMgr::DrawAppearScene(unsigned int nowCharNo, unsigned int count) {
	player->Draw();// 自分自身は描画

	unsigned int i = 0;
	for( auto enemy : enemyList ) {
		if( i < nowCharNo ) {// すでに登場済みのキャラクター
			enemy->Draw();
		} else if( i == nowCharNo ) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, count);
			enemy->Draw();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		i++;
	}
}

BattleCharMgr::RtnCode BattleCharMgr::MainProcess() {
	player->Process();

	for( auto it = enemyList.begin(); it != enemyList.end();) {
		if( ( *it )->IsDelete() ) {
			it = enemyList.erase(it);
		} else {
			( *it )->Process();
			it++;
		}
	}

	// ダメージ処理
	for( auto it = damageList.begin(); it != damageList.end(); ) {
		if( it->targetType & eCHAR_PLAYER ) {
			if( it->pos == player->GetPos() && !player->IsMuteki()) {
				it->endCount = 0;// 当たったらダメージは消去
				BattleSkillMgr::GetInst()->RegisterHitSkill(it->skillObjectID);
				int hp = ( int ) player->GetHP() - it->power;
				player->SetMuteki();
				player->AttachDamageAnim();// のけぞり処理(debug スーパーアーマー)
				player->SetHP(hp);
				if( hp <= 0 ) {
					return eRTN_LOSE;// 敗北
				}
			}
		}
		if( it->targetType & eCHAR_ENEMY ) {
			for( auto enemy : enemyList ) {
				if( it->pos == enemy->GetPos() ) {
					it->endCount = 0;// 当たったらダメージは消去
					BattleSkillMgr::GetInst()->RegisterHitSkill(it->skillObjectID);
					int hp = ( int ) enemy->GetHP() - it->power;
					enemy->SetHP(hp);
					if( hp <= 0 ) {
						EffectArg args;
						args.pos = BattleField::GetPixelPos(enemy->GetPos());
						args.rndSize = 30;
						args.num = 3;
						EffectMgr::GetInst()->Register(EffectMgr::eID_EnemyDeleted, args);
					}
				}
			}
		}
		// TODO(未実装)
		//if( damage.targetType & eCHAR_OBJECT ) {

		//}

		if( it->endCount > 1 ) {
			it->endCount--;
			it++;
		} else {
			it = damageList.erase(it);
		}
	}

	// 終了判定
	bool enemyDeleted = true;
	for( auto enemy : enemyList ) {
		if( enemy->GetHP() > 0 ) {
			enemyDeleted = false;
			break;
		}
	}
	if( enemyDeleted ) {
		return eRTN_WIN;// 勝利
	}
	return eRTN_CONTINUE;
}

void BattleCharMgr::RunAnimProc() {
	player->AnimProcess();

	for( auto it = enemyList.begin(); it != enemyList.end();) {
		if( ( *it )->IsDelete() ) {
			it = enemyList.erase(it);
		} else {
			( *it )->AnimProcess();
			it++;
		}
	}
}

CPoint<int> BattleCharMgr::GetClosestCharPos(CPoint<int> myPos, int charType) {
	// 最大距離(x*x+y*y)+1
	static const int DIST_MAX
		= BattleField::FIELD_NUM_X * BattleField::FIELD_NUM_X
		+ BattleField::FIELD_NUM_Y * BattleField::FIELD_NUM_Y
		+ 1;

	CPoint<int> resPos(-1, -1);
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

	if( resPos.x < 0 ) {
		AppLogger::Warn("CharType %dは見つかりませんでした。", charType);
	}

	return resPos;
}

CPoint<int> BattleCharMgr::GetClosestCharPosWithSameLine(CPoint<int> myPos, int charType) {
	CPoint<int> resPos(-1, -1);
	int distance = ( BattleField::FIELD_NUM_X + 1 ) * ( BattleField::FIELD_NUM_X + 1 );
	for( auto target : GetAllCharPos(charType) ) {
		if( charType & eCHAR_PLAYER ) {
			CPoint<int> pos = player->GetPos();
			if( pos.y == myPos.y ) {
				int d = ( pos.x - myPos.x ) * ( pos.x - myPos.x );
				if( distance > d ) {
					resPos.x = pos.x;
					resPos.y = pos.y;
					distance = d;
				}
			}
		}
		if( charType & eCHAR_ENEMY ) {
			for( auto enemy : enemyList ) {
				CPoint<int> pos = enemy->GetPos();
				if( pos.y == myPos.y ) {
					int d = ( pos.x - myPos.x ) * ( pos.x - myPos.x );
					if( distance > d ) {
						resPos.x = pos.x;
						resPos.y = pos.y;
						distance = d;
					}
				}
			}
		}
	}

	if( resPos.x < 0 ) {
		AppLogger::Warn("CharType %dは見つかりませんでした。", charType);
	}

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

void BattleCharMgr::RegisterRecovery(CPoint<int> pos, int power, CharType charType) {
	if( charType == eCHAR_ENEMY ) {
		for( auto enemy : enemyList ) {
			if( enemy->GetPos() == pos ) {
				int hp = ( int ) enemy->GetHP() + power;
				enemy->SetHP(hp);
				return;
			}
		}
	} else if( charType == eCHAR_PLAYER ) {
		if( player->GetPos() == pos ) {
			int hp = ( int ) player->GetHP() + power;
			player->SetHP(hp);
		}
	} else {
		AppLogger::Warn("Got unexpected char type: %d", charType);
	}
}
