#include "include.h"
#include "battleCharBase.h"
#include "animation.h"
#include "battleField.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"

bool BattleCharBase::AnimProcess() {
	// Delete用アニメーション
	if( hp <= 0 ) {
		deleteCount++;
		return false;
	}

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
	BattleFieldMgr::GetInst()->GetPanelInfo(panel, x, y);
	if( panel[BattleField::LAYER_USER] == BattleField::USER_PLAYER && myCharType != eCHAR_PLAYER ) {
		return false;
	}
	if( panel[BattleField::LAYER_USER] == BattleField::USER_ENEMY && myCharType != eCHAR_ENEMY ) {
		return false;
	}

	return true;// その場所に動ける
}

BattleCharBase::BattleCharBase(std::string name, unsigned int hp, unsigned int hpMax, CharType myCharType)
	:name(name), hp(hp), hpMax(hpMax), myCharType(myCharType), animInitialized(false)
	, defaultAnim(nullptr), deleteCount(0){
}

void BattleCharBase::Draw() {
	CPoint<int> t = BattleField::GetPixelPos(pos);
	if( hp == 0 ) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		Draw(t.x, t.y);// 光を強くするために通常より描画
		Draw(t.x, t.y);// 光を強くするために通常より描画
	}

	Draw(t.x, t.y);

	if( hp == 0 ) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void BattleCharBase::Draw(int pixelX, int pixelY) {
	if( animQueue.empty() ) {
		defaultAnim->Draw(pixelX, pixelY);
	} else {
		std::shared_ptr<Animation> anim = animQueue.front();
		anim->Draw(pixelX, pixelY);
	}
}

void BattleCharBase::Process() {
	// Baseは何もしない
	AnimProcess();// アニメーションだけは実行する
}

void BattleCharBase::AttachAnim(std::shared_ptr<Animation> anim, bool forceRun) {
	if( forceRun ) {
		// 既存のアニメーションを全削除
		while( !animQueue.empty() ) {
			animQueue.pop();
		}
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
