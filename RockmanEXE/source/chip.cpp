#include <vector>
#include <algorithm>
#include <fstream>
#include "include.h"
#include "chip.h"

ChipData::ChipData()
	:id(0), type(0), power(0), playerAct(0), isPrevMotion(false),
	imgIcon(-1), imgIconMono(-1), imgInfo(-1), code('*') {
}

ChipData::ChipData(unsigned int id, std::string name, unsigned int type, unsigned int power, unsigned int playerAct, bool isPrevMotion)
	: id(id), name(name), type(type), power(power), playerAct(playerAct), isPrevMotion(isPrevMotion),
	imgIcon(-1), imgIconMono(-1), imgInfo(-1), code('*') {
}

//　画像とチップ情報の読み込み
void ChipMgr::LoadData() {
	const std::string position = "ChipMgr::LoadData";

	if( isLoad ) {// 二度読み込みの禁止
		AppLogger::Error("Chip Data is already loaded");
		exit(1);
	}

	// チップ情報の設定
	chipData[eID_キャノン] = ChipData(0, "キャノン", 0, 40, 2, false);
	chipData[eID_ハイキャノン] = ChipData(1, "ハイキャノン", 0, 80, 2, false);
	chipData[eID_サンダーボール] = ChipData(14, "サンダーボール", 0, 40, -1, false);
	chipData[eID_フレイムライン] = ChipData(20, "フレイムライン", 0, 80, 2, false);
	chipData[eID_ミニボム] = ChipData(43, "ミニボム", 0, 50, 4, false);
	chipData[eID_ソード] = ChipData(53, "ソード", 0, 80, 3, false);
	chipData[eID_ワイドソード] = ChipData(54, "ワイドソード", 0, 80, 3, false);
	chipData[eID_ブーメラン] = ChipData(68, "ブーメラン", 0, 60, -1, false);
	chipData[eID_リカバリー10] = ChipData(108, "リカバリー10", 0, 10, -1, false);
	chipData[eID_リカバリー30] = ChipData(109, "リカバリー30", 0, 30, -1, false);
	chipData[eID_ストーンキューブ] = ChipData(125, "ストーンキューブ", 0, 0, -1, true);

	// チップアイコン画像の読み込みと設定
	std::string fname = def::IMAGE_FILE_PATH + "chip_icon.png";
	int t1[240], t2[240];
	LoadDivGraphWithErrorCheck(t1, fname, position, 30, 8, 28, 28);
	LoadDivGraph(fname.c_str(), 240, 30, 8, 28, 28, t2);// 同じファイルからなので読み込めるのは確定

	for( auto i = 0; i < eID_MAX; i++ ) {
		unsigned int id = chipData[i].id;
		chipData[i].imgIcon = t1[id];
		chipData[i].imgIconMono = t2[id];
		GraphFilter(chipData[i].imgIconMono, DX_GRAPH_FILTER_MONO, 0, 0);// モノクロ化
	}

	std::vector<int> used;// 使用しているアイコンの番号
	for( auto i = 0; i < eID_MAX; i++ ) {
		used.push_back(chipData[i].id);
	}

	// 使わないものは開放しておく
	for( int i = 0; i < 240; i++ ) {
		if( std::find(used.begin(), used.end(), i) == used.end() ) {
			DeleteGraph(t1[i]);
			DeleteGraph(t2[i]);
		}
	}

	// チップ詳細画像
	std::string fdir = def::IMAGE_FILE_PATH + "infoImage/";
	fname = fdir + "s1.png";
	chipData[eID_キャノン].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s2.png";
	chipData[eID_ハイキャノン].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s95.png";
	chipData[eID_ストーンキューブ].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s15.png";
	chipData[eID_サンダーボール].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s54.png";
	chipData[eID_ソード].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s21.png";
	chipData[eID_フレイムライン].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s109.png";
	chipData[eID_リカバリー10].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s69.png";
	chipData[eID_ブーメラン].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s44.png";
	chipData[eID_ミニボム].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s110.png";
	chipData[eID_リカバリー30].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s55.png";
	chipData[eID_ワイドソード].imgInfo = LoadGraphWithErrorCheck(fname, position);

	// チップタイプ画像
	fname = def::IMAGE_FILE_PATH + "chip_type.png";
	LoadDivGraphWithErrorCheck(imgType, fname, position, 7, 2, 28, 28);

	// 属性情報
	static const std::string TYPE_NAME[eTYPE_MAX] = {
		"無","風","ブレイク","ソード","地形","置物","回復","カーソル","数値付加","炎","水","電気","木"
	};
	for( int i = 0; i < eTYPE_MAX; i++ ) {
		typeData[i].name = TYPE_NAME[i];
		typeData[i].image = imgType[i];
	}

	isLoad = true;
}

void ChipMgr::DeleteData() {
	if( isLoad ) {
		isLoad = false;

		for( int i = 0; i < eID_MAX; i++ ) {
			DeleteGraph(chipData[i].imgIcon);
			DeleteGraph(chipData[i].imgIconMono);
			DeleteGraph(chipData[i].imgInfo);
		}

		for( int i = 0; i < eTYPE_MAX; i++ ) {
			DeleteGraph(imgType[i]);
		}
	}
}

TypeData ChipMgr::GetTypeData(int typeNo) {
	if( !isLoad ) {
		AppLogger::Error("ChipData are not loaded yet");
		exit(1);
	}
	if( !( 0 <= typeNo && typeNo < eTYPE_MAX ) ) {
		AppLogger::Error("Invalid Type Number(%d). typeNo must be between 0 and %d", typeNo, eTYPE_MAX);
		exit(1);
	}
	return typeData[typeNo];
}

ChipData ChipMgr::GetChipData(int id, char code) {
	if( !isLoad ) {
		AppLogger::Error("ChipData are not loaded yet");
		exit(1);
	}
	if( !( 0 <= id && id < eID_MAX ) ) {
		AppLogger::Error("Invalid Chip ID(%d). ID must be between 0 and %d", id, eID_MAX);
		exit(1);
	}
	ChipData ret = chipData[id];
	ret.code = code;
	return ret;
}

