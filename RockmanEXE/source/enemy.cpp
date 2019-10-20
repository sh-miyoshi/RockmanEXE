#include "include.h"
#include "enemy.h"
#include "battleCharMgr.h"
#include "battleField.h"

class EnemyBase:public BattleCharBase {
protected:
	unsigned int count;
	int imgNumber[10];
public:
	EnemyBase(std::string name, unsigned int hp);
	~EnemyBase();

	virtual void Draw();
};

class Enemy_�I:public EnemyBase {
public:
	Enemy_�I();
	~Enemy_�I();
};

class Enemy_���b�g�[��:public EnemyBase {
	static const int Power_�V���b�N�E�F�[�u = 10;// debug(�Œ�H)

	class AnimMove:public Animation {
		Enemy_���b�g�[��* obj;
	public:
		AnimMove(Enemy_���b�g�[��* obj):obj(obj) {}

		void Begin() {
			int y = BattleCharMgr::GetInst()->GetClosestCharPos(obj->pos, eCHAR_PLAYER).y;
			if( y < 0 ) {// ���b�N�}����������Ȃ�����
				// �������Ȃ�
			} else if( y < obj->pos.y ) {// ���b�N�}������ɂ���Ȃ�
				if( obj->MoveCheck(obj->pos.x, obj->pos.y - 1) ) {
					obj->SetPos(obj->pos.x, obj->pos.y - 1);
				}
			} else {
				if( obj->MoveCheck(obj->pos.x, obj->pos.y + 1) ) {
					obj->SetPos(obj->pos.x, obj->pos.y + 1);
				}
			}
		}
	};

	unsigned int waveCount;
	std::shared_ptr<AnimMove> animMove;
public:
	Enemy_���b�g�[��();
	~Enemy_���b�g�[��();

	virtual void Process();
};

//-------------------------------------------------------
// �S�̏���
//-------------------------------------------------------
std::shared_ptr<BattleCharBase> EnemyMgr::GetData(int id) {
	switch( id ) {
	case ID_�I:
		return std::shared_ptr<Enemy_�I>(new Enemy_�I());
	case ID_���b�g�[��:
		return std::shared_ptr<Enemy_���b�g�[��>(new Enemy_���b�g�[��());
	default:
		AppLogger::Error("EnemyMgr::GetData wrong char id (%d)", id);
		exit(1);
	}
	return std::shared_ptr<BattleCharBase>();
}

//-------------------------------------------------------
// �G�֘A�̊��N���X
//-------------------------------------------------------
EnemyBase::EnemyBase(std::string name, unsigned int hp)
	:count(0), BattleCharBase(name, hp, hp, eCHAR_ENEMY), imgNumber() {
	std::string fname = def::IMAGE_FILE_PATH + "���̑�/number_2.png";
	LoadDivGraphWithErrorCheck(imgNumber, fname, "EnemyBase::EnemyBase", 10, 1, 15, 20);
}

EnemyBase::~EnemyBase() {
	for( auto i = 0; i < 10; i++ ) {
		DeleteGraph(imgNumber[i]);
	}
}

void EnemyBase::Draw() {
	// TODO(HP��0�Ȃ�SetDrawBlendMode)
	BattleCharBase::Draw();

	// HP�̕`��
	CPoint<int> t = BattleField::GetPixelPos(CPoint<int>(pos.x, pos.y + 1));
	std::list<unsigned int> hpList;
	for( unsigned int h = this->hp; h > 0; h /= 10 ) {
		hpList.push_front(h % 10);
	}
	const int numSize = 15;
	t.x = t.x - numSize * hpList.size() / 2 + 7;
	for( auto h : hpList ) {
		unsigned pointer = ( h == 0 ) ? 9 : h - 1;
		DrawRotaGraph(t.x, t.y, 1, 0, imgNumber[pointer], TRUE);
		t.x += numSize;
	}
}


//-------------------------------------------------------
// �e�X�g�p�̓I
//-------------------------------------------------------
Enemy_�I::Enemy_�I():EnemyBase("�I", 10) {
	std::string fname = def::CHARACTER_IMAGE_PATH + "�I.png";
	std::shared_ptr<Animation> animStand = std::shared_ptr<Animation>(new Animation());
	animStand->LoadData(fname, CPoint<unsigned int>(100, 117), CPoint<unsigned int>(1, 1));
	BattleCharBase::SetDefaultAnim(animStand);
}

Enemy_�I::~Enemy_�I() {
}

//-------------------------------------------------------
// ���b�g�[��
//-------------------------------------------------------
Enemy_���b�g�[��::Enemy_���b�g�[��()
	:EnemyBase("���b�g�[��", 40), waveCount(0) {
	std::string fname = def::CHARACTER_IMAGE_PATH + "���b�g�[��_stand.png";
	std::shared_ptr<Animation> animStand = std::shared_ptr<Animation>(new Animation());
	animStand->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(1, 1));
	BattleCharBase::SetDefaultAnim(animStand);

	fname = def::CHARACTER_IMAGE_PATH + "���b�g�[��_move.png";
	animMove = std::shared_ptr<AnimMove>(new AnimMove(this));
	animMove->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(1, 1));
}

Enemy_���b�g�[��::~Enemy_���b�g�[��() {
}

void Enemy_���b�g�[��::Process() {
	// �f���[�g���[�V������(����s��)
	if( hp <= 0 ) {
		return;
	}

	static const int WAIT_COUNT = 1 * 60;// �ŏ��̉������Ȃ��J�E���g
	static const int MOVE_COUNT = 1 * 60;
	static const int SHOCK_WAVE_COUNT = 30;

	//-------------------------------
	// �ŏ���n�J�E���g:	�������Ȃ�
	// �ȍ~���[�v
	//	�v���C���[�T��
	//	if y���W������&��莞��
	//		�U���o�^
	//-------------------------------
	if( AnimProcess() ) {
		if( count >= WAIT_COUNT ) {
			int y = BattleCharMgr::GetInst()->GetClosestCharPos(pos, eCHAR_PLAYER).y;// ���b�N�}���̈ʒu
			if( pos.y == y )
				waveCount++;
			else
				waveCount = 0;

			if( waveCount >= SHOCK_WAVE_COUNT ) {
				// TODO(�V���b�N�E�F�[�u��ł�)
				waveCount = 0;
				count = 0;
			} else if( count % MOVE_COUNT == 0 ) {
				this->AttachAnim(animMove);
			}
		}
		count++;
	}
}
