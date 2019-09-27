#include "include.h"
#include "battleFieldMgr.h"

void BattleFieldMgr::BattleInit(){
	fieldData.LoadImages();
	fieldData.InitInBattleBegin();
	backData.LoadImages();
}

void BattleFieldMgr::BattleEnd(){
	fieldData.DeleteImages();
	backData.DeleteImages();
}

void BattleFieldMgr::Draw(){
	backData.Draw(BattleBack::eTYPE_秋原町);// debug(マップ情報からとってくるように)
	fieldData.Draw();
}

void BattleFieldMgr::Process(){
	backData.Process();
}

void BattleFieldMgr::GetPanelInfo(int result[BattleField::LAYER_MAX], int x, int y){
	fieldData.GetPanelInfo(result, x, y);
}