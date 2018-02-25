#pragma once

#include <string>

class ChipInfo{
public:
	int id;
	char code;

	ChipInfo(){}
	ChipInfo(int id, char code):id(id), code(code){}

	bool operator<(const ChipInfo& right) const{
		return id<right.id;
	}
};

// 処理はskillに任せる
class ChipData{
public:
	int id;
	int imgIcon;
	int imgIconMono;
	int imgInfo;
	std::string name;
	char code;
	int type;// 属性
	int power;
	int playerAct;// プレイヤーの行動画像はどれか
	bool isPrevMotion;// チップ使用時に暗転などの前処理があるか
};

class TypeData{
public:
	std::string name;
	int image;
	// debug(弱点とか？)
};

class ChipMgr{
public:
	enum Type{// 順番固定
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

	enum ID{
		eID_キャノン,
		eID_ハイキャノン,
		eID_ストーンキューブ,
		eID_サンダーボール,
		eID_ソード,
		eID_フレイムライン,
		eID_リカバリー10,
		eID_ブーメラン,
		eID_ミニボム,
		eID_リカバリー30,
		eID_ワイドソード,

		eID_MAX
	};
private:
	bool isLoad;
	int imgIcon[eID_MAX];
	int imgIconMono[eID_MAX];
	int imgInfo[eID_MAX];
	int imgType[14];
	ChipData chipData[eID_MAX];
	TypeData typeData[eTYPE_MAX];

	ChipMgr():isLoad(false){}
	~ChipMgr(){}
public:
	static ChipMgr *GetInst(){
		static ChipMgr inst;
		return &inst;
	}

	void LoadData();
	void DeleteData();

	TypeData GetTypeData(int type);
	ChipData GetChipData(int id, char code = '*');
	ChipData GetChipData(ChipInfo info);
};
