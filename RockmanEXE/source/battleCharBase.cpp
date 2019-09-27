#include "include.h"
#include "battleCharBase.h"
#include "animation.h"

void BattleCharBase::AnimProcess() {
	if( animQueue.empty() ) {
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
		}
	}
}

// MoveCheck method return true if you can move to pos(x,y)
bool BattleCharBase::MoveCheck(int x, int y){
	if(x<0 || x>=def::BATTLE_FILED_NUM.x){
		return false;
	}

	if(y<0 || x>=def::BATTLE_FILED_NUM.y){
		return false;
	}

	// TODO(その他のチェック)
	//   IsAnyChar?
	//   移動先が自分のパネルかどうか

	return true;// その場所に動ける
}

BattleCharBase::BattleCharBase(std::string name, unsigned int hp, unsigned int hpMax, CharType myCharType, std::shared_ptr<Animation> defaultAnim)
	:name(name), hp(hp), hpMax(hpMax), myCharType(myCharType), defaultAnim(defaultAnim), animInitialized(false){
}

void BattleCharBase::Draw() {
	int x = def::BATTLE_PANEL_SIZE.x * pos.x + def::BATTLE_PANEL_SIZE.x / 2;
	int y = def::BATTLE_PANEL_OFFSET_Y + def::BATTLE_PANEL_SIZE.y * pos.y - 10;// ふちの分だけ上げる

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
