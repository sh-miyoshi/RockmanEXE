#include "include.h"
#include "battleCharBase.h"
#include "animation.h"
#include "battleField.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"

bool BattleCharBase::AnimProcess() {
	if( animQueue.empty() ) {
		if( !defaultAnim ) {
			AppLogger::Error("Default Animationがセットされていません");
		}
		defaultAnim->Process();
	} else {
		auto anim = animQueue.front();
		if( !animInitialized ) {
			anim->Begin();
			animInitialized = true;
		}
		if( anim->Process() ) {// アニメーションが終了したら
			anim->End();
			animQueue.pop();
			animInitialized = false;
			return true;// 行動できる
		}
		return false;// アニメーション中は行動できない
	}
	return true;// 行動できる
}

// MoveCheck method return true if you can move to pos(x,y)
bool BattleCharBase::MoveCheck(int x, int y) {
	if( x < 0 || x >= BattleField::FIELD_NUM_X ) {
		return false;
	}

	if( y < 0 || y >= BattleField::FIELD_NUM_Y ) {
		return false;
	}

	// 移動先にすでにキャラクタがいたら
	for( auto pos : BattleCharMgr::GetInst()->GetAllCharPos(eCHAR_ALL) ) {
		if( pos == CPoint<int>(x, y) ) {
			return false;
		}
	}

	// 移動先が自分のパネルかどうか
	int panel[BattleField::LAYER_MAX];
	BattleFieldMgr::GetInst()->GetPanelInfo(panel,x,y);
	if( panel[BattleField::LAYER_USER] == BattleField::USER_PLAYER && myCharType != eCHAR_PLAYER ) {
		return false;
	}
	if( panel[BattleField::LAYER_USER] == BattleField::USER_ENEMY && myCharType != eCHAR_ENEMY ) {
		return false;
	}

	return true;// その場所に動ける
}

BattleCharBase::BattleCharBase(std::string name, unsigned int hp, unsigned int hpMax, CharType myCharType)
	:name(name), hp(hp), hpMax(hpMax), myCharType(myCharType), animInitialized(false), defaultAnim(nullptr) {
}

void BattleCharBase::Draw() {
	CPoint<int> t = BattleField::GetPixelPos(pos);

	if( animQueue.empty() ) {
		defaultAnim->Draw(t.x, t.y);
	} else {
		std::shared_ptr<Animation> anim = animQueue.front();
		anim->Draw(t.x, t.y);
	}
}

void BattleCharBase::Process() {
	// Baseは何もしない
	AnimProcess();// アニメーションだけは実行する
}

void BattleCharBase::AttachAnim(std::shared_ptr<Animation> anim, bool forceRun) {
	if( forceRun ) {
		AppLogger::Error("未実装です");
		exit(1);
	}
	animQueue.push(anim);
}

void BattleCharBase::SetHP(int hp) {
	if( hp <= 0 ) {
		this->hp = 0;
	} else {
		this->hp = ( unsigned int ) hp;
	}
}
