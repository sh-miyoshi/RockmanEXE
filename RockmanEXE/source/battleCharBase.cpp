#include "include.h"
#include "battleCharBase.h"
#include "animation.h"
#include "battleField.h"

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

	// TODO(その他のチェック)
	//   IsAnyChar?
	//   移動先が自分のパネルかどうか

	return true;// その場所に動ける
}

BattleCharBase::BattleCharBase(std::string name, unsigned int hp, unsigned int hpMax, CharType myCharType)
	:name(name), hp(hp), hpMax(hpMax), myCharType(myCharType), animInitialized(false), defaultAnim(nullptr) {
}

void BattleCharBase::Draw() {
	int x = BattleField::PANEL_SIZE.x * pos.x + BattleField::PANEL_SIZE.x / 2;
	int y = BattleField::BATTLE_PANEL_OFFSET_Y + BattleField::PANEL_SIZE.y * pos.y - 10;// ふちの分だけ上げる

	if( animQueue.empty() ) {
		defaultAnim->Draw(x, y);
	} else {
		std::shared_ptr<Animation> anim = animQueue.front();
		anim->Draw(x, y);
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
