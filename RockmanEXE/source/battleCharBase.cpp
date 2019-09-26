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
		if( anim->Process() ) {// �A�j���[�V�������I��������
			anim->End();
			animQueue.pop();
			animInitialized = false;
		}
	}
}

BattleCharBase::BattleCharBase(std::string name, unsigned int hp, unsigned int hpMax, CharType myCharType, std::shared_ptr<Animation> defaultAnim)
	:name(name), hp(hp), hpMax(hpMax), myCharType(myCharType), defaultAnim(defaultAnim), animInitialized(false){
}

void BattleCharBase::Draw() {
	int x = def::BATTLE_PANEL_SIZE.x * pos.x + def::BATTLE_PANEL_SIZE.x / 2;
	int y = def::BATTLE_PANEL_OFFSET_Y + def::BATTLE_PANEL_SIZE.y * pos.y - 10;// �ӂ��̕������グ��

	if( animQueue.empty() ) {
		defaultAnim->Draw(x, y);
	} else {
		std::shared_ptr<Animation> anim = animQueue.front();
		anim->Draw(x, y);
	}
}

void BattleCharBase::Process() {
	// Base�͉������Ȃ�
	AnimProcess();// �A�j���[�V���������͎��s����
}

void BattleCharBase::AttachAnim(std::shared_ptr<Animation> anim, bool forceRun) {
	if( forceRun ) {
		AppLogger::Error("�������ł�");
		exit(1);
	}
	animQueue.push(anim);
}
