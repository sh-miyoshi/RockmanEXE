#include "include.h"
#include "battleField.h"
#include "skill.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"

//-------------------------------------------------------
// �����֐�
//-------------------------------------------------------
bool TargetCondX(CPoint<int> mypos, CPoint<int> epos, bool isleft){
	if( isleft )
		return mypos.x > epos.x;
	else
		return mypos.x < epos.x;
}

//-------------------------------------------------------
// �N���X��`
//-------------------------------------------------------
class Skill_�o�X�^�[:public SkillData{
public:
	Skill_�o�X�^�[(CPoint<int> atkPos, int damage, def::charType myCharType);
	~Skill_�o�X�^�[();

	void Draw();
	void Process();
};

class Skill_�V���b�N�E�F�[�u:public SkillData{
	static const int IMAGE_DELAY = 4;// ���̉摜�Ɉڂ�܂ł̃J�E���g
	static const int NEXT_STEP_COUNT = 7 * IMAGE_DELAY;

	def::charType myCharType;
	int count;
	CPoint<int> atkPos;
	int image[7];
public:
	Skill_�V���b�N�E�F�[�u(CPoint<int> atkPos, int damage, def::charType myCharType);
	~Skill_�V���b�N�E�F�[�u();

	void Draw();
	void Process();
};

class Skill_�L���m��:public SkillData{
	static const int DAMAGE_ENTER_COUNT = 5 * 4;// �J�E���g��Ƀ_���[�W��o�^

	def::charType myCharType;
	int count;
	int imgBody[3];
	CPoint<int> atkPos;
public:
	enum{
		eTYPE_NORMAL,
		eTYPE_HIGH,
		eTYPE_MEGA,
	};

	Skill_�L���m��(CPoint<int> atkPos, int damage, def::charType myCharType, int type);
	~Skill_�L���m��();

	void Draw();
	void Process();
};

class Skill_�T���_�[�{�[��:public SkillData{
	static const int MAX_MOVE_CNT = 8;// �{�[�����ړ��ł���ő��
	static const int NEXT_STEP_COUNT = 80;
	static const int IMAGE_DELAY = 6;

	def::charType myCharType;
	int count;
	CPoint<int> atkPos;
	CPoint<float> startPos;
	int image[4];
	int moveDirect;
	int nextMoveDirect;
	CPoint<float> drawDiff;
	int moveCount;
public:
	Skill_�T���_�[�{�[��(CPoint<int> atkPos, int damage, def::charType myCharType);
	~Skill_�T���_�[�{�[��();

	void Draw();
	void Process();
};

class Skill_���J�o���[:public SkillData{
	static const int SKILL_DELAY = 2;// ���̉摜�Ɉڂ�܂ł̃J�E���g

	int count;
	CPoint<int> atkPos;
	int image[8];
public:
	Skill_���J�o���[(CPoint<int> atkPos, int recover);
	~Skill_���J�o���[();

	void Draw();
	void Process();
};

class Skill_�u�[������:public SkillData{
	static const int IMAGE_DELAY = 4;// ���̉摜�Ɉڂ�܂ł̃J�E���g
	static const int NEXT_STEP_COUNT = 2 * IMAGE_DELAY;

	def::charType myCharType;
	int count;
	int image[4];
	CPoint<int> atkPos;
	int attackType;
	int muki;// ����̎��̌���(UPorDOWN)
public:
	enum{
		eTYPE_����,
		eTYPE_����
	};

	Skill_�u�[������(CPoint<int> atkPos, int damage, def::charType myCharType, int type);
	~Skill_�u�[������();

	void Draw();
	void Process();
};

class Skill_�w���Y���[�����O:public SkillData{
	class CurveDirect{
		int directY;
		bool setFlag;
	public:
		CurveDirect():directY(0), setFlag(false){}
		void SetValue(int directY){
			this->directY = directY;
			setFlag = true;
		}
		bool IsSet()const{ return setFlag; }
		int GetAddValue(){
			if( directY == 0 )
				return 0;
			else
				return directY / abs(directY);
		}
	};

	static const int NEXT_STEP_COUNT = 9;
	static const int SKILL_DELAY = 3;// ���̉摜�Ɉڂ�܂ł̃J�E���g

	int count;
	CPoint<int> atkPos;
	CPoint<int> nextPos;
	CPoint<float> drawPos;
	int image[5];
	int moveDirectLR;
	CurveDirect curveDirect;// ��x�����Ȃ������
	def::charType damageTargetType;
	CPoint<int> targetPos;// �U���Ώۂ̈ʒu

						  // ���̕`��ʒu��Ԃ�
	CPoint<float> GetDrawPos(CPoint<float> drawPos, CPoint<int> now, CPoint<int> dist, int maxCnt, CPoint<int> ofs){
		float dx = ( ( float ) BattleField::PANEL_SIZE.x*( dist.x - now.x ) ) / maxCnt;
		drawPos.x += dx;
		float dy = ( ( float ) BattleField::PANEL_SIZE.y*( dist.y - now.y ) ) / maxCnt;
		drawPos.y += dy;
		return drawPos;
	}
public:
	Skill_�w���Y���[�����O(CPoint<int> atkPos, int damage, int directLR, int directUD, def::charType myCharType);
	~Skill_�w���Y���[�����O();

	void Draw();
	void Process();
};

class Skill_�_�[�N�A�[���u���[�h:public SkillData{
	static const int SKILL_DELAY = 1;// ���̉摜�Ɉڂ�܂ł̃J�E���g
	static const int STOP_NUM = 5;

	int count;
	CPoint<int> atkPos;
	int image[4 + STOP_NUM];
	bool isShort;
public:
	Skill_�_�[�N�A�[���u���[�h(CPoint<int> atkPos, int damage, bool isShort, def::charType myCharType);
	~Skill_�_�[�N�A�[���u���[�h();

	void Draw();
	void Process();
};

class Skill_�~�j�{��:public SkillData{
protected:
	static const unsigned BOMB_ROTATE_DELAY = 4;
	static const unsigned EXPLODE_DRAW_DLAY = 3;
	static const unsigned SKILL_COUNT = 40;// �{���𓊂��Ă��璅�e����܂ł̃J�E���g

	CPoint<int> initPos;
	CPoint<float> atkPos;
	bool isView;
	int count;
	int imgBomb[5];
	int imgExplode[16];
	int explodeCnt;
public:
	Skill_�~�j�{��(CPoint<int> atkPos, int damage, def::charType myCharType);
	~Skill_�~�j�{��();

	void Draw();
	void Process();
};

class Skill_�\�[�h_base:public SkillData{
protected:
	static const int SKILL_DELAY = 3;// ���̉摜�Ɉڂ�܂ł̃J�E���g

	CPoint<int> charPos;

	int count;
	int image[4];
public:
	Skill_�\�[�h_base(CPoint<int> charPos, int damage, def::charType myCharType);
	~Skill_�\�[�h_base();

	void Draw();
};

class Skill_�\�[�h:public Skill_�\�[�h_base{
public:
	Skill_�\�[�h(CPoint<int> charPos, int damage, def::charType myCharType);
	~Skill_�\�[�h();

	void Process();
};

class Skill_���C�h�\�[�h:public Skill_�\�[�h_base{
public:
	Skill_���C�h�\�[�h(CPoint<int> charPos, int damage, def::charType myCharType);
	~Skill_���C�h�\�[�h();

	void Process();
};

class Skill_�t���C�����C��:public SkillData{
protected:
	static const int SKILL_DELAY = 4;// ���̉摜�Ɉڂ�܂ł̃J�E���g
	static const int BODY_DRAW_DELAY = 2;

	std::vector<CPoint<int>> atkPoses;
	CPoint<int> charPos;

	int count;
	int img_body[4];
	int img_fire[5];
public:
	Skill_�t���C�����C��(CPoint<int> charPos, int damage, def::charType myCharType);
	~Skill_�t���C�����C��();

	void Draw();
};

class Skill_�t���C�����C��_�Œ�:public Skill_�t���C�����C��{
	static const int NEXT_STEP_COUNT = 20;
public:
	Skill_�t���C�����C��_�Œ�(CPoint<int> charPos, int damage, def::charType myCharType);
	~Skill_�t���C�����C��_�Œ�();

	void Process();
};

class Skill_�V���[�e�B���O�o�X�^�[:public SkillData{
	static const int SKILL_DELAY = 3;// ���̉摜�Ɉڂ�܂ł̃J�E���g

	int count;
	CPoint<int> atkPos;
	int image[5];
public:
	Skill_�V���[�e�B���O�o�X�^�[(CPoint<int> atkPos, int damage, def::charType myCharType);
	~Skill_�V���[�e�B���O�o�X�^�[();

	void Draw();
	void Process();
};

//-------------------------------------------------------
// �O���֐�
//-------------------------------------------------------
std::shared_ptr<SkillData> SkillMgr::GetData(int id, SkillArg args, def::charType myCharType){
	const std::string position = "SkillMgr::GetData ";

	switch( id ){
	case eID_�o�X�^�[:
		return std::shared_ptr<SkillData>(new Skill_�o�X�^�[(args.atkPos, args.damage, myCharType));
	case eID_�V���b�N�E�F�[�u:
		return std::shared_ptr<SkillData>(new Skill_�V���b�N�E�F�[�u(args.atkPos, args.damage, myCharType));
	case eID_�L���m��:
		return std::shared_ptr<SkillData>(new Skill_�L���m��(args.atkPos, args.damage, myCharType, Skill_�L���m��::eTYPE_NORMAL));
	case eID_�n�C�L���m��:
		return std::shared_ptr<SkillData>(new Skill_�L���m��(args.atkPos, args.damage, myCharType, Skill_�L���m��::eTYPE_HIGH));
	case eID_�T���_�[�{�[��:
		return std::shared_ptr<SkillData>(new Skill_�T���_�[�{�[��(args.atkPos, args.damage, myCharType));
	case eID_���J�o���[:
		return std::shared_ptr<SkillData>(new Skill_���J�o���[(args.atkPos, args.recover));
	case eID_�u�[������_����:
		return std::shared_ptr<SkillData>(new Skill_�u�[������(args.atkPos, args.damage, myCharType, Skill_�u�[������::eTYPE_����));
	case eID_�u�[������_����:
		return std::shared_ptr<SkillData>(new Skill_�u�[������(args.atkPos, args.damage, myCharType, Skill_�u�[������::eTYPE_����));
	case eID_�w���Y���[�����O_UP:
	{
		int dirLR = -1;
		if( myCharType == def::eCHAR_PLAYER )
			dirLR = def::eMUKI_RIGHT;
		else if( myCharType == def::eCHAR_ENEMY )
			dirLR = def::eMUKI_LEFT;
		else{
			std::string msg = position + "�w���Y���[�����O";
			msg += "�L�����^�C�v�����������ł� (" + ToString<int>(myCharType) + ")";
			ASSERT(0, msg);
		}
		return std::shared_ptr<SkillData>(new Skill_�w���Y���[�����O(args.atkPos, args.damage, dirLR, def::eMUKI_UP, myCharType));
	}
	case eID_�w���Y���[�����O_DOWN:
	{
		int dirLR = -1;
		if( myCharType == def::eCHAR_PLAYER )
			dirLR = def::eMUKI_RIGHT;
		else if( myCharType == def::eCHAR_ENEMY )
			dirLR = def::eMUKI_LEFT;
		else{
			std::string msg = position + "�w���Y���[�����O";
			msg += "�L�����^�C�v�����������ł� (" + ToString<int>(myCharType) + ")";
			ASSERT(0, msg);
		}
		return std::shared_ptr<SkillData>(new Skill_�w���Y���[�����O(args.atkPos, args.damage, dirLR, def::eMUKI_DOWN, myCharType));
	}
	case eID_�_�[�N�A�[���u���[�h_SHORT:
		return std::shared_ptr<SkillData>(new Skill_�_�[�N�A�[���u���[�h(args.atkPos, args.damage, true, myCharType));
	case eID_�~�j�{��:
		return std::shared_ptr<SkillData>(new Skill_�~�j�{��(args.atkPos, args.damage, myCharType));
	case eID_�\�[�h:
		return std::shared_ptr<SkillData>(new Skill_�\�[�h(args.atkPos, args.damage, myCharType));
	case eID_���C�h�\�[�h:
		return std::shared_ptr<SkillData>(new Skill_���C�h�\�[�h(args.atkPos, args.damage, myCharType));
	case eID_�V���[�e�B���O�o�X�^�[:
		return std::shared_ptr<SkillData>(new Skill_�V���[�e�B���O�o�X�^�[(args.atkPos, args.damage, myCharType));
	case eID_�t���C�����C��_�Œ�:
		return std::shared_ptr<SkillData>(new Skill_�t���C�����C��_�Œ�(args.atkPos, args.damage, myCharType));
	default:
		ASSERT(0, position + "���̂悤�ȃX�L���͎�������Ă��܂��� (" + ToString<int>(id) + ")");
	}
	return std::shared_ptr<SkillData>();
}

std::shared_ptr<SkillData> SkillMgr::GetData(ChipData c, SkillArg args, def::charType myCharType){
	int id = -1;
	args.damage = c.power;
	switch( c.id ){
	case ChipMgr::eID_�L���m��:
		id = eID_�L���m��;
		break;
	case ChipMgr::eID_�n�C�L���m��:
		id = eID_�n�C�L���m��;
		break;
	case ChipMgr::eID_�X�g�[���L���[�u:
		id = eID_�X�g�[���L���[�u;
		break;
	case ChipMgr::eID_�T���_�[�{�[��:
		id = eID_�T���_�[�{�[��;
		break;
	case ChipMgr::eID_�\�[�h:
		id = eID_�\�[�h;
		break;
	case ChipMgr::eID_�t���C�����C��:
		id = eID_�t���C�����C��_�Œ�;
		break;
	case ChipMgr::eID_���J�o���[10:
	case ChipMgr::eID_���J�o���[30:
		id = eID_���J�o���[;
		args.damage = 0;
		args.recover = c.power;
		break;
	case ChipMgr::eID_�u�[������:
		id = eID_�u�[������_����;
		args.atkPos = CPoint<int>(-1, BattleField::FIELD_NUM_Y - 1);// ��������U������悤��
		break;
	case ChipMgr::eID_�~�j�{��:
		id = eID_�~�j�{��;
		break;
	case ChipMgr::eID_���C�h�\�[�h:
		id = eID_���C�h�\�[�h;
		break;
	default:
		ASSERT(0, "SkillMgr::GetData �`�b�v�ƃX�L���̘A������������܂��� (" + ToString<int>(id) + ")");
	}
	return GetData(id, args, myCharType);
}

//-------------------------------------------------------
// �o�X�^�[
//-------------------------------------------------------
Skill_�o�X�^�[::Skill_�o�X�^�[(CPoint<int> atkPos, int damage, def::charType myCharType):SkillData(true, false, myCharType, damage){
	if( myCharType == def::eCHAR_PLAYER ){
		int xmax = BattleCharMgr::GetInst()->GetTargetX(atkPos.y, def::eCHAR_ENEMY | def::eCHAR_OBJECT);
		for( int x = atkPos.x + 1; x <= xmax; x++ )
			SkillData::damageList.push_back(DamageData(CPoint<int>(x, atkPos.y), damage, myCharType, false, damagePower, SkillData::id, false));
	} else if( myCharType == def::eCHAR_ENEMY ){
		int xmax = BattleCharMgr::GetInst()->GetTargetX(atkPos.y, def::eCHAR_PLAYER | def::eCHAR_OBJECT);
		if( xmax < 0 )
			xmax = 0;
		for( int x = atkPos.x - 1; x >= xmax; x-- )
			SkillData::damageList.push_back(DamageData(CPoint<int>(x, atkPos.y), damage, myCharType, false, damagePower, SkillData::id, false));
	} else
		ASSERT(0, "Skill_�o�X�^�[ missing char type (" + ToString<int>(myCharType) + ")");
}

Skill_�o�X�^�[::~Skill_�o�X�^�[(){
}

void Skill_�o�X�^�[::Draw(){
}

void Skill_�o�X�^�[::Process(){
}

//-------------------------------------------------------
// �V���b�N�E�F�[�u
//-------------------------------------------------------
Skill_�V���b�N�E�F�[�u::Skill_�V���b�N�E�F�[�u(CPoint<int> atkPos, int damage, def::charType myCharType)
	:count(0), myCharType(myCharType), SkillData(false, true, myCharType, damage){
	std::string fname = def::SKILL_IMAGE_PATH + "�V���b�N�E�F�[�u.png";
	LoadDivGraphWithCheckError(image, fname, "Skill_�V���b�N�E�F�[�u::Skill_�V���b�N�E�F�[�u", 7, 1, 100, 140);

	if( myCharType == def::eCHAR_PLAYER )
		this->atkPos.x = atkPos.x + 1;// �ڂ̑O�̈�}�X
	else
		this->atkPos.x = atkPos.x - 1;// �ڂ̑O�̈�}�X
	this->atkPos.y = atkPos.y;

	SkillData::damageList.push_back(DamageData(this->atkPos, damagePower, myCharType, true, 1, SkillData::id, false));
}

Skill_�V���b�N�E�F�[�u::~Skill_�V���b�N�E�F�[�u(){
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ )
		DeleteGraph(image[i]);
}

void Skill_�V���b�N�E�F�[�u::Draw(){
	int ino = count / IMAGE_DELAY;
	int x = BattleField::PANEL_SIZE.x*atkPos.x;
	int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*atkPos.y - 100;
	DrawGraph(x, y, image[ino], TRUE);
}

void Skill_�V���b�N�E�F�[�u::Process(){
	if( count >= NEXT_STEP_COUNT ){// ���J�E���g�����̃}�X��
		count = 0;
		atkPos.x += ( myCharType == def::eCHAR_PLAYER ) ? 1 : -1;
		// debug(���p�l��)
		if( atkPos.x < 0 || atkPos.x >= BattleField::FIELD_NUM_X ){// �U�����͈͊O�ɍs����
			delFlag = true;
			return;
		}

		// �U���̓o�^
		SkillData::damageList.push_back(DamageData(atkPos, damagePower, myCharType, true, NEXT_STEP_COUNT, SkillData::id, false));
	} else
		count++;
	BattleFieldMgr::GetInst()->AddDrawHitArea(atkPos);
}

//-------------------------------------------------------
// �L���m��
//-------------------------------------------------------
Skill_�L���m��::Skill_�L���m��(CPoint<int> atkPos, int damage, def::charType myCharType, int type)
	:count(0), myCharType(myCharType), atkPos(atkPos), SkillData(true, false, myCharType, damage){
	int temp[9];
	std::string fname = def::SKILL_IMAGE_PATH + "�L���m��_body.png";
	LoadDivGraphWithCheckError(temp, fname, "Skill_�L���m��::Skill_�L���m��", 3, 3, 46, 40);
	for( int i = 0, n = 0; i < 9; i++ ){
		if( ( i / 3 ) == type )
			imgBody[n++] = temp[i];
		else
			DeleteGraph(temp[i]);
	}

	// todo(�g���摜)
}

Skill_�L���m��::~Skill_�L���m��(){
	for( int i = 0; i < sizeof(imgBody) / sizeof(imgBody[0]); i++ )
		DeleteGraph(imgBody[i]);
}

void Skill_�L���m��::Draw(){
	int ino = ( count / 5 ) - 1;
	if( ino >= 0 ){
		if( ino > 2 )ino = 2;
		int dx = ( myCharType == def::eCHAR_PLAYER ) ? 65 : -10;
		int x = BattleField::PANEL_SIZE.x*atkPos.x + dx;
		int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*atkPos.y - 45;
		DrawGraph(x, y, imgBody[ino], TRUE);
	}
}

void Skill_�L���m��::Process(){
	if( count == DAMAGE_ENTER_COUNT ){
		// �o�^���ԂɂȂ�����_���[�W��o�^
		int target = def::eCHAR_ALL^myCharType;
		int x = BattleCharMgr::GetInst()->GetTargetX(atkPos.y, target);
		if( x > 0 )// ������ɓG������Ȃ�
			SkillData::damageList.push_back(DamageData(CPoint<int>(x, atkPos.y), damagePower, myCharType, true, 1, SkillData::id, false));
		delFlag = true;
	}
	count++;
}

//-------------------------------------------------------
// �T���_�[�{�[��
//-------------------------------------------------------
Skill_�T���_�[�{�[��::Skill_�T���_�[�{�[��(CPoint<int> atkPos, int damage, def::charType myCharType)
	:count(0), myCharType(myCharType), SkillData(false, false, myCharType, damage)
	, moveCount(0)
{
	std::string fname = def::SKILL_IMAGE_PATH + "�T���_�[�{�[��.png";
	LoadDivGraphWithCheckError(image, fname, "Skill_�T���_�[�{�[��::Skill_�T���_�[�{�[��", 4, 1, 64, 80);

	if( myCharType == def::eCHAR_PLAYER ){
		this->atkPos.x = atkPos.x + 1;// �ڂ̑O�̈�}�X
		moveDirect = def::eMUKI_RIGHT;
		startPos.x = -BattleField::PANEL_SIZE.x / 2.f;
	} else{
		this->atkPos.x = atkPos.x - 1;// �ڂ̑O�̈�}�X
		moveDirect = def::eMUKI_LEFT;
		startPos.x = BattleField::PANEL_SIZE.x / 2.f;
	}
	this->atkPos.y = atkPos.y;
	drawDiff = startPos;

	// �_���[�W���o�^
	SkillData::damageList.push_back(DamageData(this->atkPos, damagePower, myCharType, true, NEXT_STEP_COUNT, SkillData::id, false));
}

Skill_�T���_�[�{�[��::~Skill_�T���_�[�{�[��(){
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ )
		DeleteGraph(image[i]);
}

void Skill_�T���_�[�{�[��::Draw(){
	int ino = ( count / IMAGE_DELAY ) % 4;
	CPoint<int> ofs(5, -20);

	// ��ʒu
	int x = BattleField::PANEL_SIZE.x*atkPos.x + ofs.x;
	int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*atkPos.y + ofs.y;

	int direct = ( count <= NEXT_STEP_COUNT / 2 ) ? moveDirect : nextMoveDirect;
	switch( direct ){
	case def::eMUKI_RIGHT:	drawDiff.x += ( float ) BattleField::PANEL_SIZE.x / NEXT_STEP_COUNT;	break;
	case def::eMUKI_LEFT:	drawDiff.x -= ( float ) BattleField::PANEL_SIZE.x / NEXT_STEP_COUNT;	break;
	case def::eMUKI_DOWN:	drawDiff.y += ( float ) BattleField::PANEL_SIZE.y / NEXT_STEP_COUNT;	break;
	case def::eMUKI_UP:		drawDiff.y -= ( float ) BattleField::PANEL_SIZE.y / NEXT_STEP_COUNT;	break;
	}

	DrawGraph(x + ( int ) drawDiff.x, y + ( int ) drawDiff.y, image[ino], TRUE);
}

void Skill_�T���_�[�{�[��::Process(){
	if( count == NEXT_STEP_COUNT / 2 ){
		// ���ݒn�Ɉ�ԋ߂��G������
		def::charType target = def::eCHAR_ALL;
		if( myCharType == def::eCHAR_PLAYER )
			target = def::eCHAR_ENEMY;
		else if( myCharType == def::eCHAR_ENEMY )
			target = def::eCHAR_PLAYER;
		else// �v���C���[���G�L�����ȊO����T���_�[�{�[����������邱�Ƃ͍��̂Ƃ���Ȃ�
			ASSERT(0, "Skill_�T���_�[�{�[��::Process missing char type (" + ToString<int>(myCharType) + ")");
		CPoint<int> pos = BattleCharMgr::GetInst()->GetTargetPos(atkPos, target);

		// �ړ�����(�G��ǂ�������)
		int x_dif = pos.x - atkPos.x;// �v�Z�����厖(��������)
		int y_dif = pos.y - atkPos.y;// �v�Z�����厖(��������)
		if( abs(x_dif) > abs(y_dif) ){// x�����Ɉړ�
			if( x_dif > 0 ){
				nextMoveDirect = def::eMUKI_RIGHT;
				startPos = CPoint<float>(-BattleField::PANEL_SIZE.x / 2.f, 0);
			} else{
				nextMoveDirect = def::eMUKI_LEFT;
				startPos = CPoint<float>(+BattleField::PANEL_SIZE.x / 2.f, 0);
			}

		} else{// y�����Ɉړ�
			if( y_dif > 0 ){
				nextMoveDirect = def::eMUKI_DOWN;
				startPos = CPoint<float>(0, -BattleField::PANEL_SIZE.y / 2.f);
			} else{
				nextMoveDirect = def::eMUKI_UP;
				startPos = CPoint<float>(0, +BattleField::PANEL_SIZE.y / 2.f);
			}
		}
	} else if( count == NEXT_STEP_COUNT ){
		count = 0;

		drawDiff = startPos;
		moveDirect = nextMoveDirect;

		switch( nextMoveDirect ){
		case def::eMUKI_RIGHT:	atkPos.x++;	break;
		case def::eMUKI_LEFT:	atkPos.x--;	break;
		case def::eMUKI_DOWN:	atkPos.y++;	break;
		case def::eMUKI_UP:		atkPos.y--;	break;
		}
		// debug(���p�l��)

		if( atkPos.x < 0 || atkPos.x >= BattleField::FIELD_NUM_X ||
			atkPos.y < 0 || atkPos.y >= BattleField::FIELD_NUM_Y ||
			moveCount >= MAX_MOVE_CNT )
		{// �U�����͈͊O�ɍs���� or �ő�ړ��񐔂ɒB������
			delFlag = true;
			return;
		}

		// �_���[�W���o�^
		SkillData::damageList.push_back(DamageData(atkPos, damagePower, myCharType, true, NEXT_STEP_COUNT, SkillData::id, false));
		moveCount++;
	}
	count++;
	BattleFieldMgr::GetInst()->AddDrawHitArea(atkPos);
}

//-------------------------------------------------------
// ���J�o���[
//-------------------------------------------------------
Skill_���J�o���[::Skill_���J�o���[(CPoint<int> atkPos, int recover)
	:count(0), atkPos(atkPos), SkillData(false, false, def::eCHAR_ALL, -recover)
{
	std::string fname = def::SKILL_IMAGE_PATH + "���J�o���[.png";
	LoadDivGraphWithCheckError(image, fname, "Skill_���J�o���[::Skill_���J�o���[", 8, 1, 84, 144);
}

Skill_���J�o���[::~Skill_���J�o���[(){
	for( int i = 0; i < 8; i++ )
		DeleteGraph(image[i]);
}

void Skill_���J�o���[::Draw(){
	int ino = ( count / SKILL_DELAY );
	if( ino > 7 )
		ino = 7;
	int x = BattleField::PANEL_SIZE.x*atkPos.x;
	int y = BattleField::PANEL_SIZE.y*atkPos.y + 25;
	DrawGraph(x, y, image[ino], TRUE);
}

void Skill_���J�o���[::Process(){
	count++;
	if( count >= SKILL_DELAY * 8 ){
		// �����̏ꏊ�̃L������HP���񕜂�����
		SkillData::damageList.push_back(DamageData(atkPos, damagePower, def::eCHAR_ALL, false, 1, SkillData::id, true));
		delFlag = true;
	}
}

//-------------------------------------------------------
// �u�[������
//-------------------------------------------------------
Skill_�u�[������::Skill_�u�[������(CPoint<int> atkPos, int damage, def::charType myCharType, int type)
	:count(0), myCharType(myCharType), attackType(type), SkillData(false, true, myCharType, damage){
	std::string fname = def::SKILL_IMAGE_PATH + "�u�[������.png";
	LoadDivGraphWithCheckError(image, fname, "Skill_�u�[������::Skill_�u�[������", 4, 1, 100, 80);

	if( myCharType == def::eCHAR_PLAYER )
		this->atkPos.x = atkPos.x + 1;// �ڂ̑O�̈�}�X
	else
		this->atkPos.x = atkPos.x - 1;// �ڂ̑O�̈�}�X
	this->atkPos.y = atkPos.y;
	if( atkPos.y == 0 )
		muki = def::eMUKI_DOWN;
	else if( atkPos.y == 2 )
		muki = def::eMUKI_UP;

	SkillData::damageList.push_back(DamageData(this->atkPos, damagePower, myCharType, true, 1, SkillData::id, false));
}

Skill_�u�[������::~Skill_�u�[������(){
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ )
		DeleteGraph(image[i]);
}

void Skill_�u�[������::Draw(){
	int ino = ( count % 16 ) / 4;
	CPoint<int> ofs(-10, -20);
	int x = BattleField::PANEL_SIZE.x*atkPos.x + ofs.x;// ���S
	int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*atkPos.y + ofs.y;
	int xd = 0, yd = 0;
	switch( attackType ){
	case eTYPE_����:
		xd = ( int ) ( ( double ) ( count%NEXT_STEP_COUNT )*BattleField::PANEL_SIZE.x / NEXT_STEP_COUNT ) - 50;
		if( myCharType != def::eCHAR_PLAYER )
			xd *= -1;
		break;
	case eTYPE_����:
		if( ( ( myCharType == def::eCHAR_PLAYER ) && ( atkPos.x == BattleField::FIELD_NUM_X - 1 ) ) || ( ( myCharType != def::eCHAR_PLAYER ) && ( atkPos.x == 0 ) ) ){// �c�����ɉ�]
			yd = ( int ) ( ( double ) ( count%NEXT_STEP_COUNT )*BattleField::PANEL_SIZE.y / ( NEXT_STEP_COUNT * 2 ) );
			if( muki == def::eMUKI_UP )
				yd *= -1;
		} else{
			xd = ( int ) ( ( double ) ( count%NEXT_STEP_COUNT )*BattleField::PANEL_SIZE.x / NEXT_STEP_COUNT ) - 50;
			if( ( muki == def::eMUKI_UP&&atkPos.y == 0 ) || ( muki == def::eMUKI_DOWN&&atkPos.y == BattleField::FIELD_NUM_Y - 1 ) )// �A�蓹�Ȃ�
				xd *= -1;
			if( myCharType != def::eCHAR_PLAYER )
				xd *= -1;
		}
		break;
	}
	DrawGraph(x + xd, y + yd, image[ino], TRUE);
}

void Skill_�u�[������::Process(){
	if( count%NEXT_STEP_COUNT == NEXT_STEP_COUNT - 1 ){// ���J�E���g�����̃}�X��
		switch( attackType ){
		case eTYPE_����:
			atkPos.x += ( myCharType == def::eCHAR_PLAYER ) ? 1 : -1;
			break;
		case eTYPE_����:
			ASSERT(muki == def::eMUKI_UP || muki == def::eMUKI_DOWN, "Skill_�u�[������::Process wrong direction (" + ToString<int>(muki) + ")");
			if( 1 <= atkPos.x&&atkPos.x < BattleField::FIELD_NUM_X - 1 ){
				int val = ( atkPos.y == 0 ) ? -1 : 1;
				if( muki == def::eMUKI_DOWN )// ���Ɍ������Ă������Ȃ�
					val *= -1;
				if( myCharType != def::eCHAR_PLAYER )// �v���C���[����Ȃ���΋t����
					val *= -1;
				atkPos.x += val;
			} else{
				if( ( ( myCharType == def::eCHAR_PLAYER ) && ( atkPos.x == 0 ) ) || ( ( myCharType != def::eCHAR_PLAYER ) && ( atkPos.x == BattleField::FIELD_NUM_X - 1 ) ) ){// �I���ʒu�Ȃ�
																																											  // UP&0 -> -1 UP&2 -> 1 DOWN&0 1 DOWN&2 -1
					int val = ( atkPos.y == 0 ) ? -1 : 1;
					if( muki == def::eMUKI_DOWN )
						val *= -1;
					if( myCharType != def::eCHAR_PLAYER )
						val *= -1;
					atkPos.x += val;
				} else{
					if( atkPos.y == 0 ){
						if( muki == def::eMUKI_DOWN )
							atkPos.y++;
						else
							atkPos.x += ( myCharType == def::eCHAR_PLAYER ) ? -1 : 1;
					} else if( atkPos.y == BattleField::FIELD_NUM_Y - 1 ){
						if( muki == def::eMUKI_UP )
							atkPos.y--;
						else
							atkPos.x += ( myCharType == def::eCHAR_PLAYER ) ? -1 : 1;
					} else// �c�����̒��Ԓn�_
						atkPos.y += ( muki == def::eMUKI_UP ) ? -1 : 1;
				}
			}
			break;
		}

		if( atkPos.x < 0 || atkPos.x >= BattleField::FIELD_NUM_X ){// �U�����͈͊O�ɍs����
			delFlag = true;
			return;
		}

		SkillData::damageList.push_back(DamageData(atkPos, damagePower, myCharType, true, 1, SkillData::id, false));
	}
	count++;
}

//-------------------------------------------------------
// �w���Y���[�����O
//-------------------------------------------------------
Skill_�w���Y���[�����O::Skill_�w���Y���[�����O(CPoint<int> atkPos, int damage, int directLR, int directUD, def::charType myCharType)
	:count(0), SkillData(false, true, myCharType, damage), moveDirectLR(directLR), atkPos(atkPos)
{
	std::string msg = "Skill_�w���Y���[�����O wrong direction ";
	ASSERT(directLR == def::eMUKI_RIGHT || directLR == def::eMUKI_LEFT, msg + "directLR = " + ToString<int>(directLR));
	ASSERT(directUD == def::eMUKI_UP || directUD == def::eMUKI_DOWN, msg + "directUD = " + ToString<int>(directUD));

	int t[10];
	std::string fname = def::SKILL_IMAGE_PATH + "�w���Y���[�����O.png";
	LoadDivGraphWithCheckError(t, fname, "Skill_�w���Y���[�����O::Skill_�w���Y���[�����O", 5, 2, 128, 164);
	for( int i = 0; i < 5; i++ ){
		image[4 - i] = t[5 + i];
		DeleteGraph(t[i]);
	}

	if( myCharType == def::eCHAR_ENEMY )
		damageTargetType = def::eCHAR_PLAYER;
	else if( myCharType == def::eCHAR_PLAYER )
		damageTargetType = def::eCHAR_ENEMY;
	else{
		ASSERT(0, "Skill_�w���Y���[�����O missing char type (" + ToString<int>(myCharType) + ")");
	}

	// �ŏ��͎����̃}�X�A���͎΂� ��or��
	nextPos.x = ( moveDirectLR == def::eMUKI_RIGHT ) ? atkPos.x + 1 : atkPos.x - 1;
	nextPos.y = ( directUD == def::eMUKI_DOWN ) ? atkPos.y + 1 : atkPos.y - 1;

	drawPos.x = ( float ) BattleField::PANEL_SIZE.x*atkPos.x - 20;
	drawPos.y = ( float ) BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*atkPos.y - 90;

	static const int max = BattleField::FIELD_NUM_Y*( BattleField::FIELD_NUM_X / 2 );
	CPoint<int> temp[max];
	bool isleft = ( myCharType == def::eCHAR_ENEMY );
	std::function<bool(CPoint<int>)> cond = std::bind(TargetCondX, atkPos, std::placeholders::_1, isleft);
	int n = BattleCharMgr::GetInst()->GetCondPos(temp, max, damageTargetType, cond);
	int x = BattleField::FIELD_NUM_X;
	for( int i = 0; i < n; i++ ){
		if( temp[i].x < x ){
			if( abs(temp[i].x - atkPos.x) == 1 && abs(temp[i].y - atkPos.y) == 2 )
				continue;
			x = temp[i].x;
			targetPos = temp[i];
		}
	}
}

Skill_�w���Y���[�����O::~Skill_�w���Y���[�����O(){
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ )
		DeleteGraph(image[i]);
}

void Skill_�w���Y���[�����O::Draw(){
	int ino = ( count / SKILL_DELAY );
	if( ino > 4 )
		ino = 3 + ( ino / 3 ) % 2;

	drawPos = GetDrawPos(drawPos, atkPos, nextPos, NEXT_STEP_COUNT, CPoint<int>(-20, -90));
	if( moveDirectLR == def::eMUKI_RIGHT )
		DrawTurnGraphF(drawPos.x, drawPos.y, image[ino], TRUE);
	else
		DrawGraphF(drawPos.x, drawPos.y, image[ino], TRUE);

	// debug(�e������)
}

void Skill_�w���Y���[�����O::Process(){
	count++;
	if( count%NEXT_STEP_COUNT == 0 ){
		// ��{�܂������A��x�����G�����֋Ȃ���
		atkPos = nextPos;
		nextPos.x += ( moveDirectLR == def::eMUKI_RIGHT ) ? +1 : -1;
		if( !curveDirect.IsSet() ){
			int x = abs(targetPos.x - atkPos.x);
			int y = abs(targetPos.y - atkPos.y);
			if( ( x == 1 && y == 1 ) || ( x == 2 && y == 2 ) )
				curveDirect.SetValue(targetPos.y - atkPos.y);
			nextPos.y += curveDirect.GetAddValue();
		} else
			nextPos.y += curveDirect.GetAddValue();// �㉺�ɋȂ��葱����

		if( atkPos.x < 0 || atkPos.x>5 || atkPos.y < 0 || atkPos.y>2 )
			delFlag = true;

		if( !delFlag ){
			// �_���[�W���o�^
			SkillData::damageList.push_back(DamageData(atkPos, damagePower, myCharType, true, NEXT_STEP_COUNT, SkillData::id, true));
		}
	}
}

//-------------------------------------------------------
// �_�[�N�A�[���u���[�h
//-------------------------------------------------------
Skill_�_�[�N�A�[���u���[�h::Skill_�_�[�N�A�[���u���[�h(CPoint<int> atkPos, int damage, bool isShort, def::charType myCharType)
	:count(0), atkPos(atkPos), isShort(isShort), SkillData(true, true, myCharType, damage)
{
	int tmp[8];
	std::string fname = def::SKILL_IMAGE_PATH + "�_�[�N�A�[���u���[�h.png";
	LoadDivGraphWithCheckError(tmp, fname, "Skill_�_�[�N�A�[���u���[�h::Skill_�_�[�N�A�[���u���[�h", 4, 2, 188, 150);

	int index = isShort ? 3 : 7;
	for( int i = 0; i < 4; i++ ){
		image[i] = tmp[index - i];
		int ti = isShort ? 4 : 0;
		DeleteGraph(tmp[i + ti]);
	}
	for( int i = 0; i < STOP_NUM; i++ ){
		image[i + 4] = image[3];
	}
}

Skill_�_�[�N�A�[���u���[�h::~Skill_�_�[�N�A�[���u���[�h(){
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ ){
		DeleteGraph(image[i]);
		image[i] = -1;
	}
}

void Skill_�_�[�N�A�[���u���[�h::Draw(){
	int ino = ( count / SKILL_DELAY );
	if( ino > 3 + STOP_NUM )
		ino = 3 + STOP_NUM;
	CPoint<int> ofs;
	ofs.x = isShort ? -20 : -70;
	ofs.y = 70;
	int x = BattleField::PANEL_SIZE.x*atkPos.x + ofs.x;
	int y = BattleField::PANEL_SIZE.y*atkPos.y + ofs.y;
	DrawGraph(x, y, image[ino], TRUE);
}

void Skill_�_�[�N�A�[���u���[�h::Process(){
	count++;
	if( count == SKILL_DELAY * 1 ){
		// �_���[�W�̓o�^
		SkillData::damageList.push_back(DamageData(atkPos, damagePower, myCharType, true, 1, SkillData::id, true));
	}
	if( count >= SKILL_DELAY * ( 3 + STOP_NUM ) )
		delFlag = true;
}

//-------------------------------------------------------
// �~�j�{��
//-------------------------------------------------------
Skill_�~�j�{��::Skill_�~�j�{��(CPoint<int> atkPos, int damage, def::charType myCharType)
	:SkillData(true, false, myCharType, damage), count(0), initPos(atkPos)
	, isView(false), explodeCnt(0)
{
	ASSERT(myCharType == def::eCHAR_PLAYER, "Skill_�~�j�{�� missing char type (" + ToString<int>(myCharType) + ")");

	std::string fname = def::SKILL_IMAGE_PATH + "�~�j�{��.png";
	LoadDivGraphWithCheckError(imgBomb, fname, "Skill_�~�j�{��", 5, 1, 40, 30);

	fname = def::IMAGE_FILE_PATH + "explode.png";
	LoadDivGraphWithCheckError(imgExplode, fname, "Skill_�~�j�{��", 8, 2, 110, 124);
}

Skill_�~�j�{��::~Skill_�~�j�{��(){
	for( int i = 0; i < 5; i++ )
		DeleteGraph(imgBomb[i]);
	for( int i = 0; i < 16; i++ )
		DeleteGraph(imgExplode[i]);
}

void Skill_�~�j�{��::Draw(){
	if( isView ){
		int ino = ( count / BOMB_ROTATE_DELAY ) % 4;
		int x = BattleField::PANEL_SIZE.x*initPos.x + 20 + ( int ) atkPos.x;
		int y = BattleField::PANEL_SIZE.y*initPos.y + 170 + ( int ) atkPos.y;
		DrawGraph(x, y, imgBomb[ino], TRUE);
	}

	// ���������Ƀ{������
	if( explodeCnt > 0 ){
		int ino = ( explodeCnt / EXPLODE_DRAW_DLAY ) * 2;
		if( ino > 15 )
			ino = 15;
		int x = BattleField::PANEL_SIZE.x*( initPos.x + 3 ) - 10;
		int y = BattleField::PANEL_SIZE.y*initPos.y + 90;
		DrawGraph(x, y, imgExplode[ino], TRUE);
	}
}

void Skill_�~�j�{��::Process(){
	count++;

	if( explodeCnt == 0 ){
		// �������̕`��
		atkPos.x += ( BattleField::PANEL_SIZE.x*3.f / SKILL_COUNT );
		if( atkPos.x > BattleField::PANEL_SIZE.x / 2 )
			isView = true;
		if( atkPos.x > BattleField::PANEL_SIZE.x*3.f ){
			isView = false;
			explodeCnt = 1;
			// todo(�s���̎���delFlag=true)

			// �_���[�W�̓o�^
			CPoint<int> pos = initPos;
			pos.x += 3;// todo(charType�ɂ����+-�𔽓])
			SkillData::damageList.push_back(DamageData(pos, damagePower, myCharType, true, 1, SkillData::id, false));
		}
		atkPos.y = atkPos.x*( ( atkPos.x / 120 ) - 2 );// y = x^2/120 - 2x
	} else{
		explodeCnt++;
		if( explodeCnt > EXPLODE_DRAW_DLAY * 15 / 2 )
			delFlag = true;
	}
}

Skill_�\�[�h_base::Skill_�\�[�h_base(CPoint<int> charPos, int damage, def::charType myCharType)
	:count(0), charPos(charPos), SkillData(true, true, myCharType, damage)
{
	for( int i = 0; i < 4; i++ )
		image[i] = -1;
}

Skill_�\�[�h_base::~Skill_�\�[�h_base(){
	for( int i = 0; i < 4; i++ )
		DeleteGraph(image[i]);
}

void Skill_�\�[�h_base::Draw(){
	int ino = ( count / SKILL_DELAY );
	if( ino > 4 )
		ino = 4;
	int x = BattleField::PANEL_SIZE.x*charPos.x + 40;
	int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*charPos.y - 80;
	if( image[ino] != -1 ){
		ASSERT(myCharType == def::eCHAR_PLAYER, "Skill_�\�[�h_base::Draw missing char type (" + ToString<int>(myCharType) + ")");
		DrawGraph(x, y, image[ino], TRUE);
	}
}

//-------------------------------------------------------
// �\�[�h
//-------------------------------------------------------
Skill_�\�[�h::Skill_�\�[�h(CPoint<int> charPos, int damage, def::charType myCharType)
	:Skill_�\�[�h_base(charPos, damage, myCharType)
{
	std::string fname = def::SKILL_IMAGE_PATH + "�\�[�h.png";
	int t[12];
	LoadDivGraphWithCheckError(t, fname, "Skill_�\�[�h", 4, 3, 160, 150);
	for( int i = 0; i < 4; i++ ){
		image[i] = t[i];
		DeleteGraph(t[i + 4]);
		DeleteGraph(t[i + 8]);
	}
}

Skill_�\�[�h::~Skill_�\�[�h(){
}

void Skill_�\�[�h::Process(){
	count++;
	if( count == SKILL_DELAY * 1 ){
		// �ڂ̑O�̃}�X�Ƀ_���[�W�̓o�^
		CPoint<int> pos = charPos;
		if( myCharType == def::eCHAR_PLAYER )
			pos.x += 1;
		else if( myCharType == def::eCHAR_ENEMY )
			pos.x--;
		else
			AppLogger::Error("Skill_�\�[�h: �\�[�h���g�p����L�����^�C�v�����������ł� charType = " + ToString<int>(myCharType));
		SkillData::damageList.push_back(DamageData(pos, damagePower, myCharType, true, 1, SkillData::id, true));
	}
	if( count >= SKILL_DELAY * 4 )
		delFlag = true;
}

//-------------------------------------------------------
// ���C�h�\�[�h
//-------------------------------------------------------
Skill_���C�h�\�[�h::Skill_���C�h�\�[�h(CPoint<int> charPos, int damage, def::charType myCharType)
	:Skill_�\�[�h_base(charPos, damage, myCharType)
{
	std::string fname = def::SKILL_IMAGE_PATH + "�\�[�h.png";
	int t[12];
	LoadDivGraphWithCheckError(t, fname, "Skill_�\�[�h", 4, 3, 160, 150);
	for( int i = 0; i < 4; i++ ){
		image[i] = t[i + 8];
		DeleteGraph(t[i]);
		DeleteGraph(t[i + 4]);
	}
}

Skill_���C�h�\�[�h::~Skill_���C�h�\�[�h(){
}

void Skill_���C�h�\�[�h::Process(){
	count++;
	if( count == SKILL_DELAY * 1 ){
		// �ڂ̑O��3�}�X�Ƀ_���[�W�̓o�^
		CPoint<int> pos = charPos;
		if( myCharType == def::eCHAR_PLAYER )
			pos.x += 1;
		else if( myCharType == def::eCHAR_ENEMY )
			pos.x--;
		else
			AppLogger::Error("Skill_�\�[�h: �\�[�h���g�p����L�����^�C�v�����������ł� charType = " + ToString<int>(myCharType));
		SkillData::damageList.push_back(DamageData(pos, damagePower, myCharType, true, 1, SkillData::id, true));
		int y = pos.y;
		if( y > 0 ){
			pos.y = y - 1;
			SkillData::damageList.push_back(DamageData(pos, damagePower, myCharType, true, 1, SkillData::id, true));
		}
		if( y < BattleField::FIELD_NUM_Y - 1 ){
			pos.y = y + 1;
			SkillData::damageList.push_back(DamageData(pos, damagePower, myCharType, true, 1, SkillData::id, true));
		}
	}
	if( count >= SKILL_DELAY * 4 )
		delFlag = true;
}

Skill_�V���[�e�B���O�o�X�^�[::Skill_�V���[�e�B���O�o�X�^�[(CPoint<int> atkPos, int damage, def::charType myCharType)
	:SkillData(false, true, myCharType, damage), count(0), atkPos(atkPos)
{
	std::string fname = def::SKILL_IMAGE_PATH + "�V���[�e�B���O�o�X�^�[.png";
	LoadDivGraphWithCheckError(image, fname, "Skill_�V���[�e�B���O�o�X�^�[", 5, 1, 112, 96);
}

Skill_�V���[�e�B���O�o�X�^�[::~Skill_�V���[�e�B���O�o�X�^�[(){
	for( int i = 0; i < 5; i++ )
		DeleteGraph(image[i]);
}

void Skill_�V���[�e�B���O�o�X�^�[::Draw(){
	int ino = ( count / SKILL_DELAY );
	if( ino > 4 )
		ino = 4;
	int x = BattleField::PANEL_SIZE.x*atkPos.x - 15;
	int y = BattleField::PANEL_SIZE.y*atkPos.y + 100;
	DrawGraph(x, y, image[ino], TRUE);
}

void Skill_�V���[�e�B���O�o�X�^�[::Process(){
	count++;
	if( count == SKILL_DELAY * 1 ){
		SkillData::damageList.push_back(DamageData(atkPos, damagePower, myCharType, true, 1, SkillData::id, true));
	}

	if( count >= SKILL_DELAY * 5 )
		delFlag = true;
	BattleFieldMgr::GetInst()->AddDrawHitArea(atkPos);
}

Skill_�t���C�����C��::Skill_�t���C�����C��(CPoint<int> charPos, int damage, def::charType myCharType)
	:count(0), SkillData(false, true, myCharType, damage), charPos(charPos)
{
	std::string fname = def::SKILL_IMAGE_PATH + "�t���C�����C��_�Β�.png";
	LoadDivGraphWithCheckError(img_fire, fname, "Skill_�t���C�����C��", 5, 1, 72, 120);
	if( myCharType == def::eCHAR_PLAYER ){// �v���C���[�Ȃ�body���`��
		// �摜�ǂݍ���
		fname = def::SKILL_IMAGE_PATH + "�t���C�����C��_body.png";
		LoadDivGraphWithCheckError(img_body, fname, "Skill_�t���C�����C��", 4, 1, 64, 64);
	} else{
		for( int i = 0; i < 4; i++ )
			img_body[i] = -1;
	}
}

Skill_�t���C�����C��::~Skill_�t���C�����C��(){
	for( int i = 0; i < 4; i++ )
		DeleteGraph(img_body[i]);
	for( int i = 0; i < 5; i++ )
		DeleteGraph(img_fire[i]);
}

void Skill_�t���C�����C��::Draw(){
	int ino = ( count / SKILL_DELAY );
	if( ino > 4 )
		ino = 4;
	for( std::vector<CPoint<int>>::iterator it = atkPoses.begin(); it != atkPoses.end(); it++ ){
		int x = BattleField::PANEL_SIZE.x*it->x + 7;
		int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*it->y -70;
		DrawGraph(x, y, img_fire[ino], TRUE);
	}

	// body��`�悷��Ȃ�
	if( img_body[0] != -1 ){
		ino = ( count / BODY_DRAW_DELAY );
		if( ino > 3 )
			ino = 3;
		int x = BattleField::PANEL_SIZE.x*charPos.x + 60;
		int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*charPos.y -60;
		if( myCharType == def::eCHAR_PLAYER ){
			// �U���{�f�B�̕`��
			DrawGraph(x, y, img_body[ino], TRUE);
		} else{
			// todo(�G�̍U�����̃I�t�Z�b�g�����߂�)
			ASSERT(0);// ������
		}
	}
}

Skill_�t���C�����C��_�Œ�::Skill_�t���C�����C��_�Œ�(CPoint<int> charPos, int damage, def::charType myCharType)
	:Skill_�t���C�����C��(charPos,damage,myCharType)
{
	CPoint<int> tp;
	// �o�^�̏���(�������O��)
	tp.x = ( myCharType == def::eCHAR_PLAYER ) ? charPos.x + 2 : charPos.x - 2;
	if( charPos.y > 0 ){
		tp.y = charPos.y - 1;
		atkPoses.push_back(tp);
	}
	tp.y = charPos.y;
	atkPoses.push_back(tp);
	if( charPos.y < BattleField::FIELD_NUM_Y - 1 ){
		tp.y = charPos.y + 1;
		atkPoses.push_back(tp);
	}
}

Skill_�t���C�����C��_�Œ�::~Skill_�t���C�����C��_�Œ�(){
}

void Skill_�t���C�����C��_�Œ�::Process(){
	count++;
	if( count >= 40 )// debug
		delFlag = true;
}
