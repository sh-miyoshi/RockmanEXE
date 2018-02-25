#include <time.h>
#include "include.h"
#include "fps.h"
#include "state.h"
#include "battle.h"
#include "drawCharacter.h"
#include "chip.h"
#include "fade.h"
#include "selectMgr.h"
#include "player.h"
#include "enemy.h"
#include "effectMgr.h"

bool g_exitFlag = false;
unsigned long long g_count = 0;

class Main{
	class StateTitle:public StateBase{
		// �t�F�[�hin���Č���L�[���������܂�
		class StateTitleBegin:public StateBase{
			static const int ENTER_PERMIT_COUNT = 60;

			StateTitle *obj;
			int count;
			Fade fade;
		public:
			StateTitleBegin(StateTitle *obj);
			~StateTitleBegin();

			void Draw();
			void Process();
		};

		class StateTitleSelect:public StateBase{
			StateTitle *obj;
			SelectMgr selectMgr;
		public:
			StateTitleSelect(StateTitle *obj);
			~StateTitleSelect();

			void Draw();
			void Process();
		};

		Main *obj;
		int imgBack;
		StateMgr stateMgr;
	public:
		StateTitle(Main *obj);
		~StateTitle();

		void Draw();
		void Process();
	};

	class StateCharSelect:public StateBase{
		static const int SELECT_X_NUM = 3;

		Main *obj;
		int count;
		int select;
		std::vector<std::shared_ptr<BattleCharBase>> allEnemy;
	public:
		StateCharSelect(Main *obj);
		~StateCharSelect();

		void Draw();
		void Process();
	};

	class StateBattle:public StateBase{
		Main *obj;
		Battle battle;
		int battleEnemyKind;
	public:
		StateBattle(Main *obj, std::list<BattleEnemyInit_t> enemys, bool isBoss, int battleEnemyKind);
		~StateBattle();

		void Draw();
		void Process();
	};

	class StateLog:public StateBase{
		Main *obj;
	public:
		StateLog(Main *obj);
		~StateLog();

		void Draw();
		void Process();
	};

	class StateGameOver:public StateBase{
		static const int OFFSET_UP = 8;
		static const int BACK_BOX_SIZE_X = 16;
		static const int BACK_BOX_SIZE_Y = 16;
		static const int ABS_BOX_RANGE_SIZE = def::FMX * 2;

		Main *obj;
		int image;
		int count;
		std::vector<CPoint<int>> back_box_pos[19][3];

		void ReshapePosition();// ���W���͈͓��Ɏ��܂�悤�ɐ��`
		void BoxDraw();// �͈͓���box�����`�悷��
	public:
		StateGameOver(Main *obj);
		~StateGameOver();

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
	DrawCharacter::GetInst()->LoadImages();
	ChipMgr::GetInst()->LoadData();
	PlayerMgr::GetInst()->CreateNewPlayer();// �f�o�b�O�p�Ƀv���C���[��������(���Ƃŏ㏑������)
	EffectMgr::GetInst()->Init();

	Main main;
	main.Process();

	DrawCharacter::GetInst()->DeleteImages();
	ChipMgr::GetInst()->DeleteData();
	EffectMgr::GetInst()->End();

	DxLib_End();
	return 0;
}

Main::Main(void){
	stateMgr.ChangeNext(new StateTitle(this));
}

Main::~Main(void){
}

void Main::Process(){
	while( ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 ){
		// ���C������
		stateMgr.Draw();
		EffectMgr::GetInst()->Draw();

		stateMgr.Process();
		EffectMgr::GetInst()->Process();

		CKey::GetInst()->Update();
		fps.Process();// fps����
		fps.Draw(def::FMX - 60, 10);

		if( g_exitFlag )
			break;

		if( CKey::GetInst()->CheckKey(eKEY_START) == 1 )
			break;

		g_count++;
	}
}

//-------------------------------------------------------
// �^�C�g��
//-------------------------------------------------------
Main::StateTitle::StateTitle(Main * obj):obj(obj){
	std::string fname = def::IMAGE_FILE_PATH + "title_back.png";
	imgBack = LoadGraphWithCheckError(fname, "Main::StateTitle::StateTitle");

	stateMgr.ChangeNext(new StateTitleBegin(this));
}

Main::StateTitle::~StateTitle(){
	DeleteGraph(imgBack);
}

void Main::StateTitle::Draw(){
	DrawGraph(0, 0, imgBack, FALSE);
	stateMgr.Draw();
}

void Main::StateTitle::Process(){
	stateMgr.Process();
}

Main::StateTitle::StateTitleBegin::StateTitleBegin(StateTitle * obj)
	:obj(obj), count(0)
{
	fade.SetFadeIn(40);
}

Main::StateTitle::StateTitleBegin::~StateTitleBegin(){
}

void Main::StateTitle::StateTitleBegin::Draw(){
	fade.Draw();

	if( count > ENTER_PERMIT_COUNT ){
		// todo(PRESS ENTER�̕\���A�_��)
	}
}

void Main::StateTitle::StateTitleBegin::Process(){
	count++;
	fade.Process();
	if( count > ENTER_PERMIT_COUNT ){
		if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 ){
			obj->stateMgr.ChangeNext(new StateTitleSelect(obj));
		}
	}
}

