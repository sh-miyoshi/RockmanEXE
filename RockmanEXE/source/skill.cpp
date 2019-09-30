#include "include.h"
#include "skill.h"
#include "battleCharMgr.h"
#include "battleField.h"

//-------------------------------------------------------
// クラス定義
//-------------------------------------------------------
class Skill_バスター:public SkillData {
public:
	Skill_バスター(CPoint<int> charPos, int damage, int targetType);
	~Skill_バスター();

	void Draw();
	bool Process();
};

std::shared_ptr<SkillData> SkillMgr::GetData(int id, SkillArg args) {
	switch( id ) {
	case eID_バスター:
		return std::shared_ptr<Skill_バスター>(new Skill_バスター(args.charPos, args.power, args.targetType));
	default:
		AppLogger::Error(ToString("SkillMgr::GetData wrong skill id (%d)", id));
		exit(1);
	}
	return std::shared_ptr<SkillData>();
}

Skill_バスター::Skill_バスター(CPoint<int> charPos, int damage, int targetType) {
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

	if( damagePos.x > 0 ) {// 攻撃がヒットする位置に対象がいたら
		// ダメージデータの登録
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(damagePos,damage,targetType));
	}
}

Skill_バスター::~Skill_バスター() {
}

void Skill_バスター::Draw() {
}

bool Skill_バスター::Process() {
	return true;// 即終了
}
