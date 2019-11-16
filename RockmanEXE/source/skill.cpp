#include "include.h"
#include "skill.h"
#include "battleCharMgr.h"
#include "battleField.h"
#include "effectMgr.h"

// TODO(�G�Ƀq�b�g���̃G�t�F�N�g)

//-------------------------------------------------------
// �N���X��`
//-------------------------------------------------------
class Skill_�o�X�^�[:public SkillData {
public:
	Skill_�o�X�^�[(CPoint<int> charPos, int damage, CharType myCharType);
	~Skill_�o�X�^�[();

	void Draw();
	bool Process();
};

class Skill_�L���m���n:public SkillData {
	static const int DAMAGE_REGISTER_COUNT = 20;
	static const int SKILL_END_COUNT = 28;

	CharType myCharType;
	int count;
	int imgBody[3];
	CPoint<int> charPos;
	int damage;
public:
	enum Type {
		eTYPE_NORMAL,
		eTYPE_HIGH,
		eTYPE_MEGA,
	};

	Skill_�L���m���n(CPoint<int> charPos, int damage, CharType myCharType, Type skillType);
	~Skill_�L���m���n();

	void Draw();
	bool Process();
};

class Skill_�V���b�N�E�F�[�u:public SkillData {
	static const int IMAGE_DELAY = 4;// ���̉摜�Ɉڂ�܂ł̃J�E���g
	static const int NEXT_STEP_COUNT = 7 * IMAGE_DELAY;

	CharType myCharType;
	int count;
	CPoint<int> atkPos;
	int image[7];
	int damage;
public:
	Skill_�V���b�N�E�F�[�u(CPoint<int> charPos, int damage, CharType myCharType);
	~Skill_�V���b�N�E�F�[�u();

	void Draw();
	bool Process();
};

class Skill_�T���_�[�{�[��:public SkillData {
	static const int NEXT_STEP_COUNT = 80;
	static const int IMAGE_DELAY = 6;

	CharType myCharType;
	int count;
	CPoint<int> atkPos;
	CPoint<float> startPos, drawOfs;
	def::Muki moveDirect, nextMoveDirect;
	int image[4];
	int damage;
	unsigned int ariveTime;
public:
	Skill_�T���_�[�{�[��(CPoint<int> charPos, int damage, CharType myCharType, unsigned int ariveTime);
	~Skill_�T���_�[�{�[��();

	void Draw();
	bool Process();
};

class Skill_�\�[�h�n:public SkillData {
	static const int SKILL_DELAY = 3;// ���̉摜�Ɉڂ�܂ł̃J�E���g

	unsigned int count;
	int damage;
	int image[4];
	CharType myCharType;
	std::vector<CPoint<int>> atkPoses;
public:
	enum Type {
		eTYPE_�\�[�h,
		eTYPE_���C�h�\�[�h,
	};

	Skill_�\�[�h�n(Type type, CPoint<int> charPos, int damage, CharType myCharType);
	~Skill_�\�[�h�n();

	void Draw();
	bool Process();
};

//-------------------------------------------------------
// �O���[�o�����\�b�h
//-------------------------------------------------------
std::shared_ptr<SkillData> SkillMgr::GetData(int id, SkillArg args) {
	switch( id ) {
	case eID_�o�X�^�[:
		return std::shared_ptr<Skill_�o�X�^�[>(new Skill_�o�X�^�[(args.charPos, args.power, args.myCharType));
	case eID_�L���m��:
		return std::shared_ptr<Skill_�L���m���n>(new Skill_�L���m���n(args.charPos, args.power, args.myCharType, Skill_�L���m���n::eTYPE_NORMAL));
	case eID_�n�C�L���m��:
		return std::shared_ptr<Skill_�L���m���n>(new Skill_�L���m���n(args.charPos, args.power, args.myCharType, Skill_�L���m���n::eTYPE_HIGH));
	case eID_�V���b�N�E�F�[�u:
		return std::shared_ptr<Skill_�V���b�N�E�F�[�u>(new Skill_�V���b�N�E�F�[�u(args.charPos, args.power, args.myCharType));
	case eID_�T���_�[�{�[��:
		return std::shared_ptr<Skill_�T���_�[�{�[��>(new Skill_�T���_�[�{�[��(args.charPos, args.power, args.myCharType, args.ariveTime));
	case eID_�\�[�h:
		return std::shared_ptr<Skill_�\�[�h�n>(new Skill_�\�[�h�n(Skill_�\�[�h�n::eTYPE_�\�[�h, args.charPos, args.power, args.myCharType));
	case eID_���C�h�\�[�h:
		return std::shared_ptr<Skill_�\�[�h�n>(new Skill_�\�[�h�n(Skill_�\�[�h�n::eTYPE_���C�h�\�[�h, args.charPos, args.power, args.myCharType));
	default:
		AppLogger::Error("SkillMgr::GetData wrong skill id (%d)", id);
		exit(1);
	}
	return std::shared_ptr<SkillData>();
}

std::shared_ptr<SkillData> SkillMgr::GetData(ChipData c, SkillArg args) {
	int id = -1;
	switch( c.id ) {
	case ChipMgr::eID_�L���m��:
		id = eID_�L���m��;
		break;
	case ChipMgr::eID_�n�C�L���m��:
		id = eID_�n�C�L���m��;
		break;
	case ChipMgr::eID_�T���_�[�{�[��:
		id = eID_�T���_�[�{�[��;
		break;
	case ChipMgr::eID_�X�g�[���L���[�u:
		id = eID_�X�g�[���L���[�u;
		break;
	case ChipMgr::eID_�\�[�h:
		id = eID_�\�[�h;
		break;
	case ChipMgr::eID_���C�h�\�[�h:
		id = eID_���C�h�\�[�h;
		break;
	case ChipMgr::eID_�t���C�����C��:
		id = eID_�t���C�����C��_�Œ�;
		break;
	case ChipMgr::eID_���J�o���[10:
	case ChipMgr::eID_���J�o���[30:
		id = eID_���J�o���[;
		break;
	case ChipMgr::eID_�u�[������:
		id = eID_�u�[������_����;
		break;
	case ChipMgr::eID_�~�j�{��:
		id = eID_�~�j�{��;
		break;
	default:
		AppLogger::Error("Chip %d�ɑ΂���X�L���͖������ł�", c.id);
		exit(1);
	}
	return GetData(id, args);
}

//-------------------------------------------------------
// �o�X�^�[
//-------------------------------------------------------
Skill_�o�X�^�[::Skill_�o�X�^�[(CPoint<int> charPos, int damage, CharType myCharType):SkillData(false) {
	int targetType = eCHAR_ALL ^ myCharType;
	CPoint<int> damagePos = BattleCharMgr::GetInst()->GetClosestCharPosWithSameLine(charPos, targetType);
	if( damagePos.x > 0 ) {// �U�����q�b�g����ʒu�ɑΏۂ�������
		// �_���[�W�f�[�^�̓o�^
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(damagePos, damage, targetType, 1, GetObjectID()));
		EffectArg args;
		args.pos = BattleField::GetPixelPos(damagePos);
		args.rndSize = 10;
		EffectMgr::GetInst()->Register(EffectMgr::eID_���b�N�o�X�^�[_HIT, args);
	}
}

Skill_�o�X�^�[::~Skill_�o�X�^�[() {
}

void Skill_�o�X�^�[::Draw() {
}

bool Skill_�o�X�^�[::Process() {
	return true;// ���I��
}

//-------------------------------------------------------
// �L���m��
//-------------------------------------------------------
Skill_�L���m���n::Skill_�L���m���n(CPoint<int> charPos, int damage, CharType myCharType, Type skillType)
	:charPos(charPos), count(0), myCharType(myCharType), imgBody(), damage(damage), SkillData(false) {

	int temp[9];
	std::string fname = def::SKILL_IMAGE_PATH + "�L���m��_body.png";
	LoadDivGraphWithErrorCheck(temp, fname, "Skill_�L���m��::Skill_�L���m��", 3, 3, 46, 40);
	for( int i = 0, n = 0; i < 9; i++ ) {
		if( ( i / 3 ) == skillType )
			imgBody[n++] = temp[i];
		else
			DeleteGraph(temp[i]);
	}

	// todo(�g���摜)
}

Skill_�L���m���n::~Skill_�L���m���n() {
	for( int i = 0; i < sizeof(imgBody) / sizeof(imgBody[0]); i++ )
		DeleteGraph(imgBody[i]);
}

void Skill_�L���m���n::Draw() {
	int ino = ( count / 5 ) - 1;
	if( ino >= 0 ) {
		if( ino > 2 )ino = 2;
		int dx = ( myCharType == eCHAR_PLAYER ) ? 45 : -10;// TODO(�Goffset�̒���)
		if( count >= 12 && count < 22 ) {
			dx -= 15;
		}
		CPoint<int> t = BattleField::GetPixelPos(charPos);
		DrawRotaGraph(t.x + dx, t.y - 11, 1, 0, imgBody[ino], TRUE);
	}
}

bool Skill_�L���m���n::Process() {
	count++;

	if( count == DAMAGE_REGISTER_COUNT ) {
		// �o�^���ԂɂȂ�����_���[�W��o�^
		int targetType = eCHAR_ALL ^ myCharType ^ eCHAR_OBJECT;
		CPoint<int> targetPos = BattleCharMgr::GetInst()->GetClosestCharPosWithSameLine(charPos, targetType);
		if( targetPos.x > 0 ) {// ������ɓG������Ȃ�
			BattleCharMgr::GetInst()->RegisterDamage(DamageData(targetPos, damage, targetType, 1, GetObjectID()));
		}
	}

	if( count >= SKILL_END_COUNT ) {
		return true;// �I��
	}

	return false;
}

Skill_�V���b�N�E�F�[�u::Skill_�V���b�N�E�F�[�u(CPoint<int> charPos, int damage, CharType myCharType)
	:count(0), myCharType(myCharType), image(), damage(damage), SkillData(true) {

	std::string fname = def::SKILL_IMAGE_PATH + "�V���b�N�E�F�[�u.png";
	LoadDivGraphWithErrorCheck(image, fname, "Skill_�V���b�N�E�F�[�u::Skill_�V���b�N�E�F�[�u", 7, 1, 100, 140);

	if( myCharType == eCHAR_PLAYER )
		this->atkPos.x = charPos.x + 1;// �ڂ̑O�̈�}�X
	else
		this->atkPos.x = charPos.x - 1;// �ڂ̑O�̈�}�X
	this->atkPos.y = charPos.y;

	int targetType = eCHAR_ALL ^ myCharType;
	BattleCharMgr::GetInst()->RegisterDamage(DamageData(this->atkPos, damage, targetType, NEXT_STEP_COUNT, GetObjectID()));
}

Skill_�V���b�N�E�F�[�u::~Skill_�V���b�N�E�F�[�u() {
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ ) {
		DeleteGraph(image[i]);
	}
}

void Skill_�V���b�N�E�F�[�u::Draw() {
	int ino = count / IMAGE_DELAY;
	CPoint<int> pos = BattleField::GetPixelPos(atkPos);
	DrawRotaGraph(pos.x, pos.y, 1, 0, image[ino], TRUE);
}

bool Skill_�V���b�N�E�F�[�u::Process() {
	if( count >= NEXT_STEP_COUNT ) {// ���J�E���g�����̃}�X��
		count = 0;
		atkPos.x += ( myCharType == eCHAR_PLAYER ) ? 1 : -1;
		// debug(���p�l��)
		if( atkPos.x < 0 || atkPos.x >= BattleField::FIELD_NUM_X ) {// �U�����͈͊O�ɍs����
			return true;
		}

		// �U���̓o�^
		int targetType = eCHAR_ALL ^ myCharType;
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(this->atkPos, damage, targetType, NEXT_STEP_COUNT, GetObjectID()));
	} else
		count++;
	// TODO(HitAREA�̕`��)
	return false;
}

Skill_�T���_�[�{�[��::Skill_�T���_�[�{�[��(CPoint<int> charPos, int damage, CharType myCharType, unsigned int ariveTime)
	:myCharType(myCharType), count(0), image(), damage(damage), ariveTime(ariveTime), SkillData(true) {

	std::string fname = def::SKILL_IMAGE_PATH + "�T���_�[�{�[��.png";
	LoadDivGraphWithErrorCheck(image, fname, "Skill_�T���_�[�{�[��::Skill_�T���_�[�{�[��", 4, 1, 64, 80);

	if( myCharType == eCHAR_PLAYER ) {
		this->atkPos.x = charPos.x + 1;// �ڂ̑O�̈�}�X
		moveDirect = def::eMUKI_RIGHT;
		startPos.x = -( float ) BattleField::PANEL_SIZE.x / 2.f;
	} else {
		this->atkPos.x = charPos.x - 1;// �ڂ̑O�̈�}�X
		moveDirect = def::eMUKI_LEFT;
		startPos.x = ( float ) BattleField::PANEL_SIZE.x / 2.f;
	}
	this->atkPos.y = charPos.y;
	drawOfs = startPos;

	// �_���[�W�o�^
	int targetType = eCHAR_ALL ^ myCharType;
	BattleCharMgr::GetInst()->RegisterDamage(DamageData(this->atkPos, damage, targetType, NEXT_STEP_COUNT, GetObjectID()));
}

Skill_�T���_�[�{�[��::~Skill_�T���_�[�{�[��() {
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ ) {
		DeleteGraph(image[i]);
	}
}

void Skill_�T���_�[�{�[��::Draw() {
	CPoint<int> pos = BattleField::GetPixelPos(atkPos);
	int ino = ( count / IMAGE_DELAY ) % 4;
	DrawRotaGraph(pos.x + ( int ) drawOfs.x, pos.y + ( int ) drawOfs.y + 20, 1, 0, image[ino], TRUE);
}

bool Skill_�T���_�[�{�[��::Process() {
	// �`��p��Offset�̍X�V
	int direct = ( count <= NEXT_STEP_COUNT / 2 ) ? moveDirect : nextMoveDirect;
	switch( direct ) {
	case def::eMUKI_RIGHT:	drawOfs.x += ( float ) BattleField::PANEL_SIZE.x / NEXT_STEP_COUNT;	break;
	case def::eMUKI_LEFT:	drawOfs.x -= ( float ) BattleField::PANEL_SIZE.x / NEXT_STEP_COUNT;	break;
	case def::eMUKI_DOWN:	drawOfs.y += ( float ) BattleField::PANEL_SIZE.y / NEXT_STEP_COUNT;	break;
	case def::eMUKI_UP:		drawOfs.y -= ( float ) BattleField::PANEL_SIZE.y / NEXT_STEP_COUNT;	break;
	}

	// ���̈ړ���͒����ɗ����i�K�Ō��肷��
	if( count == NEXT_STEP_COUNT / 2 ) {
		int targetType = eCHAR_ALL ^ myCharType ^ eCHAR_OBJECT;
		CPoint<int> pos = BattleCharMgr::GetInst()->GetClosestCharPos(atkPos, targetType);

		// �ړ�����(�G��ǂ�������)
		int x_dif = pos.x - atkPos.x;// �v�Z�����厖(��������)
		int y_dif = pos.y - atkPos.y;// �v�Z�����厖(��������)
		if( abs(x_dif) > abs(y_dif) ) {// x�����Ɉړ�
			if( x_dif > 0 ) {
				nextMoveDirect = def::eMUKI_RIGHT;
				startPos = CPoint<float>(-( float ) BattleField::PANEL_SIZE.x / 2.f, 0);
			} else {
				nextMoveDirect = def::eMUKI_LEFT;
				startPos = CPoint<float>(+( float ) BattleField::PANEL_SIZE.x / 2.f, 0);
			}
		} else {// y�����Ɉړ�
			if( y_dif > 0 ) {
				nextMoveDirect = def::eMUKI_DOWN;
				startPos = CPoint<float>(0, -( float ) BattleField::PANEL_SIZE.y / 2.f);
			} else {
				nextMoveDirect = def::eMUKI_UP;
				startPos = CPoint<float>(0, +( float ) BattleField::PANEL_SIZE.y / 2.f);
			}
		}
	}

	if( count == NEXT_STEP_COUNT ) {
		count = 0;
		drawOfs = startPos;
		moveDirect = nextMoveDirect;

		switch( nextMoveDirect ) {
		case def::eMUKI_RIGHT:	atkPos.x++;	break;
		case def::eMUKI_LEFT:	atkPos.x--;	break;
		case def::eMUKI_DOWN:	atkPos.y++;	break;
		case def::eMUKI_UP:		atkPos.y--;	break;
		}

		if( ariveTime > 1 ) {
			ariveTime--;
		} else {
			return true;
		}
		// debug(���p�l��)
		// �U�����͈͊O�ɍs����
		if( atkPos.x < 0 || atkPos.x >= BattleField::FIELD_NUM_X || atkPos.y < 0 || atkPos.y >= BattleField::FIELD_NUM_Y ) {
			return true;
		}

		// �_���[�W���o�^
		int targetType = eCHAR_ALL ^ myCharType;
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(this->atkPos, damage, targetType, NEXT_STEP_COUNT, GetObjectID()));
	}

	count++;
	return false;
}

Skill_�\�[�h�n::Skill_�\�[�h�n(Type type, CPoint<int> charPos, int damage, CharType myCharType)
	:myCharType(myCharType), count(0), image(), damage(damage), SkillData(true) {

	// Set Image
	std::string fname = def::SKILL_IMAGE_PATH + "�\�[�h.png";
	int t[12];
	LoadDivGraphWithErrorCheck(t, fname, "Skill_�\�[�h", 4, 3, 160, 150);
	for( int i = 0; i < 4; i++ ) {
		switch( type ) {
		case eTYPE_�\�[�h:
			image[i] = t[i];
			DeleteGraph(t[i + 4]);
			DeleteGraph(t[i + 8]);
			break;
		case eTYPE_���C�h�\�[�h:
			image[i] = t[i + 8];
			DeleteGraph(t[i]);
			DeleteGraph(t[i + 4]);
			break;
		default:
			AppLogger::Error("�\�����ʃ\�[�h�^�C�v���I������܂���. type: %d", type);
			break;
		}
	}

	// TODO(�U���ʒu�̃Z�b�g)
	CPoint<int> pos = charPos;
	switch( myCharType ) {
	case eCHAR_PLAYER:
		pos.x++;
		break;
	case eCHAR_ENEMY:
		pos.x--;
		break;
	default:
		AppLogger::Error("�\�����ʃL�����^�C�v���I������܂���. type: %d", myCharType);
		break;
	}
	atkPoses.push_back(pos);// �Ƃ肠�����ڂ̑O�̈�}�X�͍U��

	if( type == eTYPE_���C�h�\�[�h ) {
		// �㉺���U��
		atkPoses.push_back(CPoint<int>(pos.x, pos.y - 1));
		atkPoses.push_back(CPoint<int>(pos.x, pos.y + 1));
	}
}

Skill_�\�[�h�n::~Skill_�\�[�h�n() {
	for( int i = 0; i < 4; i++ ) {
		DeleteGraph(image[i]);
	}
}

void Skill_�\�[�h�n::Draw() {
	int ino = ( count / SKILL_DELAY );
	if( ino > 4 ) {
		ino = 4;
	}
	CPoint<int> pos = BattleField::GetPixelPos(atkPoses[0]);
	DrawRotaGraph(pos.x, pos.y, 1, 0, image[ino], TRUE);
}

bool Skill_�\�[�h�n::Process() {
	count++;
	if( count == SKILL_DELAY * 1 ) {
		// �_���[�W�f�[�^�̓o�^
		int targetType = eCHAR_ALL ^ myCharType;
		for( auto pos : atkPoses ) {
			BattleCharMgr::GetInst()->RegisterDamage(DamageData(pos, damage, targetType, 1, GetObjectID()));
		}
	} else if( count >= SKILL_DELAY * 4 ) {
		return true;
	}
	return false;
}
