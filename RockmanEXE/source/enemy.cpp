#include "include.h"
#include "enemy.h"
#include "battleCharMgr.h"

class Enemy_���b�g�[��:public BattleCharBase {
	static const int Power_�V���b�N�E�F�[�u = 10;// debug(�Œ�H)

	class AnimMove:public Animation {
		Enemy_���b�g�[��* obj;
	public:
		AnimMove(Enemy_���b�g�[��* obj):obj(obj) {}

		void Begin() {
			int y = BattleCharMgr::GetInst()->GetClosestCharPos(obj->pos, eCHAR_PLAYER).y;
			if( y < obj->pos.y ) {// ���b�N�}������ɂ���Ȃ�
				if( obj->MoveCheck(obj->pos.x, obj->pos.y - 1) ) {
					obj->SetPos(obj->pos.x, obj->pos.y - 1);
				}
			} else if( y > obj->pos.y ) {
				if( obj->MoveCheck(obj->pos.x, obj->pos.y + 1) ) {
					obj->SetPos(obj->pos.x, obj->pos.y + 1);
				}
			}
		}
	};

	unsigned int count;
	unsigned int waveCount;
	std::shared_ptr<Animation> animStand;
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
	case ID_���b�g�[��:
		return std::shared_ptr<Enemy_���b�g�[��>(new Enemy_���b�g�[��());
	default:
		AppLogger::Error(ToString("EnemyMgr::GetData wrong char id (%d)", id));
		exit(1);
	}
	return std::shared_ptr<BattleCharBase>();
}

//-------------------------------------------------------
// ���b�g�[��
//-------------------------------------------------------
Enemy_���b�g�[��::Enemy_���b�g�[��()
	:BattleCharBase("���b�g�[��", 40, 40, eCHAR_ENEMY, animStand), count(0), waveCount(0) {
	std::string fname = def::ENEMY_IMAGE_PATH + "���b�g�[��_stand.png";
	animStand->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(1, 1));

	fname = def::ENEMY_IMAGE_PATH + "���b�g�[��_move.png";
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

	AnimProcess();
}
