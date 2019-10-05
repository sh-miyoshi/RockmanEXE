#include "include.h"
#include "battleCharBase.h"
#include "animation.h"
#include "battleField.h"

bool BattleCharBase::AnimProcess() {
	if( animQueue.empty() ) {
		if( !defaultAnim ) {
			AppLogger::Error("Default Animation���Z�b�g����Ă��܂���");
		}
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
			return true;// �s���ł���
		}
		return false;// �A�j���[�V�������͍s���ł��Ȃ�
	}
	return true;// �s���ł���
}

// MoveCheck method return true if you can move to pos(x,y)
bool BattleCharBase::MoveCheck(int x, int y) {
	if( x < 0 || x >= BattleField::FIELD_NUM_X ) {
		return false;
	}

	if( y < 0 || y >= BattleField::FIELD_NUM_Y ) {
		return false;
	}

	// TODO(���̑��̃`�F�b�N)
	//   IsAnyChar?
	//   �ړ��悪�����̃p�l�����ǂ���

	return true;// ���̏ꏊ�ɓ�����
}

BattleCharBase::BattleCharBase(std::string name, unsigned int hp, unsigned int hpMax, CharType myCharType)
	:name(name), hp(hp), hpMax(hpMax), myCharType(myCharType), animInitialized(false), defaultAnim(nullptr) {
}

void BattleCharBase::Draw() {
	int x = BattleField::PANEL_SIZE.x * pos.x + BattleField::PANEL_SIZE.x / 2;
	int y = BattleField::BATTLE_PANEL_OFFSET_Y + BattleField::PANEL_SIZE.y * pos.y - 10;// �ӂ��̕������グ��

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

void BattleCharBase::SetHP(int hp) {
	if( hp <= 0 ) {
		this->hp = 0;
	} else {
		this->hp = ( unsigned int ) hp;
	}
}
