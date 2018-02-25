#include "include.h"
#include "battleCharBase.h"
#include "battleField.h"
#include "battleFieldMgr.h"
#include "battleCharMgr.h"

//---------------------------------------------
// 外部関数
//---------------------------------------------
BattleCharBase::BattleCharBase(std::string name, int hp, def::charType myCharType)
	:name(name), mutekiCnt(0), mutekiEndCnt(0), deleteCount(0)
	, BattleObject(hp, CPoint<int>(), GetDamagedType(myCharType), myCharType){
}

bool BattleCharBase::DamageProc(std::list<DamageData>& damageList){
	if( hp <= 0 ){
		mutekiCnt = 0;
		return false;
	}

	// 無敵時処理
	if( mutekiEndCnt > 0 ){
		mutekiCnt++;
		if( mutekiCnt > mutekiEndCnt ){
			mutekiCnt = 0;
			mutekiEndCnt = 0;
		}
		// インビジ貫通攻撃以外削除してダメージプロセスへ
		for( std::list<DamageData>::iterator it = damageList.begin(); it != damageList.end();){
			// debug(インビジ貫通攻撃は未実装)
			it = damageList.erase(it);
		}
		BattleObject::DamageProc(damageList);
		return false;
	}

	// 現在: true(のけぞる) false(のけぞらない)
	return BattleObject::DamageProc(damageList);// debug: enumで書くべきかな
}

void BattleCharBase::Draw(){
	if( mutekiCnt % 2 == 0 )
		BattleObject::Draw();
}

void BattleCharBase::Process(){
	if( hp <= 0 ){
		static const int DELETE_MOTION_TIME = 30;
		if( deleteCount > DELETE_MOTION_TIME ){
			delFlag = true;
		} else
			deleteCount++;
	}
}

//---------------------------------------------
// 内部関数
//---------------------------------------------
def::charType BattleCharBase::GetDamagedType(def::charType myCharType){
	switch( myCharType ){
	case def::eCHAR_ENEMY:
		return def::eCHAR_PLAYER;
	case def::eCHAR_PLAYER:
		return def::eCHAR_ENEMY;
	}
	ASSERT(0, "BattleCharBase::GetDamagedType invalied charType (" + ToString<int>(myCharType) + ")");
	return def::eCHAR_ALL;
}

int BattleCharBase::GetMyPanel(def::charType myCharType){
	switch( myCharType ){
	case def::eCHAR_ENEMY:
		return BattleField::USER_ENEMY;
	case def::eCHAR_PLAYER:
		return BattleField::USER_PLAYER;
	}
	ASSERT(0, "BattleCharBase::GetMyPanel invalied charType (" + ToString<int>(myCharType) + ")");
	return -1;
}

bool BattleCharBase::Move(int direct, bool isMove){
	switch( direct ) {
	case def::eMUKI_UP:
		if( pos.y > 0 && MoveCheck(pos.x, pos.y - 1) ) {
			if( isMove )
				pos.y--;
			return true;
		}
		break;
	case def::eMUKI_DOWN:
		if( pos.y < BattleField::FIELD_NUM_Y - 1 && MoveCheck(pos.x, pos.y + 1) ) {
			if( isMove )
				pos.y++;
			return true;
		}
		break;
	case def::eMUKI_LEFT:
		if( pos.x > 0 && MoveCheck(pos.x - 1, pos.y) ) {
			if( isMove )
				pos.x--;
			return true;
		}
		break;
	case def::eMUKI_RIGHT:
		if( pos.x < BattleField::FIELD_NUM_X - 1 && MoveCheck(pos.x + 1, pos.y) ) {
			if( isMove )
				pos.x++;
			return true;
		}
		break;
	}
	return false;// 動けない
}

bool BattleCharBase::MoveCheck(int x, int y, bool checkPanel){
	// checkPanelがtrueの時だけmyPanelをチェック
	int myPanel = GetMyPanel(myCharType);

	int panelInfo[BattleField::LAYER_MAX];
	BattleFieldMgr::GetInst()->GetPanelInfo(panelInfo, x, y);// パネル情報の取得
	// 移動先が自分のパネルで、かつキャラクタがいなければ
	if( checkPanel&&panelInfo[BattleField::LAYER_USER] != myPanel )
		return false;
	if( !BattleCharMgr::GetInst()->IsAnyChar(CPoint<int>(x, y)) ) {
		// debug(歩けるかチェック)
		// debug(フロート、エア)
		return true;
	} else
		return false;
}