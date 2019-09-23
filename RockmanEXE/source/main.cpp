#include <time.h>
#include "include.h"
#include "state.h"
#include "main.h"
#include "battle.h"

bool gExitFlag = false;
unsigned long long gGameCount = 0;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow){
	// システム関係の初期化
	srand(( unsigned int ) time(NULL));
	AppLogger::CleanupLogFile();

	std::string fname = def::TEXT_FILE_PATH + "font.ttf";
	if( AddFontResourceEx(fname.c_str(), FR_PRIVATE, NULL) <= 0 ){
		// フォント読み込み失敗
		AppLogger::Error("failed to read font file: " + fname);
	}

	// DxLib関係の初期化
	ChangeWindowMode(TRUE);//ウィンドウモード
	SetGraphMode(def::FMX, def::FMY, 16);//ウィンドウサイズ変更
	SetOutApplicationLogValidFlag(TRUE);//logを表示する
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetDXArchiveKeyString(def::RESOURCE_PASSWORD.c_str());

	// アプリケーション関係の初期化

	Main main;
	main.Process();

	// アプリケーション関係の終了処理

	DxLib_End();
	return 0;
}

Main::Main(void){
	stateMgr.ChangeNext(new Battle(this));
}

Main::~Main(void){
}

void Main::Process(){
	while( ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 ){
		// メイン処理
		stateMgr.Process();

		stateMgr.Draw();

		CKey::GetInst()->Update();
		fps.Process();// fps制御
		fps.Draw(def::FMX - 60, 10);

		if( gExitFlag )
			break;

		// debug(Escapeキーでは終了できないようにする)
		if( CKey::GetInst()->CheckKey(eKEY_START) == 1 )
			break;

		gGameCount++;
	}
}