Main::StateTitle::StateTitleSelect::StateTitleSelect(StateTitle * obj)
	:obj(obj)
{
	const std::string data[] = { "�͂��߂���","�Â�����" };
	selectMgr.Create(180, 230, data, 2);
	selectMgr.SetColor({ WHITE,( int ) GetColor(128,255,0) });
	if( !PlayerMgr::GetInst()->IsContinueOK() ){
		selectMgr.SetUnSelectable(1);// ����������ł��Ȃ�����
	} else{
		selectMgr.SetPointer(1);// �f�t�H���g�łÂ������I��
	}
}

Main::StateTitle::StateTitleSelect::~StateTitleSelect(){
}

void Main::StateTitle::StateTitleSelect::Draw(){
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);// �A���t�@�u�����h
	DrawBox(0, 0, def::FMX, def::FMY, BLACK, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);// �m�[�}���u�����h

	selectMgr.Draw();
}

void Main::StateTitle::StateTitleSelect::Process(){
	switch( selectMgr.Process() ){
	case 0:// �͂��߂���
		PlayerMgr::GetInst()->CreateNewPlayer();
		obj->obj->stateMgr.ChangeNext(new StateCharSelect(obj->obj));
		break;
	case 1:// �Â�����
		PlayerMgr::GetInst()->ContinueWithSaveFile();
		obj->obj->stateMgr.ChangeNext(new StateCharSelect(obj->obj));
		break;
	}
}


//-------------------------------------------------------
// �L�����I��
//-------------------------------------------------------
Main::StateCharSelect::StateCharSelect(Main * obj):obj(obj){
	for( int i = 0; i < EnemyMgr::ID_MAX; i++ ){
		allEnemy.push_back(EnemyMgr::GetData(i));
		allEnemy[i]->LoadAnim();
	}
}

Main::StateCharSelect::~StateCharSelect(){
	for( int i = 0; i < EnemyMgr::ID_MAX; i++ ){
		allEnemy[i]->DeleteAnim();
	}
}

