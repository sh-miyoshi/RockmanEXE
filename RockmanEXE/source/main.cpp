#include <time.h>
#include "include.h"
#include "state.h"
#include "battle.h"
#include "playerMgr.h"
#include "fps.h"
#include "chip.h"
#include "effectMgr.h"

bool gExitFlag = false;
unsigned long long gGameCount = 0;

class Main {
	class StateBattle :public StateBase{
		Main* obj;
		Battle battle;
	public:
		StateBattle(Main* obj);
		~StateBattle();

		void Draw();
		void Process();
	};

	Fps fps;
	StateMgr stateMgr;
public:
	Main();
	~Main();

	void Process();
};


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
	// システム関係の初期化
	srand(( unsigned int ) time(NULL));
	AppLogger::CleanupLogFile();

	// TODO(バージョン情報の設定)

	std::string fname = def::TEXT_FILE_PATH + "font.ttf";
	if( AddFontResourceEx(fname.c_str(), FR_PRIVATE, NULL) <= 0 ) {
		// フォント読み込み失敗
		AppLogger::Error("failed to read font file: %s", fname);
	}

	// DxLib関係の初期化
	ChangeWindowMode(TRUE);//ウィンドウモード
	SetGraphMode(def::FMX, def::FMY, 16);//ウィンドウサイズ変更
	SetOutApplicationLogValidFlag(TRUE);//logを表示する
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetDXArchiveKeyString(def::RESOURCE_PASSWORD.c_str());

	// アプリケーション関係の初期化
	ChipMgr::GetInst()->LoadData();
	EffectMgr::GetInst()->LoadImages();
	PlayerMgr::GetInst()->InitPlayer();// TODO(タイトル画面で選択する)

	Main main;
	main.Process();

	// アプリケーション関係の終了処理
	EffectMgr::GetInst()->DeleteImages();
	ChipMgr::GetInst()->DeleteData();

	DxLib_End();
	return 0;
}

Main::Main(void) {
	AppLogger::Info("Change Main State to StateBattle");
	stateMgr.ChangeNext(new StateBattle(this));
}

Main::~Main(void) {
}

void Main::Process() {
	while( ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 ) {
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

Main::StateBattle::StateBattle(Main* obj):obj(obj) {
}

Main::StateBattle::~StateBattle() {
}

void Main::StateBattle::Draw() {
	battle.Draw();
}

void Main::StateBattle::Process() {
	switch( battle.Process() ) {
	case Battle::eRTN_WIN:
		break;
	case Battle::eRTN_LOSE:
		break;
	}
}
