#include "include.h"
#include "enemy.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"
#include "effectMgr.h"

class EnemyBase:public BattleCharBase{
protected:
	int count;
	int atkCount;
public:
	EnemyBase(std::string name, int hp)
		:BattleCharBase(name, hp, def::eCHAR_ENEMY), count(0), atkCount(0){}
	~EnemyBase(){}

	virtual void Process(){
		BattleCharBase::Process();

		// ���j�摜��o�^
		if( deleteCount == 1 ){
			int x = BattleField::PANEL_SIZE.x*pos.x + BattleField::PANEL_SIZE.x / 2;
			int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*pos.y - 10;// �ӂ��̕������グ��
			EffectMgr::GetInst()->Register(EffectMgr::eID_EXPLODE,CPoint<int>(x,y));
		}
	}
};

class Enemy_���b�g�[��:public EnemyBase{
	static const int Power_�V���b�N�E�F�[�u = 10;// debug(�Œ�H)

	class ActMove:public AnimProcess{
		Enemy_���b�g�[�� *obj;
	public:
		ActMove(Enemy_���b�g�[�� *obj):obj(obj){}

		void Begin(){
			int y = BattleCharMgr::GetInst()->GetTargetPos(obj->pos, def::eCHAR_PLAYER).y;
			if( y < obj->pos.y )// ���b�N�}������ɂ���Ȃ�
				obj->Move(def::eMUKI_UP, true);
			else if( y > obj->pos.y )
				obj->Move(def::eMUKI_DOWN, true);
		}
	};
	class Act_�V���b�N�E�F�[�u:public AnimProcess{
		bool regdFlag;
		int count;
		Enemy_���b�g�[�� *obj;
	public:
		Act_�V���b�N�E�F�[�u(Enemy_���b�g�[�� *obj):obj(obj), count(0), regdFlag(false){}

		void Begin(){
			regdFlag = false;
			count = 0;
		}

		void Process(){
			const int timing = 12 * obj->anim[ANIM_ATK1].imageDelay;
			if( count >= timing && !regdFlag ){// �U�����s���^�C�~���O�ɂȂ�����
				// �U���̓o�^
				SkillArg args;
				args.atkPos = obj->pos;
				args.damage = Power_�V���b�N�E�F�[�u;
				BattleCharMgr::GetInst()->RegisterAttack(SkillMgr::GetData(SkillMgr::eID_�V���b�N�E�F�[�u, args, def::eCHAR_ENEMY));
				regdFlag = true;
			}
			count++;
		}
	};
public:
	Enemy_���b�g�[��();
	~Enemy_���b�g�[��();

	void Process();
	void LoadAnim();
};

class Enemy_�r���[:public EnemyBase{
	static const int Power_�T���_�[�{�[�� = 20;// debug(�Œ�H)

	class ActMove:public AnimProcess{
		Enemy_�r���[ *obj;
	public:
		ActMove(Enemy_�r���[ *obj):obj(obj){}

		void Begin(){
			// �����������_���Ō��߂�
			int direct = -1;
			switch( rand() % 4 ){
			case 0: direct = def::eMUKI_UP; break;
			case 1: direct = def::eMUKI_DOWN; break;
			case 2: direct = def::eMUKI_RIGHT; break;
			case 3: direct = def::eMUKI_LEFT; break;
			default: ASSERT(0,ToString("Enemy_�r���[::ActMove::Begin illegal direction (%d)",direct));// �N���肦�Ȃ�
			}
			obj->Move(direct, true);
		}
	};
	class Act_�T���_�[�{�[��:public AnimProcess{
		bool regdFlag;
		int count;
		Enemy_�r���[ *obj;
	public:
		Act_�T���_�[�{�[��(Enemy_�r���[ *obj):obj(obj){}

		void Begin(){
			regdFlag = false;
			count = 0;
		}

		void Process(){
			const int timing = 7 * obj->anim[ANIM_ATK1].imageDelay;
			if( count >= timing && !regdFlag ){// �U�����s���^�C�~���O�ɂȂ�����
				// �U���̓o�^
				SkillArg args;
				args.atkPos = obj->pos;
				args.damage = Power_�T���_�[�{�[��;
				BattleCharMgr::GetInst()->RegisterAttack(SkillMgr::GetData(SkillMgr::eID_�T���_�[�{�[��, args , def::eCHAR_ENEMY));
				regdFlag = true;
			}
			count++;
		}
	};
public:
	Enemy_�r���[();
	~Enemy_�r���[();

	void Process();
	void LoadAnim();
};

// todo(�ړ����̖��G���Ԃ���)
// todo(�_�[�N�A�[���u���[�h�悯���)
class Enemy_�t�H���e:public EnemyBase{
	class ActMove:public AnimProcess{
		Enemy_�t�H���e *obj;
	public:
		ActMove(Enemy_�t�H���e *obj):obj(obj){}

		void Begin(){
			CPoint<int> pos = obj->moveTargetPos.front();
			obj->moveTargetPos.pop();
			obj->isMoveSuccess = obj->MoveCheck(pos.x, pos.y, false);
			if( obj->isMoveSuccess ){
				obj->pos = pos;
			}
		}
	};
	class Act_�V���[�e�B���O�o�X�^�[:public AnimProcess{
		static const int ATK_DELAY = 15;
		static const int ATK_WAIT_TIME = 30;

		Enemy_�t�H���e *obj;
		int count;
		CPoint<int> atkPos;
		int atkNum;
	public:
		static const int ATTACK_NUM = 15;
		static const int Power_�V���[�e�B���O�o�X�^�[ = 40;// debug(�_���[�W)

		Act_�V���[�e�B���O�o�X�^�[(Enemy_�t�H���e *obj):obj(obj){}
		void Begin(){
			count = 0;
			atkNum = 0;

			// �����̃��b�N�}���̈ʒu�̒����烉���_���őI��
			atkPos.x = rand() % BattleField::FIELD_NUM_X / 2;
			atkPos.y = rand() % BattleField::FIELD_NUM_Y;
		}

		void Process(){
			count++;
			if( count < ATK_WAIT_TIME )// ��莞�ԑ҂�
				return;

			if( count % ATK_DELAY == ATK_DELAY - 1 ){
				// �U���̓o�^
				SkillArg args;
				args.atkPos = atkPos;
				args.damage = Power_�V���[�e�B���O�o�X�^�[;
				std::shared_ptr<SkillData> skill = SkillMgr::GetData(SkillMgr::eID_�V���[�e�B���O�o�X�^�[, args, def::eCHAR_ENEMY);
				BattleCharMgr::GetInst()->RegisterAttack(skill);

				atkPos.x = rand() % BattleField::FIELD_NUM_X / 2;
				atkPos.y = rand() % BattleField::FIELD_NUM_Y;
				atkNum++;
			}
		}
	};
	class Act_�w���Y���[�����O:public AnimProcess{
		static const int POWER_�w���Y���[�����O = 40;// debug(�_���[�W��)
		static const int IMAGE_DELAY = 8;

		Enemy_�t�H���e *obj;
		int count;
		int drawCount;
		bool drawFlag;
	public:
		Act_�w���Y���[�����O(Enemy_�t�H���e *obj)
			:obj(obj), count(0), drawCount(0), drawFlag(false){
		}

		void Begin(){
			count = 0;
			drawFlag = false;
			drawCount = 0;
		}

		void Draw(){
			if( drawFlag ){
				int x = BattleField::PANEL_SIZE.x*obj->pos.x + BattleField::PANEL_SIZE.x / 2;
				int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*obj->pos.y - 10;// �ӂ��̕������グ��
				int no = obj->img�w���Y���[�����O[drawCount / IMAGE_DELAY];
				DrawRotaGraph(x - 50, y - 30, 1, 0, no, TRUE);
			}
		}

		void Process(){
			if( count == 45 ){
				// �U���̓o�^
				SkillArg args;
				args.atkPos = obj->pos;
				args.damage = POWER_�w���Y���[�����O;
				std::shared_ptr<SkillData> skill = SkillMgr::GetData(SkillMgr::eID_�w���Y���[�����O_UP, args, def::eCHAR_ENEMY);
				BattleCharMgr::GetInst()->RegisterAttack(skill);
				drawFlag = true;
			} else if( count == 75 ){
				// �U���̓o�^
				SkillArg args;
				args.atkPos = obj->pos;
				args.damage = POWER_�w���Y���[�����O;
				std::shared_ptr<SkillData> skill = SkillMgr::GetData(SkillMgr::eID_�w���Y���[�����O_DOWN, args, def::eCHAR_ENEMY);
				BattleCharMgr::GetInst()->RegisterAttack(skill);
				drawFlag = true;
			}
			if( drawFlag ){
				drawCount++;
				if( drawCount >= ( IMAGE_DELAY * 3 ) ){
					drawFlag = false;
					drawCount = 0;
				}
			}
			count++;
		}
	};
	class Act_�_�[�N�A�[���u���[�h:public AnimProcess{
		static const int POWER_�_�[�N�A�[���u���[�h = 40;// debug(�_���[�W��)

		Enemy_�t�H���e *obj;
	public:
		Act_�_�[�N�A�[���u���[�h(Enemy_�t�H���e *obj):obj(obj){}

		void Begin(){
			// �U���̓o�^
			CPoint<int> pos = obj->pos;
			pos.x--;// todo(��납��̍U��)
			SkillArg args;
			args.atkPos = pos;
			args.damage = POWER_�_�[�N�A�[���u���[�h;
			std::shared_ptr<SkillData> skill = SkillMgr::GetData(SkillMgr::eID_�_�[�N�A�[���u���[�h_SHORT, args, def::eCHAR_ENEMY);
			BattleCharMgr::GetInst()->RegisterAttack(skill);
		}
	};

	std::queue<CPoint<int>> moveTargetPos;
	CPoint<int> prevPos;
	int nextMoveNum;
	bool isMoveSuccess;
	int img�w���Y���[�����O[3];

	CPoint<int> GetMoveTargetPos();
	void Attack_�V���[�e�B���O�o�X�^�[();
	void Attack_�w���Y���[�����O();
	void Attack_�_�[�N�A�[���u���[�h_1();
	void Attack_�_�[�N�A�[���u���[�h_3();
	void Attack_�_�[�N�l�X�I�[�o�[���[�h();
public:
	Enemy_�t�H���e();
	~Enemy_�t�H���e();

	void Process();
	void LoadAnim();
	virtual void DeleteAnim();
	virtual void Draw();
};

//-------------------------------------------------------
// �O���֐�
//-------------------------------------------------------
std::shared_ptr<BattleCharBase> EnemyMgr::GetData(int id){
	switch( id ){
	case ID_���b�g�[��:
		return std::shared_ptr<BattleCharBase>(new Enemy_���b�g�[��());
	case ID_�r���[:
		return std::shared_ptr<BattleCharBase>(new Enemy_�r���[());
	case ID_�t�H���e:
		return std::shared_ptr<BattleCharBase>(new Enemy_�t�H���e());
	default:
		ASSERT(0,ToString("EnemyMgr::GetData wrong char id (%d)",id));
	}
	return std::shared_ptr<BattleCharBase>();
}

std::list<EnemyMgr::EnemyAttackChip_t> EnemyMgr::GetEnemyChip(int id){
	static std::list<EnemyAttackChip_t> result;
	EnemyAttackChip_t t;
	t.charID = id;

	// �e�L�������ƂɃ`�b�v�����Z�b�g
	switch( id ){
	case ID_�r���[:
		t.chip.id = ChipMgr::eID_�T���_�[�{�[��;
		t.bustingLevel = 6;
		t.chip.code = 'A';
		result.push_back(t);
		t.bustingLevel = 11;
		t.chip.code = '*';
		result.push_back(t);
		break;
	}

	return result;
}

//-------------------------------------------------------
// ���b�g�[��
//-------------------------------------------------------
Enemy_���b�g�[��::Enemy_���b�g�[��():EnemyBase("���b�g�[��", 40){
	anim[ANIM_ATK1].imageDelay = 4;// �V���b�N�E�F�[�u���[�V�����̃f�B���C

							 // �A�j���[�V�����̐ݒ�
	anim[ANIM_MOVE].animProc = std::shared_ptr<ActMove>(new ActMove(this));
	anim[ANIM_ATK1].animProc = std::shared_ptr<Act_�V���b�N�E�F�[�u>(new Act_�V���b�N�E�F�[�u(this));
}

Enemy_���b�g�[��::~Enemy_���b�g�[��(){
}

void Enemy_���b�g�[��::Process(){
	EnemyBase::Process();
	// �f���[�g���[�V������(����s��)
	if( hp <= 0 ){
		return;
	}

	static const int WAIT_COUNT = 1 * 60;// �ŏ��̉������Ȃ��J�E���g
	static const int MOVE_COUNT = 1 * 60;
	static const int ATK1_COUNT = 30;

	//-------------------------------
	// �ŏ���n�J�E���g:	�������Ȃ�
	// �ȍ~���[�v
	//	�v���C���[�T��
	//	if y���W������&��莞��
	//		�U���o�^
	//-------------------------------
	if( AnimProcess() ){
		if( count >= WAIT_COUNT ){
			int y = BattleCharMgr::GetInst()->GetTargetPos(pos, def::eCHAR_PLAYER).y;// ���b�N�}���̈ʒu
			if( pos.y == y )
				atkCount++;
			else
				atkCount = 0;

			if( atkCount >= ATK1_COUNT ){
				SetAnim(ANIM_ATK1);
				atkCount = 0;
				count = 0;
			} else if( count%MOVE_COUNT == 0 )
				SetAnim(ANIM_MOVE);
		}
		count++;
	}
}

void Enemy_���b�g�[��::LoadAnim(){
	static const std::string position = "Enemy_���b�g�[��:LoadAnim";

	std::string fname = def::ENEMY_IMAGE_PATH + "���b�g�[��_move.png";
	LoadDivGraphWithCheckError(anim[ANIM_MOVE].image, fname, position, 1, 1, 100, 100);
	anim[ANIM_MOVE].imageNum = 1;

	fname = def::ENEMY_IMAGE_PATH + "���b�g�[��_atk.png";
	LoadDivGraphWithCheckError(anim[ANIM_ATK1].image, fname, position, 15, 1, 100, 140);
	anim[ANIM_ATK1].imageNum = 15;
}

//-------------------------------------------------------
// �r���[
//-------------------------------------------------------
Enemy_�r���[::Enemy_�r���[():EnemyBase("�r���[", 50){
	anim[ANIM_ATK1].imageDelay = 6;// debug(�K��)

							 // �A�j���[�V�����̐ݒ�
	anim[ANIM_MOVE].animProc = std::shared_ptr<ActMove>(new ActMove(this));
	anim[ANIM_ATK1].animProc = std::shared_ptr<Act_�T���_�[�{�[��>(new Act_�T���_�[�{�[��(this));
}

Enemy_�r���[::~Enemy_�r���[(){
}

void Enemy_�r���[::Process(){
	EnemyBase::Process();
	// �f���[�g���[�V������(����s��)
	if( hp <= 0 ){
		return;
	}

	static const int WAIT_COUNT = 90;// �ŏ��̉������Ȃ��J�E���g
	static const int ACT_COUNT = 1 * 60;

	//-------------------------------
	// �ŏ���n�J�E���g:	�������Ȃ�
	// �ȍ~���[�v
	//	4/5: �K���Ɉړ�
	//	1/5: �U��
	//-------------------------------

	if( AnimProcess() ){
		if( count >= WAIT_COUNT ){
			if( count%ACT_COUNT == 0 ){// �s������
				if( atkCount < 4 ){//�ړ�����
					SetAnim(ANIM_MOVE);
					atkCount++;
				} else{// �U������
					// todo(�����̍U�������݂��Ȃ��Ȃ�ABattleMgr���狭�����[�V�����ݒ�ŉ��)
					SetAnim(ANIM_ATK1);
					atkCount = 0;
					count = 0;
				}
			}
		}
		count++;
	}
}

void Enemy_�r���[::LoadAnim(){
	static const std::string position = "Enemy_�r���[:LoadAnim";
	int temp[8 * 3];
	std::string fname = def::ENEMY_IMAGE_PATH + "�r���[.png";
	LoadDivGraphWithCheckError(temp, fname, position, 8, 3, 112, 114);
	for( int i = 0; i < 8; i++ )// �ŏ���8�͎g��Ȃ�
		DeleteGraph(temp[i]);
	for( int i = 0; i < 6; i++ )// ����6�͈ړ�
		anim[ANIM_MOVE].image[i] = temp[i + 8];
	DeleteGraph(temp[6 + 8]);
	DeleteGraph(temp[7 + 8]);
	for( int i = 0; i < 8; i++ )// �Ō��8�͍U��
		anim[ANIM_ATK1].image[i] = temp[i + 16];
	anim[ANIM_MOVE].imageNum = 6;
	anim[ANIM_ATK1].imageNum = 8;
}

//-------------------------------------------------------
// �t�H���e
//-------------------------------------------------------
Enemy_�t�H���e::Enemy_�t�H���e()
	:EnemyBase("�t�H���e", 1000), prevPos(-1, -1), nextMoveNum(1)
{
	// �A�j���[�V�����̐ݒ�
	anim[ANIM_MOVE].animProc = std::shared_ptr<ActMove>(new ActMove(this));
	anim[ANIM_ATK1].animProc = std::shared_ptr<Act_�V���[�e�B���O�o�X�^�[>(new Act_�V���[�e�B���O�o�X�^�[(this));
	anim[ANIM_ATK2].animProc = std::shared_ptr<Act_�w���Y���[�����O>(new Act_�w���Y���[�����O(this));
	anim[ANIM_ATK3].animProc = std::shared_ptr<Act_�_�[�N�A�[���u���[�h>(new Act_�_�[�N�A�[���u���[�h(this));

	anim[ANIM_MOVE].imageDelay = 2;
	anim[ANIM_ATK1].imageDelay = 6;
	anim[ANIM_ATK2].imageDelay = 6;
	anim[ANIM_ATK3].imageDelay = 2;

	drawPosOffset.y = -30;
}

Enemy_�t�H���e::~Enemy_�t�H���e(){
}

void Enemy_�t�H���e::Draw(){
	// ��ʒu���W
	int x = BattleField::PANEL_SIZE.x*pos.x + BattleField::PANEL_SIZE.x / 2;
	int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*pos.y - 10;// �ӂ��̕������グ��
	int ino = -1;

	if( hp <= 0 ){
		SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
		int image = anim[ANIM_DAMAGE].image[0];
		// todo(�A�j���[�V�����̒ǉ�)
		for( int i = 0; i < 10; i++ )
			DrawRotaGraph(x + drawPosOffset.x, y + drawPosOffset.y, 1, 0, image, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		return;
	}

	switch( BattleCharBase::atachedAnimNo ){
	case ANIM_MOVE:
	{
		int dx = pos.x - prevPos.x;
		int dy = pos.y - prevPos.y;
		for( int i = 0; i <= BattleObject::actCount / anim[BattleCharBase::atachedAnimNo].imageDelay; i++ ){
			int tx = prevPos.x*( anim[ANIM_MOVE].imageNum - 1 ) + dx*i;
			int ty = prevPos.y*( anim[ANIM_MOVE].imageNum - 1 ) + dy*i;
			tx = BattleField::PANEL_SIZE.x*tx / ( anim[ANIM_MOVE].imageNum - 1 ) + BattleField::PANEL_SIZE.x / 2;
			ty = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*ty / ( anim[ANIM_MOVE].imageNum - 1 ) - 10;// �ӂ��̕������グ��
			ino = anim[ANIM_MOVE].image[anim[ANIM_MOVE].imageNum - 1 - i];
			DrawRotaGraph(tx, ty + drawPosOffset.y, 1, 0, ino, TRUE);
		}
	}
	break;
	case ANIM_ATK1:// �V���[�e�B���O�o�X�^�[
	{
		int n = BattleObject::actCount / anim[BattleCharBase::atachedAnimNo].imageDelay;
		if( n >= 9 )
			n = 7 + ( n % 2 );
		ino = anim[BattleCharBase::atachedAnimNo].image[n];
		DrawRotaGraph(x, y + drawPosOffset.y, 1, 0, ino, TRUE);
	}
	break;
	default:
		BattleCharBase::Draw();
		break;
	}
}

void Enemy_�t�H���e::Process(){
	EnemyBase::Process();
	// �f���[�g���[�V������(����s��)
	if( hp <= 0 ){
		return;
	}

	static const int WAIT_COUNT = 30;

	if( AnimProcess() ){
		// �s�����s���͗\������ׂď���
		if( !isMoveSuccess && !reserveAnim.empty() ){
			while( !reserveAnim.empty() )
				reserveAnim.pop();
			while( !moveTargetPos.empty() )
				moveTargetPos.pop();
			return;
		}

		if( !reserveAnim.empty() ){
			int anim = reserveAnim.front();
			reserveAnim.pop();
			SetAnim(anim);
			return;
		}

		count++;
		if( count%WAIT_COUNT == 0 ){
			if( nextMoveNum > 0 ){
				// �ړ�����
				nextMoveNum--;
				moveTargetPos.push(GetMoveTargetPos());
				prevPos = pos;
				SetAnim(ANIM_MOVE);
			} else{
				nextMoveNum = 3 + rand() % 3;
				// �U������(HP�̌������痐���ōU������)
				// �V���[�e�B���O�o�X�^�[(S),�w���Y���[�����O(H),�_�[�N�A�[���u���[�h(D1or3),�_�[�N�l�X�I�[�o�[���[�h(O)
				// HP: MAX�`1/2 -> D1(60%), D3(20%), H(10%), S(10%)
				// HP: 1/2�`1/4 -> D1(30%), D3(20%), H(20%), S(25%), O(5%)
				// HP: 1/4�`0   -> D1(10%), D3(25%), H(20%), S(35%), O(10%)
				int prob = rand() % 100;
				int half_hp = hpMax / 2;
				int quarter_hp = hpMax / 4;
				// debug(�U���X�P�W���[�����O�͖�����)
				if( hp>half_hp ){
					if( prob < 60 ){
						Attack_�_�[�N�A�[���u���[�h_1();
					} else if( prob < 90 ){
						Attack_�w���Y���[�����O();
					} else{
						Attack_�V���[�e�B���O�o�X�^�[();
					}
				} else if( hp>quarter_hp ){
					if( prob < 40 ){
						Attack_�_�[�N�A�[���u���[�h_1();
					} else if( prob < 70 ){
						Attack_�w���Y���[�����O();
					} else{
						Attack_�V���[�e�B���O�o�X�^�[();
					}
				} else{
					if( prob < 20 ){
						Attack_�_�[�N�A�[���u���[�h_1();
					} else if( prob < 60 ){
						Attack_�w���Y���[�����O();
					} else{
						Attack_�V���[�e�B���O�o�X�^�[();
					}
				}
			}
		}
	}
}

void Enemy_�t�H���e::LoadAnim(){
	static const std::string position = "Enemy_�t�H���e:LoadAnim";

	int temp[9 * 5];
	std::string fname = def::ENEMY_IMAGE_PATH + "�t�H���e.png";
	LoadDivGraphWithCheckError(temp, fname, position, 9, 5, 136, 172);
	for( int i = 0; i < 4; i++ )// �ŏ���4�͈ړ�
		anim[ANIM_MOVE].image[i] = temp[i];
	for( int i = 4; i < 9; i++ )
		DeleteGraph(temp[i]);
	for( int i = 0; i < 9; i++ )// ����9�̓V���[�e�B���O�o�X�^�[
		anim[ANIM_ATK1].image[i] = temp[i + 9];
	for( int i = 0; i < 9; i++ )// ����9�̓w���Y���[�����O
		anim[ANIM_ATK2].image[i] = temp[i + 18];
	for( int i = 9; i < 18; i++ )// �V���b�g����������
		anim[ANIM_ATK2].image[i] = anim[ANIM_ATK2].image[7 + ( i % 2 )];
	for( int i = 0; i < 3; i++ )// ����3�̓_�[�N�A�[���u���[�h(1 or 3��)
		anim[ANIM_ATK3].image[i] = temp[i + 27];
	for( int i = 3; i < 9; i++ )
		DeleteGraph(temp[i + 27]);
	for( int i = 0; i < 1; i++ )// �Ō��1�̓_���[�W
		anim[ANIM_DAMAGE].image[i] = temp[i + 36];
	for( int i = 1; i < 9; i++ )
		DeleteGraph(temp[i + 36]);
	// debug(�_�[�N�l�X�I�[�o�[���[�h�̍s���摜�̓w���Y���[�����O�ƈꏏ)

	anim[ANIM_MOVE].imageNum = 4;
	anim[ANIM_ATK1].imageNum = 9 + Act_�V���[�e�B���O�o�X�^�[::ATTACK_NUM * 2;
	anim[ANIM_ATK2].imageNum = 18;
	anim[ANIM_ATK3].imageNum = 3;
	anim[ANIM_DAMAGE].imageNum = 1;

	int t[10];
	fname = def::SKILL_IMAGE_PATH + "�w���Y���[�����O.png";
	LoadDivGraphWithCheckError(t, fname, position, 5, 2, 128, 164);
	for( int i = 0; i < 3; i++ )
		img�w���Y���[�����O[i] = t[i];
	for( int i = 3; i < 10; i++ )
		DeleteGraph(t[i]);
}

void Enemy_�t�H���e::DeleteAnim(){
	BattleObject::DeleteAnim();
	for( int i = 0; i < 3; i++ )
		DeleteGraph(img�w���Y���[�����O[i]);
}

CPoint<int> Enemy_�t�H���e::GetMoveTargetPos(){
	std::vector<CPoint<int>> move_targets;
	int t[BattleField::LAYER_MAX];
	for( int x = 0; x < BattleField::FIELD_NUM_X; x++ ){
		for( int y = 0; y < BattleField::FIELD_NUM_Y; y++ ){
			BattleFieldMgr::GetInst()->GetPanelInfo(t, x, y);
			// �ړ��\�Ȉʒu���v�b�V��
			if( t[BattleField::LAYER_USER] == BattleField::USER_ENEMY )
				if( !BattleCharMgr::GetInst()->IsAnyChar(CPoint<int>(x, y)) )
					move_targets.push_back(CPoint<int>(x, y));
		}
	}
	int i = rand() % move_targets.size();
	return move_targets[i];
}

void Enemy_�t�H���e::Attack_�V���[�e�B���O�o�X�^�[(){
	SetAnim(ANIM_ATK1);
}

void Enemy_�t�H���e::Attack_�w���Y���[�����O(){
	moveTargetPos.push(CPoint<int>(5, 1));// �ŉ����S�n�Ɉړ�
	reserveAnim.push(ANIM_MOVE);
	reserveAnim.push(ANIM_ATK2);
}

void Enemy_�t�H���e::Attack_�_�[�N�A�[���u���[�h_1(){
	// todo(�ړ��O�ɋA��ꏊ�̃t���O�𗧂Ă�)

	// �v���C���[�̐��ʂɈړ�
	CPoint<int> pos = BattleCharMgr::GetInst()->GetTargetPos(this->pos, def::eCHAR_PLAYER);
	pos.x++;
	moveTargetPos.push(pos);
	reserveAnim.push(ANIM_MOVE);

	// �U��
	reserveAnim.push(ANIM_ATK3);

	// ���̏ꏊ�ɋA��
	moveTargetPos.push(this->pos);
	reserveAnim.push(ANIM_MOVE);
}

void Enemy_�t�H���e::Attack_�_�[�N�A�[���u���[�h_3(){
	ASSERT(0,"Enemy_�t�H���e::Attack_�_�[�N�A�[���u���[�h_3 Unimplemented Skill");// todo
}

void Enemy_�t�H���e::Attack_�_�[�N�l�X�I�[�o�[���[�h(){
	ASSERT(0, "Enemy_�t�H���e::Attack_�_�[�N�l�X�I�[�o�[���[�h Unimplemented Skill");// todo
}
