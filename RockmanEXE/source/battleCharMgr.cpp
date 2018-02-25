#include "include.h"
#include "battleCharMgr.h"
#include "battleField.h"
#include "drawCharacter.h"

class BattleSkillMgr{
	std::list<std::shared_ptr<SkillData>> skillInfo;
	std::list<DamageData> damageInfo;

public:
	BattleSkillMgr(){}
	~BattleSkillMgr(){}

	void Draw();
	void Process();
	void RegisterAttack(std::shared_ptr<SkillData> skill);
	std::list<DamageData> GetDamageInfo()const{ return damageInfo; }
	void UpdateDamageInfo(std::list<DamageData> damageInfo){ this->damageInfo = damageInfo; }
};

//--------------------------------------
// BattleCharMgr
//--------------------------------------
BattleCharMgr::BattleCharMgr():skillMgr(nullptr){
}

BattleCharMgr::~BattleCharMgr(){
}

void BattleCharMgr::Init(std::list<BattleEnemyInit_t> enemyInfo){
	if( skillMgr )
		ASSERT(0,"BattleCharMgr::Init Game Logic Error(Please call End method)");
	skillMgr = new BattleSkillMgr();

	objects.clear();
	enemys.clear();
	enemyChips.clear();

	player = PlayerMgr::GetInst()->CreateBattlePlayer();
	player->LoadAnim();
	player->SetPos(CPoint<int>(1, 1));
	player->ShuffleFolder();

	std::string position = "BattleCharMgr::Init";
	std::string fname = def::IMAGE_FILE_PATH + "mind_status.png";
	int temp[18];
	LoadDivGraphWithCheckError(temp, fname, position, 6, 3, 88, 32);
	for( int i = MIND_MAX; i < 18; i++ )
		DeleteGraph(temp[i]);
	for( int i = 0; i < MIND_MAX; i++ )
		imgMindStatus[i] = temp[i];
	fname = def::IMAGE_FILE_PATH + "hp_frame.png";
	imgHpFrame = LoadGraphWithCheckError(fname, position);

	// 敵キャラの追加
	for( std::list<BattleEnemyInit_t>::iterator it = enemyInfo.begin(); it != enemyInfo.end(); it++ ){
		std::shared_ptr<BattleCharBase> t = EnemyMgr::GetData(it->id);
		t->LoadAnim();
		t->SetPos(it->pos);
		enemys.push_back(t);
		enemyChips=merge<EnemyMgr::EnemyAttackChip_t>(enemyChips, EnemyMgr::GetEnemyChip(it->id));
	}
}

void BattleCharMgr::End(){
	if( !skillMgr )
		ASSERT(0, "BattleCharMgr::Init Game Logic Error(Please call Init method)");
	delete skillMgr;
	skillMgr = nullptr;

	player->DeleteAnim();
	for( std::vector<std::shared_ptr<BattleObject>>::iterator it = objects.begin(); it != objects.end(); it++ ){
		( *it )->DeleteAnim();
	}
	for( std::vector<std::shared_ptr<BattleCharBase>>::iterator it = enemys.begin(); it != enemys.end(); it++ ){
		( *it )->DeleteAnim();
	}
	objects.clear();
	enemys.clear();
	enemyChips.clear();

	for( int i = 0; i < sizeof(imgMindStatus) / sizeof(imgMindStatus[0]); i++ )
		DeleteGraph(imgMindStatus[i]);
	DeleteGraph(imgHpFrame);
}

