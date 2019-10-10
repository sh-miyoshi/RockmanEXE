#pragma once

#include <string>
#include <vector>
#include "battleCharBase.h"
#include "chip.h"
#include "animation.h"

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
	unsigned int chargeCount;
	unsigned int chargeMaxTime;
	unsigned int busterPower;
	MindStatus mindStatus;
	int imgCharge[16];
	std::vector<ChipInfo> chipFolder;
	std::list<ChipInfo> sendChipList;

	std::shared_ptr<Animation> anim[eANIM_MAX];
public:
	BattlePlayer(std::string name, unsigned int hp, unsigned int hpMax, unsigned int busterPower, std::vector<ChipInfo> chipFolder);
	~BattlePlayer();

	virtual void Draw();
	virtual void Process();

	std::vector<ChipInfo> GetHandData(unsigned max);
	void SetSendChipList(std::vector<int> selectedIndexes);

	MindStatus GetMindStatus()const { return mindStatus; }
};

class PlayerMgr {
	static const int FOLDER_NUM = 30;

	std::string name;
	unsigned int hp, hpMax;
	unsigned int busterPower;
	std::vector<ChipInfo> chipFolder;

	BattlePlayer* battlePlayer;

	PlayerMgr();
	~PlayerMgr();
public:
	static PlayerMgr* GetInst() {
		static PlayerMgr inst;
		return &inst;
	}

	BattlePlayer* GetBattleChar() { return battlePlayer; }
	void InitBattleChar();

	void InitPlayer();
	// TODO void LoadSaveFile();
	// TODO Save()
};
