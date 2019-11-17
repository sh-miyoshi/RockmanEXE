#pragma once

#include <string>
#include "animation.h"

class ChipInfo {
public:
	int id;
	char code;

	ChipInfo():id(-1), code('*') {}
	ChipInfo(int id, char code):id(id), code(code) {}

	bool operator<(const ChipInfo& right) const {
		return id < right.id;
	}
};

// 処理はskillに任せる
class ChipData {
public:
	unsigned int id;
	int imgIcon;
	int imgIconMono;
	int imgInfo;
	std::string name;
	char code;
	unsigned int type;// 属性
	unsigned int power;
	PlayerAnimType playerAct;// プレイヤーの行動画像はどれか
	bool isPrevMotion;// チップ使用時に暗転などの前処理があるか

	ChipData();
	ChipData(unsigned int id, std::string name, unsigned int type, unsigned int power, PlayerAnimType playerAct, bool isPrevMotion);
	~ChipData() {}
};

typedef struct TypeData {
	std::string name;
	int image;
	// debug(弱点とか？)
}TypeData;

class ChipMgr {
public:
	enum Type {// 順番固定
		eTYPE_無,
		eTYPE_風,
		eTYPE_ブレイク,
		eTYPE_ソード,
		eTYPE_地形,
		eTYPE_置物,
		eTYPE_回復,
		eTYPE_カーソル,
		eTYPE_数値付加,
		eTYPE_炎,
		eTYPE_水,
		eTYPE_電気,
		eTYPE_木,

		eTYPE_MAX
	};

	enum ID {
		eID_キャノン = 0,
		eID_ハイキャノン = 1,
		eID_サンダーボール = 14,
		eID_フレイムライン = 20,
		eID_ミニボム = 43,
		eID_ソード = 53,
		eID_ワイドソード = 54,
		eID_ブーメラン = 68,
		eID_リカバリー10 = 108,
		eID_リカバリー30 = 109,
		eID_ストーンキューブ = 125,

		eID_MAX
	};
private:
	bool isLoad;
	int imgType[14];
	ChipData chipData[eID_MAX];
	TypeData typeData[eTYPE_MAX];

	ChipMgr():isLoad(false), imgType() {}
	~ChipMgr() {}
public:
	static ChipMgr* GetInst() {
		static ChipMgr inst;
		return &inst;
	}

	void LoadData();
	void DeleteData();

	TypeData GetTypeData(int typeNo);
	ChipData GetChipData(int id, char code = '*');
	ChipData GetChipData(ChipInfo c) { return GetChipData(c.id, c.code); }
};
