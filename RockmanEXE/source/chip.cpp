#include <vector>
#include <algorithm>
#include "include.h"
#include "chip.h"
#include "csv.h"

//　画像とチップ情報の読み込み
void ChipMgr::LoadData(){
	const std::string position = "ChipMgr::LoadData";

	ASSERT(!isLoad, position + " already loaded data");// 二度読み込みの禁止
	isLoad = true;

	// チップアイコン画像
	std::string fname = def::IMAGE_FILE_PATH + "chip_icon.png";
	int t1[240], t2[240];
	LoadDivGraphWithCheckError(t1, fname, position, 30, 8, 28, 28);
	LoadDivGraph(fname.c_str(), 240, 30, 8, 28, 28, t2);// 同じファイルからなので読み込めるのは確定

	// アイコンは基本的に手動で設定
	imgIcon[eID_キャノン] = t1[0];
	imgIconMono[eID_キャノン] = t2[0];
	imgIcon[eID_ハイキャノン] = t1[1];
	imgIconMono[eID_ハイキャノン] = t2[1];
	imgIcon[eID_ストーンキューブ] = t1[125];
	imgIconMono[eID_ストーンキューブ] = t2[125];
	imgIcon[eID_サンダーボール] = t1[14];
	imgIconMono[eID_サンダーボール] = t2[14];
	imgIcon[eID_ソード] = t1[53];
	imgIconMono[eID_ソード] = t2[53];
	imgIcon[eID_フレイムライン] = t1[20];
	imgIconMono[eID_フレイムライン] = t2[20];
	imgIcon[eID_リカバリー10] = t1[108];
	imgIconMono[eID_リカバリー10] = t2[108];
	imgIcon[eID_ブーメラン] = t1[68];
	imgIconMono[eID_ブーメラン] = t2[68];
	imgIcon[eID_ミニボム] = t1[43];
	imgIconMono[eID_ミニボム] = t2[43];
	imgIcon[eID_リカバリー30] = t1[109];
	imgIconMono[eID_リカバリー30] = t2[109];
	imgIcon[eID_ワイドソード] = t1[54];
	imgIconMono[eID_ワイドソード] = t2[54];

	std::vector<int> used;// 使用しているアイコンの番号
	used.push_back(0);
	used.push_back(1);
	used.push_back(125);
	used.push_back(14);
	used.push_back(53);
	used.push_back(20);
	used.push_back(108);
	used.push_back(68);
	used.push_back(43);
	used.push_back(109);
	used.push_back(54);

	// モノクロ化
	for( int i = 0; i < eID_MAX; i++ )
		GraphFilter(imgIconMono[i], DX_GRAPH_FILTER_MONO, 0, 0);

	// 使わないものは開放しておく
	for( int i = 0; i < 240; i++ ){
		if( std::find(used.begin(), used.end(), i) == used.end() ){
			DeleteGraph(t1[i]);
			DeleteGraph(t2[i]);
		}
	}

	// チップ詳細画像
	std::string fdir = def::IMAGE_FILE_PATH + "infoImage/";
	fname = fdir + "s1.png";
	imgInfo[eID_キャノン] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s2.png";
	imgInfo[eID_ハイキャノン] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s95.png";
	imgInfo[eID_ストーンキューブ] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s15.png";
	imgInfo[eID_サンダーボール] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s54.png";
	imgInfo[eID_ソード] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s21.png";
	imgInfo[eID_フレイムライン] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s109.png";
	imgInfo[eID_リカバリー10] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s69.png";
	imgInfo[eID_ブーメラン] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s44.png";
	imgInfo[eID_ミニボム] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s110.png";
	imgInfo[eID_リカバリー30] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s55.png";
	imgInfo[eID_ワイドソード] = LoadGraphWithCheckError(fname, position);

	// チップタイプ画像
	fname = def::IMAGE_FILE_PATH + "chip_type.png";
	LoadDivGraphWithCheckError(imgType, fname, position, 7, 2, 28, 28);

	// チップ情報
	fname = def::TEXT_FILE_PATH + "chip_data.csv";
	std::string buf[CSV::COLUMN_MAX];
	CSV input(fname, CSV::MODE_READ);
	for( int i = 0; i < eID_MAX&&input.getData(buf); i++ ){
		chipData[i].id = i;
		chipData[i].imgInfo = imgInfo[i];
		chipData[i].imgIcon = imgIcon[i];
		chipData[i].imgIconMono = imgIconMono[i];
		chipData[i].name = buf[0];
		chipData[i].code = '*';// 適当
		chipData[i].type = atoi(buf[1].c_str());
		chipData[i].power = atoi(buf[2].c_str());
		chipData[i].playerAct = atoi(buf[3].c_str());
		chipData[i].isPrevMotion = atoi(buf[4].c_str()) != 0;
	}

	// 属性情報
	static const std::string TYPE_NAME[eTYPE_MAX] = {
		"無","風","ブレイク","ソード","地形","置物","回復","カーソル","数値付加","炎","水","電気","木"
	};
	for( int i = 0; i < eTYPE_MAX; i++ ){
		typeData[i].name = TYPE_NAME[i];
		typeData[i].image = imgType[i];
	}
}

void ChipMgr::DeleteData(){
	isLoad = false;

	for( int i = 0; i < eID_MAX; i++ ){
		DeleteGraph(imgIcon[i]);
		DeleteGraph(imgIconMono[i]);
		DeleteGraph(imgInfo[i]);
	}

	for( int i = 0; i < 14; i++ )
		DeleteGraph(imgType[i]);
}

TypeData ChipMgr::GetTypeData(int type){
	ASSERT(isLoad, "ChipMgr::GetTypeData data are not loaded");
	ASSERT(0 <= type&&type < eTYPE_MAX, ToString("ChipMgr::GetTypeData invalied type (%d)", type));
	return typeData[type];
}

ChipData ChipMgr::GetChipData(int id, char code){
	ASSERT(isLoad, "ChipMgr::GetChipData data are not loaded");
	ASSERT(0 <= id&&id < eID_MAX, ToString("ChipMgr::GetTypeData invalied chip id (%d)", id));
	ChipData ret = chipData[id];
	ret.code = code;
	return ret;
}

ChipData ChipMgr::GetChipData(ChipInfo info){
	return GetChipData(info.id, info.code);
}