void Main::StateCharSelect::Draw(){
	static const std::string SELECT_NAMES[def::eKIND_MAX] = {
		"���b�g�[�� x1",
		"���b�g�[�� x3",
		"�r���[ x1",
		"���b�g�[�� and �r���[",
		"�t�H���e",
		"���"
	};


	DrawStringCenter(def::FMX / 2, 20, SELECT_NAMES[select], WHITE);// �G���`��
	DrawBox(def::FMX / 2 - 75, 50, def::FMX / 2 + 75, 200, WHITE, FALSE);// �t���[���`��

	// �G�̃��C���摜�`��
	switch( select ){
	case def::eKIND_���b�g�[��1:
		allEnemy[EnemyMgr::ID_���b�g�[��]->DrawAnim(def::FMX / 2, def::FMY / 2 - 30);
		break;
	case def::eKIND_���b�g�[��3:
		allEnemy[EnemyMgr::ID_���b�g�[��]->DrawAnim(def::FMX / 2, def::FMY / 2 - 40);
		allEnemy[EnemyMgr::ID_���b�g�[��]->DrawAnim(def::FMX / 2 - 30, def::FMY / 2 - 10);
		allEnemy[EnemyMgr::ID_���b�g�[��]->DrawAnim(def::FMX / 2 + 30, def::FMY / 2 - 10);
		break;
	case def::eKIND_�r���[:
		allEnemy[EnemyMgr::ID_�r���[]->DrawAnim(def::FMX / 2, def::FMY / 2 - 30);
		break;
	case def::eKIND_���b�g�[��_�r���[:
		allEnemy[EnemyMgr::ID_�r���[]->DrawAnim(def::FMX / 2 + 25, def::FMY / 2 - 20);
		allEnemy[EnemyMgr::ID_���b�g�[��]->DrawAnim(def::FMX / 2 - 40, def::FMY / 2 - 20);
		break;
	case def::eKIND_�t�H���e:
		allEnemy[EnemyMgr::ID_�t�H���e]->DrawAnim(def::FMX / 2, def::FMY / 2 - 10);
		break;
	case def::eKIND_���:
		DrawString(def::FMX / 2 - 20, def::FMY / 2 - 40, "���", WHITE);
		break;
	default:
		ASSERT(0,"Main::StateCharSelect::Draw wrong enemy kind ("+ToString<int>(select)+")");
		break;
	}

	// �I�����ꗗ
	for( int i = 0; i < def::eKIND_MAX; i++ ){
		const int size = 40;
		int x = ( i%SELECT_X_NUM )*( size + 2 ) + 180;
		int y = ( i / SELECT_X_NUM )*size + 230;
		DrawBox(x, y, x + size, y + size, WHITE, FALSE);
		DrawFormatString(x + size / 2 - 5, y + size / 2 - 5, WHITE, "%d", i + 1);

		if( i == select && ( count / 30 ) % 2 == 0 ){
			const int th = 4;

			DrawBox(x, y, x + th, y + size, RED, TRUE);
			DrawBox(x + size, y, x + size - th, y + size, RED, TRUE);
			DrawBox(x, y, x + size, y + th, RED, TRUE);
			DrawBox(x, y + size, x + size, y + size - th, RED, TRUE);
		}
	}
}

void Main::StateCharSelect::Process(){
	count++;

	// todo(char[select]�̃��[�V����[�ړ��Ƃ��U���������_����])

	if( CKey::GetInst()->CheckKey(eKEY_LEFT) == 1 ){
		if( select == 0 )
			select = def::eKIND_MAX - 1;
		else
			select--;
	} else if( CKey::GetInst()->CheckKey(eKEY_RIGHT) == 1 ){
		if( select == def::eKIND_MAX - 1 )
			select = 0;
		else
			select++;
	} else if( CKey::GetInst()->CheckKey(eKEY_UP) == 1 ){
		if( select >= SELECT_X_NUM )
			select -= SELECT_X_NUM;
	} else if( CKey::GetInst()->CheckKey(eKEY_DOWN) == 1 ){
		if( select < def::eKIND_MAX - SELECT_X_NUM )
			select += SELECT_X_NUM;
	} else if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 ){
		std::list<BattleEnemyInit_t> data;
		bool isBoss = false;
		switch( select ){
		case def::eKIND_���b�g�[��1:
			data.push_back(BattleEnemyInit_t{ EnemyMgr::ID_���b�g�[��,CPoint<int>(4,1) });
			break;
		case def::eKIND_���b�g�[��3:
			data.push_back(BattleEnemyInit_t{ EnemyMgr::ID_���b�g�[��,CPoint<int>(5,1) });
			data.push_back(BattleEnemyInit_t{ EnemyMgr::ID_���b�g�[��,CPoint<int>(3,0) });
			data.push_back(BattleEnemyInit_t{ EnemyMgr::ID_���b�g�[��,CPoint<int>(3,2) });
			break;
		case def::eKIND_�r���[:
			data.push_back(BattleEnemyInit_t{ EnemyMgr::ID_�r���[,CPoint<int>(4,1) });
			break;
		case def::eKIND_���b�g�[��_�r���[:
			data.push_back(BattleEnemyInit_t{ EnemyMgr::ID_�r���[,CPoint<int>(4,0) });
			data.push_back(BattleEnemyInit_t{ EnemyMgr::ID_���b�g�[��,CPoint<int>(5,2) });
			break;
		case def::eKIND_�t�H���e:
			data.push_back(BattleEnemyInit_t{ EnemyMgr::ID_�t�H���e,CPoint<int>(4,1) });
			isBoss = true;
			break;
		case def::eKIND_���:
			obj->stateMgr.ChangeNext(new StateLog(obj));
			return;
		default:
			ASSERT(0, "Main::StateCharSelect::Process wrong enemy kind (" + ToString<int>(select) + ")");
			break;
		}
		obj->stateMgr.ChangeNext(new StateBattle(obj, data, isBoss, select));
	}
}

//-------------------------------------------------------
// �o�g��
//-------------------------------------------------------
Main::StateBattle::StateBattle(Main * obj, std::list<BattleEnemyInit_t> enemys, bool isBoss, int battleEnemyKind)
	:obj(obj), battle(enemys, isBoss), battleEnemyKind(battleEnemyKind){
}

Main::StateBattle::~StateBattle(){
}

void Main::StateBattle::Draw(){
	battle.Draw();
}

void Main::StateBattle::Process(){
	switch( battle.Process() ){
	case eBATTLE_RTN_WIN:
		PlayerMgr::GetInst()->IncrementBattleResult(PlayerMgr::eBT_RTN_WIN, battleEnemyKind);
		obj->stateMgr.ChangeNext(new StateCharSelect(obj));// �G�I����ʂɖ߂�
		break;
	case eBATTLE_RTN_LOSE:
		PlayerMgr::GetInst()->IncrementBattleResult(PlayerMgr::eBT_RTN_LOSE, battleEnemyKind);
		obj->stateMgr.ChangeNext(new StateGameOver(obj));// �Q�[���I�[�o�[��ʂ�
		break;
	}
}

//-------------------------------------------------------
// ���
//-------------------------------------------------------
Main::StateLog::StateLog(Main * obj):obj(obj){
}

Main::StateLog::~StateLog(){
}

void Main::StateLog::Draw(){
	static const std::string SELECT_NAMES[def::eKIND_MAX-1] = {
		"���b�g�[�� x1        ",
		"���b�g�[�� x3        ",
		"�r���[ x1            ",
		"���b�g�[�� and �r���[",
		"�t�H���e             ",
	};

	// ��� (�����񐔁E�s�k��)
//	int win_num = PlayerMgr::GetInst()->GetBattleResult("win");
//	int lose_num = PlayerMgr::GetInst()->GetBattleResult("lose");
	DrawStringCenter(def::FMX / 2, 50, "--���--", WHITE);
	DrawString(250,80,"������  �s�k��",WHITE);
	for( int i = 0; i < sizeof(SELECT_NAMES) / sizeof(SELECT_NAMES[0]); i++ ){
		DrawString(50,110+i*30, SELECT_NAMES[i].c_str(),WHITE);
		int win = PlayerMgr::GetInst()->GetBattleResult(PlayerMgr::eBT_RTN_WIN,i);
		int lose = PlayerMgr::GetInst()->GetBattleResult(PlayerMgr::eBT_RTN_LOSE, i);
		DrawFormatString(280, 110 + i * 30,WHITE,"%4d",win);
		DrawFormatString(365, 110 + i * 30, WHITE, "%4d", lose);
	}
}

void Main::StateLog::Process(){
	if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 || CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ){
		obj->stateMgr.ChangeNext(new StateCharSelect(obj));
	}
}

//-------------------------------------------------------
// �Q�[���I�[�o�[
//-------------------------------------------------------
void Main::StateGameOver::ReshapePosition(){
	for( int y = 0; y < 19; y++ ){
		for( int n = 0; n < 3; n++ ){
			for( std::vector<CPoint<int>>::iterator it = back_box_pos[y][n].begin(); it != back_box_pos[y][n].end(); it++ ){
				// x�����e�͈͊O�ɂ���Ȃ�߂�
				while( it->x < -ABS_BOX_RANGE_SIZE )
					it->x += ( def::FMX + ABS_BOX_RANGE_SIZE );
				while( it->x > def::FMX + ABS_BOX_RANGE_SIZE )
					it->x -= ( def::FMX + ABS_BOX_RANGE_SIZE * 2 );
			}
		}
	}
}

void Main::StateGameOver::BoxDraw(){
	for( int y = 0; y < 19; y++ ){
		for( int n = 0; n < 3; n++ ){
			for( std::vector<CPoint<int>>::iterator it = back_box_pos[y][n].begin(); it != back_box_pos[y][n].end(); it++ ){
				if( it->x + BACK_BOX_SIZE_X > 0 && it->x < def::FMX ){// �`��͈͓��ɂ���Ȃ�
					DrawBox(it->x, it->y, it->x + BACK_BOX_SIZE_X, it->y + BACK_BOX_SIZE_Y, GetColor(160, 56, 128), TRUE);
				}
			}
		}
	}
}

Main::StateGameOver::StateGameOver(Main * obj):obj(obj), count(0){
	std::string fname = def::IMAGE_FILE_PATH + "game_over.png";
	image = LoadGraphWithCheckError(fname, "Main::StateGameOver::StateGameOver");

	// block size: 4,7,8
	const int init_rand_range = ABS_BOX_RANGE_SIZE * 2 + def::FMX;
	for( int y = 0; y < 19; y++ ){
		// rnd_x = random value in left_screen - FMX to right_screen + FMX
		int rnd_x = rand() % init_rand_range - ABS_BOX_RANGE_SIZE;
		int block_size = 4;
		int ty = y*BACK_BOX_SIZE_Y + OFFSET_UP;
		for( int i = 0; i < block_size; i++ ){
			back_box_pos[y][0].push_back(CPoint<int>(rnd_x + i*BACK_BOX_SIZE_X, ty));
		}

		// back_box_pos[y][1-2] is calclated by back_box_pos[y][0]
		const int space = def::FMX * 6 / 7;
		for( int i = 1; i < 3; i++ ){
			block_size = 6 + i;
			CPoint<int> t = back_box_pos[y][0][0];
			t.x += space*i;// add space
			for( int j = 0; j < block_size; j++ ){
				t.x += BACK_BOX_SIZE_X;// add block length
				back_box_pos[y][i].push_back(t);
			}
		}
	}

	ReshapePosition();

	std::string init_pos = "Init GameOver Block Position:\n";
	int max_x = -ABS_BOX_RANGE_SIZE, min_x = def::FMX + ABS_BOX_RANGE_SIZE;
	for( int y = 0; y < 19; y++ ){
		init_pos += "  [";
		for( int n = 0; n < 3; n++ ){
			init_pos += " [";
			for( std::vector<CPoint<int>>::iterator it = back_box_pos[y][n].begin(); it != back_box_pos[y][n].end(); it++ ){
				init_pos += ToString("(%d,%d),", it->x, it->y);
				if( it->x > max_x )
					max_x = it->x;
				if( it->x < min_x )
					min_x = it->x;
			}
			init_pos.erase(--init_pos.end());// �Ō�̃J���}���폜
			init_pos += "],";
		}
		init_pos.erase(--init_pos.end());// �Ō�̃J���}���폜
		init_pos += "]\n";
	}
	init_pos += ToString("    max x: %d, min x: %d\n", max_x, min_x);
	AppLogger::Log(AppLogger::eLOG_DEBUG, init_pos);
}

Main::StateGameOver::~StateGameOver(){
}

void Main::StateGameOver::Draw(){
	DrawBox(0, 0, def::FMX, def::FMY, GetColor(128, 24, 96), TRUE);
	BoxDraw();

	DrawGraph(0, 0, image, TRUE);
}

void Main::StateGameOver::Process(){
	// BOX���ړ������鏈��
	const int slide_value = 2;
	for( int y = 0; y < 19; y++ ){
		for( int n = 0; n < 3; n++ ){
			for( std::vector<CPoint<int>>::iterator it = back_box_pos[y][n].begin(); it != back_box_pos[y][n].end(); it++ ){
				it->x -= slide_value;
			}
		}
	}
	ReshapePosition();

	// �L�[�������ꂽ��^�C�g���ɖ߂�
	if( count > 60 ){
		if( CKey::GetInst()->CheckKey(eKEY_CANCEL) != 0 || CKey::GetInst()->CheckKey(eKEY_ENTER) != 0 ){
			obj->stateMgr.ChangeNext(new StateTitle(obj));
		}
	}
	count++;
}
