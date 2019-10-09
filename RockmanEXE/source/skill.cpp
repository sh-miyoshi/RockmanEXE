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
	static const int DAMAGE_REGISTER_DELAY = 5 * 4;// �J�E���g��Ƀ_���[�W��o�^

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
	case ChipMgr::eID_�X�g�[���L���[�u:
	case ChipMgr::eID_�T���_�[�{�[��:
	case ChipMgr::eID_�\�[�h:
	case ChipMgr::eID_�t���C�����C��:
	case ChipMgr::eID_���J�o���[10:
	case ChipMgr::eID_���J�o���[30:
	case ChipMgr::eID_�u�[������:
	case ChipMgr::eID_�~�j�{��:
	case ChipMgr::eID_���C�h�\�[�h:
	default:
		AppLogger::Error("Chip %d�ɑ΂���X�L���͖������ł�", c.id);
		exit(1);
	}
	return GetData(id, args);
}

//-------------------------------------------------------
// �o�X�^�[
//-------------------------------------------------------
Skill_�o�X�^�[::Skill_�o�X�^�[(CPoint<int> charPos, int damage, CharType myCharType) {
	int targetType = eCHAR_ALL ^ myCharType;
	CPoint<int> damagePos = BattleCharMgr::GetInst()->GetClosestCharPosWithSameLine(charPos, targetType);
	if( damagePos.x > 0 ) {// �U�����q�b�g����ʒu�ɑΏۂ�������
		// �_���[�W�f�[�^�̓o�^
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(damagePos, damage, targetType));
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
	:charPos(charPos), count(0), myCharType(myCharType), imgBody(), damage(damage) {

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
		int dx = ( myCharType == eCHAR_PLAYER ) ? 40 : -10;// TODO(�Goffset�̒���)
		CPoint<int> t = BattleField::GetPixelPos(charPos);
		DrawRotaGraph(t.x+dx, t.y-10,1,0, imgBody[ino], TRUE);
	}
}

bool Skill_�L���m���n::Process() {
	count++;

	if( count >= DAMAGE_REGISTER_DELAY ) {
		// �o�^���ԂɂȂ�����_���[�W��o�^
		int targetType = eCHAR_ALL ^ myCharType;
		CPoint<int> targetPos = BattleCharMgr::GetInst()->GetClosestCharPosWithSameLine(charPos, targetType);
		if( targetPos.x > 0 ) {// ������ɓG������Ȃ�
			BattleCharMgr::GetInst()->RegisterDamage(DamageData(targetPos, damage, targetType));
		}
		return true;
	}

	return false;
}
