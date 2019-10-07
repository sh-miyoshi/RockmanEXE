#include <random>
#include <algorithm>
#include "include.h"
#include "state.h"
#include "battle.h"
#include "playerMgr.h"
#include "fps.h"
#include "chip.h"
#include "effectMgr.h"
#include "drawCharacter.h"

bool gExitFlag = false;
unsigned long long gGameCount = 0;
std::mt19937_64 rnd_generator;

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
	// �V�X�e���֌W�̏�����
	AppLogger::CleanupLogFile();
	std::random_device rnd;
	rnd_generator.seed(rnd());

	// TODO(�o�[�W�������̐ݒ�)

	std::string fname = def::TEXT_FILE_PATH + "font.ttf";
	if( AddFontResourceEx(fname.c_str(), FR_PRIVATE, NULL) <= 0 ) {
		// �t�H���g�ǂݍ��ݎ��s
		AppLogger::Error("failed to read font file: %s", fname);
	}

	// DxLib�֌W�̏�����
	ChangeWindowMode(TRUE);//�E�B���h�E���[�h
	SetGraphMode(def::FMX, def::FMY, 16);//�E�B���h�E�T�C�Y�ύX
	SetOutApplicationLogValidFlag(TRUE);//log��\������
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetDXArchiveKeyString(def::RESOURCE_PASSWORD.c_str());

	// �A�v���P�[�V�����֌W�̏�����
	ChipMgr::GetInst()->LoadData();
	EffectMgr::GetInst()->LoadImages();
	DrawCharacter::GetInst()->LoadImages();
	PlayerMgr::GetInst()->InitPlayer();// TODO(�^�C�g����ʂőI������)

	Main main;
	main.Process();

	// �A�v���P�[�V�����֌W�̏I������
	DrawCharacter::GetInst()->DeleteImages();
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
