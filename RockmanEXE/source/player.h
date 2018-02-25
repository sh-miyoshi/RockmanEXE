#pragma once

#include <vector>
#include <map>
#include "battleCharBase.h"
#include "chip.h"

// �U�����
// ANIM_ATK1: �o�X�^�[
// ANIM_ATK2: �L���m��
// ANIM_ATK3: �\�[�h
// ANIM_ATK4: �{��

enum MindState{// �摜�̏��ɍ��킹��
	MIND_�V���N��,
	MIND_�{��,
	MIND_�ʏ�,
	MIND_�s��,
	MIND_��,
	MIND_���[��,
	MIND_�A�N�A,
	MIND_�E�b�h,
	MIND_�W�����N,
	MIND_�u���[�X,
	MIND_���^��,
	MIND_�K�b�c,
	MIND_�T�[�`,
	MIND_�i���o�[,
	MIND_�t�@�C�A,
	MIND_�E�B���h,
	MIND_�T���_�[,

	MIND_MAX
};

class BattlePlayer:public BattleCharBase{
	enum ShotState{
		ST_NULL,// �V���b�g��ł��Ă��Ȃ�
		ST_NORMAL,
		ST_CHARGE,
	};

	class ActMove:public AnimProcess{
		BattlePlayer *obj;
	public:
		ActMove(BattlePlayer *obj):obj(obj){}
		void Begin();
	};

	class ActShot:public AnimProcess{
		BattlePlayer *obj;
	public:
		ActShot(BattlePlayer *obj):obj(obj){}
		void Begin();
	};

	class ActDamage:public AnimProcess{
		BattlePlayer *obj;
	public:
		ActDamage(BattlePlayer *obj):obj(obj){}
		void Begin();
	};

	int imgCharge[16];
	int chargeMaxTime;// �`���[�W����������܂ł̎���
	int shotPower;
	int chargeCount;
	int moveDirect;
	int mindState;
	ShotState shotState;
	std::vector<ChipInfo> folder;
	std::vector<ChipInfo> sendChips;

	virtual void DamageMutekiFunc();
public:
	BattlePlayer(int shotPower, int mindState, int hp, int hpMax, std::vector<ChipInfo> folder);
	~BattlePlayer();

	void Draw();
	void DrawSendChipIcon();
	void LoadAnim();
	void DeleteAnim();
	void Process();

	int GetMindState()const{ return mindState; }
	void SetMindState(int mindState){ this->mindState = mindState; }

	std::vector<ChipInfo> GetHandData(unsigned max);
	void SendChips(std::vector<unsigned> selectedIndexed);
	void ShuffleFolder();
};

class PlayerMgr{
public:
	static const int FOLDER_NUM = 30;// �t�H���_�ɓ����`�b�v�̖���

	enum BattleResult{
		eBT_RTN_WIN,
		eBT_RTN_LOSE,
		// debug(���������H)

		eBT_RTN_MAX
	};
private:
	int money;
	std::vector<ChipInfo> folder;
	int hp, hpMax;
	int shotPower;
	int battleResult[def::eKIND_MAX-1][eBT_RTN_MAX];
	int mindState;

	PlayerMgr(){}
	~PlayerMgr(){}

	void ResetPlayerInfo();
public:
	static PlayerMgr *GetInst(){
		static PlayerMgr inst;
		return &inst;
	}

	// Getter and Setter
	int GetMoney()const{ return money; }
	void SetMoney(int money){ this->money = money; }
	int GetHp()const{ return hp; }
	void SetHp(int hp){ this->hp = hp; }
	int GetHpMax()const{ return hpMax; }
	void SetHpMax(int hpMax){ this->hpMax = hpMax; }
	int GetBattleResult(BattleResult ret, int battleEnemyKind);
	void IncrementBattleResult(BattleResult ret, int battleEnemyKind);
	int GetMindState()const{ return mindState; }
	void SetMindState(int mindState){ this->mindState = mindState; }

	// ���̑��̊֐�
	void Save();
	void CreateNewPlayer();
	void ContinueWithSaveFile();
	bool IsContinueOK();

	std::shared_ptr<BattlePlayer> CreateBattlePlayer();
};
