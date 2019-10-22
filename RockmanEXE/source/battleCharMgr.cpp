#include "include.h"
#include "battleCharMgr.h"
#include "playerMgr.h"
#include "battleField.h"
#include "effectMgr.h"

DamageData::DamageData():power(0), targetType(0),endCount(1) {
}

DamageData::DamageData(CPoint<int> pos, int power, int targetType,unsigned int endCount)
	: pos(pos), power(power), targetType(targetType),endCount(endCount) {
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

void BattleCharMgr::DrawAppearScene(unsigned int nowCharNo, unsigned int count) {
	player->Draw();// �������g�͕`��

	unsigned int i = 0;
	for( auto enemy : enemyList ) {
		if( i < nowCharNo ) {// ���łɓo��ς݂̃L�����N�^�[
			enemy->Draw();
		} else if( i == nowCharNo ) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, count);
			enemy->Draw();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

BattleCharMgr::RtnCode BattleCharMgr::MainProcess() {
	player->Process();

	for( auto enemy : enemyList ) {
		enemy->Process();
	}

	// �_���[�W����
	for( auto it = damageList.begin(); it != damageList.end(); ) {
		if( it->targetType & eCHAR_PLAYER ) {
			if( it->pos == player->GetPos() ) {
				int hp = ( int ) player->GetHP() - it->power;
				player->SetHP(hp);
				if( hp <= 0 ) {
					return eRTN_LOSE;// �s�k
				}
			}
		}
		if( it->targetType & eCHAR_ENEMY ) {
			for( auto enemy : enemyList ) {
				if( it->pos == enemy->GetPos() ) {
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
		// TODO(������)
		//if( damage.targetType & eCHAR_OBJECT ) {

		//}

		if( it->endCount > 1 ) {
			it->endCount--;
			it++;
		} else {
			it = damageList.erase(it);
		}
	}

	// �I������
	bool enemyDeleted = true;
	for( auto enemy : enemyList ) {
		if( enemy->GetHP() > 0 ) {
			enemyDeleted = false;
			break;
		}
	}
	if( enemyDeleted ) {
		return eRTN_WIN;// ����
	}
	return eRTN_CONTINUE;
}

void BattleCharMgr::RunAnimProc() {
	player->AnimProcess();

	for( auto enemy : enemyList ) {
		enemy->AnimProcess();
	}
}

CPoint<int> BattleCharMgr::GetClosestCharPos(CPoint<int> myPos, int charType) {
	// �ő勗��(x*x+y*y)+1
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

	// TODO(������)
	//if( charType & eCHAR_OBJECT ) {
	//	AppLogger::Error("BattleCharMgr::GetClosestCharPos ������");
	//	exit(1);
	//}

	if( resPos.x < 0 ) {
		AppLogger::Warn("CharType %d�͌�����܂���ł����B", charType);
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
		AppLogger::Warn("CharType %d�͌�����܂���ł����B", charType);
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
	// TODO(������)
	//if( charType & eCHAR_OBJECT ) {
	//	AppLogger::Error("BattleCharMgr::GetAllCharPos ������");
	//	exit(1);
	//}
	return res;
}

void BattleCharMgr::RegisterDamage(DamageData data) {
	damageList.push_back(data);
}
