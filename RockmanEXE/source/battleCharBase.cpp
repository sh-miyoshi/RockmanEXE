#include "include.h"
#include "battleCharBase.h"
#include "animation.h"
#include "battleField.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"

bool BattleCharBase::AnimProcess() {
	// Delete�p�A�j���[�V����
	if( hp <= 0 ) {
		deleteCount++;
		return false;
	}

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

	// �ړ���ɂ��łɃL�����N�^��������
	for( auto pos : BattleCharMgr::GetInst()->GetAllCharPos(eCHAR_ALL) ) {
		if( pos == CPoint<int>(x, y) ) {
			return false;
		}
	}

	// �ړ��悪�����̃p�l�����ǂ���
	int panel[BattleField::LAYER_MAX];
	BattleFieldMgr::GetInst()->GetPanelInfo(panel, x, y);
	if( panel[BattleField::LAYER_USER] == BattleField::USER_PLAYER && myCharType != eCHAR_PLAYER ) {
		return false;
	}
	if( panel[BattleField::LAYER_USER] == BattleField::USER_ENEMY && myCharType != eCHAR_ENEMY ) {
		return false;
	}

	return true;// ���̏ꏊ�ɓ�����
}

BattleCharBase::BattleCharBase(std::string name, unsigned int hp, unsigned int hpMax, CharType myCharType)
	:name(name), hp(hp), hpMax(hpMax), myCharType(myCharType), animInitialized(false)
	, defaultAnim(nullptr), deleteCount(0){
}

void BattleCharBase::Draw() {
	CPoint<int> t = BattleField::GetPixelPos(pos);
	if( hp == 0 ) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		Draw(t.x, t.y);// �����������邽�߂ɒʏ���`��
		Draw(t.x, t.y);// �����������邽�߂ɒʏ���`��
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
	// Base�͉������Ȃ�
	AnimProcess();// �A�j���[�V���������͎��s����
}

void BattleCharBase::AttachAnim(std::shared_ptr<Animation> anim, bool forceRun) {
	if( forceRun ) {
		// �����̃A�j���[�V������S�폜
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
