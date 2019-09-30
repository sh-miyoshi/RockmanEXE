#include "include.h"
#include "skill.h"
#include "battleCharMgr.h"
#include "battleField.h"

//-------------------------------------------------------
// �N���X��`
//-------------------------------------------------------
class Skill_�o�X�^�[:public SkillData {
public:
	Skill_�o�X�^�[(CPoint<int> charPos, int damage, int targetType);
	~Skill_�o�X�^�[();

	void Draw();
	bool Process();
};

std::shared_ptr<SkillData> SkillMgr::GetData(int id, SkillArg args) {
	switch( id ) {
	case eID_�o�X�^�[:
		return std::shared_ptr<Skill_�o�X�^�[>(new Skill_�o�X�^�[(args.charPos, args.power, args.targetType));
	default:
		AppLogger::Error(ToString("SkillMgr::GetData wrong skill id (%d)", id));
		exit(1);
	}
	return std::shared_ptr<SkillData>();
}

Skill_�o�X�^�[::Skill_�o�X�^�[(CPoint<int> charPos, int damage, int targetType) {
	int dist = BattleField::FIELD_NUM_X * BattleField::FIELD_NUM_X;
	std::vector<CPoint<int>> targets = BattleCharMgr::GetInst()->GetAllCharPos(targetType);
	CPoint<int> damagePos(-1, -1);
	for( auto t : targets ) {
		if( t.y == charPos.y ) {
			int d = ( t.x - charPos.x ) * ( t.x - charPos.x );
			if( d < dist ) {
				damagePos = t;
				dist = d;
			}
		}
	}

	if( damagePos.x > 0 ) {// �U�����q�b�g����ʒu�ɑΏۂ�������
		// �_���[�W�f�[�^�̓o�^
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(damagePos,damage,targetType));
	}
}

Skill_�o�X�^�[::~Skill_�o�X�^�[() {
}

void Skill_�o�X�^�[::Draw() {
}

bool Skill_�o�X�^�[::Process() {
	return true;// ���I��
}
