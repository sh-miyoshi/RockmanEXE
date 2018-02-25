#pragma once

#include <vector>
#include <map>
#include "battleCharBase.h"
#include "chip.h"

// 攻撃種類
// ANIM_ATK1: バスター
// ANIM_ATK2: キャノン
// ANIM_ATK3: ソード
// ANIM_ATK4: ボム

enum MindState{// 画像の順に合わせる
	MIND_シンクロ,
	MIND_怒り,
	MIND_通常,
	MIND_不安,
	MIND_闇,
	MIND_ロール,
	MIND_アクア,
	MIND_ウッド,
	MIND_ジャンク,
	MIND_ブルース,
	MIND_メタル,
	MIND_ガッツ,
	MIND_サーチ,
	MIND_ナンバー,
	MIND_ファイア,
	MIND_ウィンド,
	MIND_サンダー,

	MIND_MAX
};

class BattlePlayer:public BattleCharBase{
	enum ShotState{
		ST_NULL,// ショットを打っていない
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
	int chargeMaxTime;// チャージが完了するまでの時間
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
	static const int FOLDER_NUM = 30;// フォルダに入れるチップの枚数

	enum BattleResult{
		eBT_RTN_WIN,
		eBT_RTN_LOSE,
		// debug(引き分け？)

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

	// その他の関数
	void Save();
	void CreateNewPlayer();
	void ContinueWithSaveFile();
	bool IsContinueOK();

	std::shared_ptr<BattlePlayer> CreateBattlePlayer();
};
