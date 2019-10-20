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
#include "title.h"

bool gExitFlag = false;
unsigned long long gGameCount = 0;
std::mt19937_64 rnd_generator;

class Main {
	class StateTitle:public StateBase {
		Main* obj;
		Title title;
	public:
		StateTitle(Main* obj);
		~StateTitle();

		void Draw();
		void Process();
	};

	class StateMenu:public StateBase {
		Main* obj;
	public:
		StateMenu(Main* obj);
		~StateMenu();

		void Draw();
		void Process();
	};

	class StateBattle:public StateBase {
		Main* obj;
		Battle battle;
		std::vector<EnemyMgr::EnemyID> enemies;
	public:
		StateBattle(std::vector<Battle::EnemyInfo> enemies, Main* obj);
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
	//AppLogger::Info("Change Main State to StateTitle");
	//stateMgr.ChangeNext(new StateTitle(this));

	// debug�pstateChange
	PlayerMgr::GetInst()->CreateNewPlayer();// TODO(�^�C�g����ʂőI������)
	AppLogger::Info("Change Main State to StateBattle");
	std::vector<Battle::EnemyInfo> enemies;
	Battle::EnemyInfo e1;
	e1.id = EnemyMgr::ID_�I;
	e1.pos = CPoint<int>(4, 1);
	enemies.push_back(e1);
	stateMgr.ChangeNext(new StateBattle(enemies, this));
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

Main::StateTitle::StateTitle(Main* obj):obj(obj) {
}

Main::StateTitle::~StateTitle() {
}

void Main::StateTitle::Draw() {
	title.Draw();
}

void Main::StateTitle::Process() {
	if( title.Process() ) {
		obj->stateMgr.ChangeNext(new StateMenu(obj));
	}
}

Main::StateMenu::StateMenu(Main* obj):obj(obj) {
}

Main::StateMenu::~StateMenu() {
}

void Main::StateMenu::Draw() {
	DrawString(100, 100, "Menu", RED);
}

void Main::StateMenu::Process() {
}


Main::StateBattle::StateBattle(std::vector<Battle::EnemyInfo> enemies, Main* obj)
	:obj(obj), battle(enemies) {

	for( auto e : enemies ) {
		this->enemies.push_back(e.id);
	}
}

Main::StateBattle::~StateBattle() {
}

void Main::StateBattle::Draw() {
	battle.Draw();
}

void Main::StateBattle::Process() {
	switch( battle.Process() ) {
	case Battle::eRTN_WIN:
		PlayerMgr::GetInst()->UpdateBattleResult(true, enemies);
		obj->stateMgr.ChangeNext(new StateMenu(obj));
		break;
	case Battle::eRTN_LOSE:
		PlayerMgr::GetInst()->UpdateBattleResult(false, enemies);
		obj->stateMgr.ChangeNext(new StateMenu(obj));
		break;
	}
}
