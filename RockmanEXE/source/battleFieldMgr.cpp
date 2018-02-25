#include "include.h"
#include "battleFieldMgr.h"
#include "battleCharMgr.h"

void BattleFieldMgr::Init(){
	fieldData.LoadImages();
	fieldData.Init();
	backMgr.LoadImages();
	gaugeCount = 0;
	gaugeUpSpeed = GUSPD_NORMAL;
	drawGaugeMaxCount = 0;

	std::string position = "BattleFieldMgr::Init";
	std::string fname = def::IMAGE_FILE_PATH + "gauge.png";
	imgGauge[0] = LoadGraphWithCheckError(fname, position);

	fname = def::IMAGE_FILE_PATH + "gauge_max.png";
	LoadDivGraphWithCheckError(imgGauge + 1, fname, position, 1, 4, 288, 30);

	fname = def::IMAGE_FILE_PATH + "mind_window_frame.png";
	imgMindWindowFrame = LoadGraphWithCheckError(fname, position);
}

void BattleFieldMgr::End(){
	fieldData.DeleteImages();
	backMgr.DeleteImages();
	for( int i = 0; i < 5; i++ )
		DeleteGraph(imgGauge[i]);
	DeleteGraph(imgMindWindowFrame);
}

void BattleFieldMgr::Draw(){
	backMgr.Draw(BattleBack::eTYPE_秋原町);// debug(マップ情報からとってくるように)
	fieldData.Draw();
}

void BattleFieldMgr::DrawBaseFrame(bool isShift){
	if( !isShift ){
		// ゲージの描画
		if( gaugeCount < GAUGE_MAX_COUNT ){
			DrawGraph(96, 5, imgGauge[0], TRUE);
			static const int GAUGE_MAX_SIZE = 256;
			int size = GAUGE_MAX_SIZE*gaugeCount / GAUGE_MAX_COUNT;
			DrawBox(112, 19, 112 + size, 21, GetColor(123, 154, 222), TRUE);
			DrawBox(112, 21, 112 + size, 29, GetColor(231, 235, 255), TRUE);
			DrawBox(112, 29, 112 + size, 31, GetColor(123, 154, 222), TRUE);
		} else{
			int c = ( ( drawGaugeMaxCount++ / 20 ) % 4 ) + 1;
			DrawGraph(96, 5, imgGauge[c], TRUE);
		}
	}

	int xsh = isShift ? 235 : 0;

	// ココロウィンドウ
	CPoint<int> t(7 + xsh, 40);
	DrawGraph(t.x, t.y, imgMindWindowFrame, TRUE);// フレームの描画
	BattleCharMgr::GetInst()->DrawPlayerState(t);
}

void BattleFieldMgr::Process(){
	backMgr.Process();
}

void BattleFieldMgr::GaugeProcess(){
	if( gaugeCount < GAUGE_MAX_COUNT ){
		gaugeCount += gaugeUpSpeed;
	}
}

void BattleFieldMgr::GetPanelInfo(int result[BattleField::LAYER_MAX], int x, int y){
	fieldData.GetPanelInfo(result, x, y);
}

void BattleFieldMgr::SetGaugeSpeed(GaugeUpSpeed spd){
	gaugeUpSpeed = spd;
}

void BattleFieldMgr::AddDrawHitArea(CPoint<int> pos){
	fieldData.AddDrawHitArea(pos);
}