#pragma once

#include <string>
#include <vector>
#include "battleCharBase.h"
#include "chip.h"
#include "animation.h"
#include "enemy.h"

class BattlePlayer:public BattleCharBase {
public:
	enum MindStatus {// ���ԌŒ�(�摜�̏���)
		eSTATUS_�t���V���N��,
		eSTATUS_�{��,
		eSTATUS_�ʏ�,
		eSTATUS_�s��,
		eSTATUS_�_�[�N,
		eSTATUS_���[���\�E��,
		eSTATUS_�A�N�A�\�E��,
		eSTATUS_�E�b�h�\�E��,
		eSTATUS_�W�����N�\�E��,
		eSTATUS_�u���[�X�\�E��,
		eSTATUS_���^���\�E��,
		eSTATUS_�K�b�c�\�E��,
		eSTATUS_�T�[�`�\�E��,
		eSTATUS_�i���o�[�\�E��,
		eSTATUS_�t�@�C�A�\�E��,
		eSTATUS_�E�B���h�\�E��,
		eSTATUS_�T���_�[�\�E��,

		eSTATUS_MAX
	};
private:
	static const int MUTEKI_TIME = 2 * 60;

	unsigned int chargeCount;
	unsigned int chargeMaxTime;
	unsigned int busterPower;
	int mutekiCount;
	MindStatus mindStatus;
	int imgCharge[16];
	std::vector<ChipInfo> chipFolder;
	std::list<ChipInfo> sendChipList;

	std::shared_ptr<Animation> anim[eANIM_MAX];
	std::shared_ptr<Animation> damageAnim;
public:
	BattlePlayer(
		std::string name,
		unsigned int hp,
		unsigned int hpMax,
		unsigned int busterPower,
		MindStatus mindStatus,
		std::vector<ChipInfo> chipFolder
	);
	~BattlePlayer();

	virtual void Draw();
	virtual void Process();

	MindStatus GetMindStatus()const { return mindStatus; }
	std::vector<ChipInfo> GetHandData(unsigned max);
	void SetSendChipList(std::vector<int> selectedIndexes);

	// TODO(�ȉ���Method��Boss���g�p����̂�BattleCharBase�ɂ܂Ƃ߂�)
	void SetMuteki() { mutekiCount = 0; }
	bool IsMuteki()const { return mutekiCount >= 0; }
	void AttachDamageAnim();
};

class PlayerMgr {
	static const int FOLDER_NUM = 30;
	enum BattleResult {
		eBT_RTN_WIN,
		eBT_RTN_LOSE,
		eBT_RTN_MAX
	};

	std::string name;
	unsigned int hp, hpMax;
	unsigned int busterPower;
	BattlePlayer::MindStatus mindStatus;
	std::vector<ChipInfo> chipFolder;
	unsigned int battleResult[EnemyMgr::ID_MAX][eBT_RTN_MAX];

	BattlePlayer* battlePlayer;

	PlayerMgr();
	~PlayerMgr();

	void Save();
public:
	static PlayerMgr* GetInst() {
		static PlayerMgr inst;
		return &inst;
	}

	BattlePlayer* GetBattleChar() { return battlePlayer; }
	void InitBattleChar();

	void CreateNewPlayer();
	void ContinueWithSaveFile();
	void UpdateBattleResult(bool isWin, std::vector<EnemyMgr::EnemyID> enemies);
	bool IsContinueOK();
};
