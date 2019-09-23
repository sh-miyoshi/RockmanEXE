#include <time.h>
#include "include.h"
#include "state.h"
#include "main.h"
#include "battle.h"

bool gExitFlag = false;
unsigned long long gGameCount = 0;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow){
	// �V�X�e���֌W�̏�����
	srand(( unsigned int ) time(NULL));
	AppLogger::CleanupLogFile();

	std::string fname = def::TEXT_FILE_PATH + "font.ttf";
	if( AddFontResourceEx(fname.c_str(), FR_PRIVATE, NULL) <= 0 ){
		// �t�H���g�ǂݍ��ݎ��s
		AppLogger::Error("failed to read font file: " + fname);
	}

	// DxLib�֌W�̏�����
	ChangeWindowMode(TRUE);//�E�B���h�E���[�h
	SetGraphMode(def::FMX, def::FMY, 16);//�E�B���h�E�T�C�Y�ύX
	SetOutApplicationLogValidFlag(TRUE);//log��\������
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetDXArchiveKeyString(def::RESOURCE_PASSWORD.c_str());

	// �A�v���P�[�V�����֌W�̏�����

	Main main;
	main.Process();

	// �A�v���P�[�V�����֌W�̏I������

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
		// ���C������
		stateMgr.Process();

		stateMgr.Draw();

		CKey::GetInst()->Update();
		fps.Process();// fps����
		fps.Draw(def::FMX - 60, 10);

		if( gExitFlag )
			break;

		// debug(Escape�L�[�ł͏I���ł��Ȃ��悤�ɂ���)
		if( CKey::GetInst()->CheckKey(eKEY_START) == 1 )
			break;

		gGameCount++;
	}
}