void BattleCharMgr::DrawObjects(int drawNum, int count){
	int i = 0;
	for( std::vector<std::shared_ptr<BattleObject>>::iterator it = objects.begin(); it != objects.end(); it++, i++ ){
		CPoint<int> pos = ( *it )->GetPos();
		int x = BattleField::PANEL_SIZE.x*pos.x + BattleField::PANEL_SIZE.x / 2;
		int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*( pos.y + 1 ) - 20;
		( *it )->Draw();
	}
	i = 0;
	for( std::vector<std::shared_ptr<BattleCharBase>>::iterator it = enemys.begin(); it != enemys.end(); it++, i++ ){
		CPoint<int> pos = ( *it )->GetPos();
		int x = BattleField::PANEL_SIZE.x*pos.x + BattleField::PANEL_SIZE.x / 2;
		int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*( pos.y + 1 ) - 20;
		if( drawNum >= 0 ){
			if( i == drawNum - 1 ){
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, count);
			} else if( i > drawNum - 1 )// キャラを表示しない
				break;
		}
		( *it )->Draw();
		DrawCharacter::GetInst()->DrawNumberCenter(x, y, ( *it )->GetHP(), DrawCharacter::COL_WHITE_SMALL);// HPの描画
		if( drawNum >= 0 ){
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	if( !player->isDelete() ){
		player->Draw();
		skillMgr->Draw();
	}
}

BattleRtnCode BattleCharMgr::Process(){
	skillMgr->Process();
	std::list<DamageData> damageInfo = skillMgr->GetDamageInfo();

	if( player->DamageProc(damageInfo) ){
		// todo(キャラ依存のスキルを中止)
	}
	if( player->isDelete() ){
		return eBATTLE_RTN_LOSE;
	}
	player->Process();

	for( std::vector<std::shared_ptr<BattleObject>>::iterator it = objects.begin(); it != objects.end();){
		( *it )->DamageProc(damageInfo);
		if( ( *it )->isDelete() )
			it = objects.erase(it);
		else{
			( *it )->Process();
			it++;
		}
	}

	for( std::vector<std::shared_ptr<BattleCharBase>>::iterator it = enemys.begin(); it != enemys.end();){
		if( ( *it )->DamageProc(damageInfo) ){
			// todo(キャラ依存のスキルを中止)
		}
		if( ( *it )->isDelete() )
			it = enemys.erase(it);
		else{
			( *it )->Process();
			it++;
		}
	}
	if( enemys.empty() ){
		return eBATTLE_RTN_WIN;
	}

	skillMgr->UpdateDamageInfo(damageInfo);

	return eBATTLE_RTN_CONTINUE;
}

bool BattleCharMgr::IsAnyChar(CPoint<int> pos){
	// 生きているオブジェクトのみ対象
	if( player->GetPos() == pos && player->GetHP() > 0 )
		return true;
	for( std::vector<std::shared_ptr<BattleObject>>::iterator it = objects.begin(); it != objects.end(); it++ )
		if( ( *it )->GetPos() == pos && ( *it )->GetHP() > 0 )
			return true;
	for( std::vector<std::shared_ptr<BattleCharBase>>::iterator it = enemys.begin(); it != enemys.end(); it++ )
		if( ( *it )->GetPos() == pos && ( *it )->GetHP() > 0 )
			return true;
	return false;
}

int BattleCharMgr::GetTargetX(int y, int target){
	if( target&def::eCHAR_PLAYER ){
		if( player->GetHP() > 0 )
			return -1;
		else if( y == player->GetPos().y )
			return player->GetPos().x;
	}

	int x = BattleField::FIELD_NUM_X;

	if( target&def::eCHAR_OBJECT ){
		for( std::vector<std::shared_ptr<BattleObject>>::iterator it = objects.begin(); it != objects.end(); it++ ){
			if( ( *it )->GetPos().y == y && ( *it )->GetPos().x < x && ( *it )->GetHP() > 0 )
				x = ( *it )->GetPos().x;
		}
	}
	if( target&def::eCHAR_ENEMY ){
		for( std::vector<std::shared_ptr<BattleCharBase>>::iterator it = enemys.begin(); it != enemys.end(); it++ ){
			if( ( *it )->GetPos().y == y && ( *it )->GetPos().x < x && ( *it )->GetHP() > 0 )
				x = ( *it )->GetPos().x;
		}
	}

	if( x == BattleField::FIELD_NUM_X )
		x = -1;// ターゲットはいない
	return x;
}

CPoint<int> BattleCharMgr::GetTargetPos(CPoint<int> myPos, def::charType target){
	// 最大距離(x*x+y*y)+1
	static const int DIST_MAX
		= BattleField::FIELD_NUM_X*BattleField::FIELD_NUM_X
		+ BattleField::FIELD_NUM_Y*BattleField::FIELD_NUM_Y
		+ 1;

	CPoint<int> pos(-1, -1);
	int distance = DIST_MAX;
	for( int y = 0; y < BattleField::FIELD_NUM_Y; y++ ){
		for( int x = 0; x < BattleField::FIELD_NUM_X; x++ ){
			if( target&def::eCHAR_PLAYER ){
				if( player->GetPos() == CPoint<int>(x, y) && player->GetHP() > 0 ){
					int d = ( x - myPos.x )*( x - myPos.x ) + ( y - myPos.y )*( y - myPos.y );
					if( distance > d ){
						pos.x = x;
						pos.y = y;
						distance = d;
					}
				}
			}
			if( target&def::eCHAR_ENEMY ){
				for( std::vector<std::shared_ptr<BattleCharBase>>::iterator it = enemys.begin(); it != enemys.end(); it++ ){
					if( ( *it )->GetPos() == CPoint<int>(x, y) ){
						int d = ( x - myPos.x )*( x - myPos.x ) + ( y - myPos.y )*( y - myPos.y );
						if( distance > d && ( *it )->GetHP() > 0 ){
							pos.x = x;
							pos.y = y;
							distance = d;
						}
					}
				}
			}
			if( target&def::eCHAR_OBJECT ){
				for( std::vector<std::shared_ptr<BattleObject>>::iterator it = objects.begin(); it != objects.end(); it++ ){
					if( ( *it )->GetPos() == CPoint<int>(x, y) ){
						int d = ( x - myPos.x )*( x - myPos.x ) + ( y - myPos.y )*( y - myPos.y );
						if( distance > d && ( *it )->GetHP() > 0 ){
							pos.x = x;
							pos.y = y;
							distance = d;
						}
					}
				}
			}
		}
	}
	return pos;
}

int BattleCharMgr::GetCondPos(CPoint<int>* result, int max, def::charType charType, std::function<bool(CPoint<int>)> cond){
	if( max == 0 )
		return 0;

	int cnt = 0;
	if( charType&def::eCHAR_PLAYER ){
		if( cond(player->GetPos()) && player->GetHP() > 0 ){
			result[cnt++] = player->GetPos();
			if( cnt >= max )
				return cnt;
		}
	}

	if( charType&def::eCHAR_ENEMY ){
		for( std::vector<std::shared_ptr<BattleCharBase>>::iterator it = enemys.begin(); it != enemys.end(); it++ ){
			if( cond(( *it )->GetPos()) && ( *it )->GetHP() > 0 ){
				result[cnt++] = ( *it )->GetPos();
				if( cnt >= max )
					return cnt;
			}
		}
	}
	if( charType&def::eCHAR_OBJECT ){
		for( std::vector<std::shared_ptr<BattleObject>>::iterator it = objects.begin(); it != objects.end(); it++ ){
			if( cond(( *it )->GetPos()) && ( *it )->GetHP() > 0 ){
				result[cnt++] = ( *it )->GetPos();
				if( cnt >= max )
					return cnt;
			}
		}
	}

	return cnt;
}

void BattleCharMgr::RegisterObject(std::shared_ptr<BattleObject> object){
	objects.push_back(object);
}

void BattleCharMgr::DrawPlayerState(CPoint<int> pos){
	// ロックマンの心ウィンドウの描画
	DrawGraph(pos.x, pos.y, imgMindStatus[player->GetMindState()], TRUE);

	// ロックマンのHP描画
	DrawGraph(pos.x, 5, imgHpFrame, TRUE);
	DrawCharacter::Color hp_col = ( player->GetHP() < player->GetHPMax() / 3 ) ? DrawCharacter::COL_RED : DrawCharacter::COL_WHITE;
	DrawCharacter::GetInst()->DrawNumber(pos.x + 7, 7, player->GetHP(), hp_col, 4);
}

std::vector<ChipInfo> BattleCharMgr::GetPlayerHand(int max){
	return player->GetHandData(max);
}

void BattleCharMgr::SendActionChips(std::vector<unsigned> selectedIndexes){
	player->SendChips(selectedIndexes);
}

void BattleCharMgr::DrawPlayerSendChipIcon(){
	player->DrawSendChipIcon();
}

void BattleCharMgr::RegisterAttack(std::shared_ptr<SkillData> skill){
	skillMgr->RegisterAttack(skill);
}

//--------------------------------------
// BattleSkillMgr
//--------------------------------------
void BattleSkillMgr::Draw(){
	// 攻撃の描画
	for( std::list<std::shared_ptr<SkillData>>::iterator it = skillInfo.begin(); it != skillInfo.end(); it++ ){
		if( !( *it )->delFlag )
			( *it )->Draw();
	}
}

void BattleSkillMgr::Process(){
	// ダメージ処理
	std::vector<int> eraseSkillID;
	for( std::list<DamageData>::iterator it = damageInfo.begin(); it != damageInfo.end();){
		it->delCount--;
		if( it->forceDelFlag ){// オブジェクトに当たって消える
			eraseSkillID.push_back(it->skillID);// 削除するスキルの登録
			it = damageInfo.erase(it);
		} else if( it->delCount <= 0 )
			it = damageInfo.erase(it);
		else
			it++;
	}

	// スキルに関する処理
	for( std::list<std::shared_ptr<SkillData>>::iterator it = skillInfo.begin(); it != skillInfo.end();){
		bool forceDelFlag = false;
		// オブジェクトに当たっていて、貫通攻撃でないなら
		if( find<int>(eraseSkillID, ( *it )->id) && !( *it )->isPenetrate )
			forceDelFlag = true;

		if( ( *it )->delFlag || forceDelFlag ){
			// StopCharMotion(it->charID);// キャラのモーションをストップ
			it = skillInfo.erase(it);
		} else{
			( *it )->Process();

			// 攻撃情報
			for( std::list<DamageData>::iterator dit = ( *it )->damageList.begin(); dit != ( *it )->damageList.end(); dit++ )
				damageInfo.push_back(*dit);
			( *it )->damageList.clear();// ダメージ情報は毎回初期化
			it++;
		}
	}
}

void BattleSkillMgr::RegisterAttack(std::shared_ptr<SkillData> skill){
	skillInfo.push_back(skill);
}