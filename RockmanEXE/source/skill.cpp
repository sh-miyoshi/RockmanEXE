#include "include.h"
#include "skill.h"
#include "battleCharMgr.h"
#include "battleField.h"

// TODO(敵にヒット時のエフェクト)

//-------------------------------------------------------
// クラス定義
//-------------------------------------------------------
class Skill_バスター:public SkillData {
public:
	Skill_バスター(CPoint<int> charPos, int damage, CharType myCharType);
	~Skill_バスター();

	void Draw();
	bool Process();
};

class Skill_キャノン系:public SkillData {
	static const int DAMAGE_REGISTER_DELAY = 5 * 4;// カウント後にダメージを登録

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

	Skill_キャノン系(CPoint<int> charPos, int damage, CharType myCharType, Type skillType);
	~Skill_キャノン系();

	void Draw();
	bool Process();
};

//-------------------------------------------------------
// グローバルメソッド
//-------------------------------------------------------
std::shared_ptr<SkillData> SkillMgr::GetData(int id, SkillArg args) {
	switch( id ) {
	case eID_バスター:
		return std::shared_ptr<Skill_バスター>(new Skill_バスター(args.charPos, args.power, args.myCharType));
	case eID_キャノン:
		return std::shared_ptr<Skill_キャノン系>(new Skill_キャノン系(args.charPos, args.power, args.myCharType, Skill_キャノン系::eTYPE_NORMAL));
	case eID_ハイキャノン:
		return std::shared_ptr<Skill_キャノン系>(new Skill_キャノン系(args.charPos, args.power, args.myCharType, Skill_キャノン系::eTYPE_HIGH));
	default:
		AppLogger::Error("SkillMgr::GetData wrong skill id (%d)", id);
		exit(1);
	}
	return std::shared_ptr<SkillData>();
}

//-------------------------------------------------------
// バスター
//-------------------------------------------------------
Skill_バスター::Skill_バスター(CPoint<int> charPos, int damage, CharType myCharType) {
	int targetType = eCHAR_ALL ^ myCharType;
	CPoint<int> damagePos = BattleCharMgr::GetInst()->GetClosestCharPosWithSameLine(charPos, targetType);
	if( damagePos.x > 0 ) {// 攻撃がヒットする位置に対象がいたら
		// ダメージデータの登録
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(damagePos, damage, targetType));
	}
}

Skill_バスター::~Skill_バスター() {
}

void Skill_バスター::Draw() {
}

bool Skill_バスター::Process() {
	return true;// 即終了
}

//-------------------------------------------------------
// キャノン
//-------------------------------------------------------
Skill_キャノン系::Skill_キャノン系(CPoint<int> charPos, int damage, CharType myCharType, Type skillType)
	:charPos(charPos), count(0), myCharType(myCharType), imgBody(), damage(damage) {

	int temp[9];
	std::string fname = def::SKILL_IMAGE_PATH + "キャノン_body.png";
	LoadDivGraphWithErrorCheck(temp, fname, "Skill_キャノン::Skill_キャノン", 3, 3, 46, 40);
	for( int i = 0, n = 0; i < 9; i++ ) {
		if( ( i / 3 ) == skillType )
			imgBody[n++] = temp[i];
		else
			DeleteGraph(temp[i]);
	}

	// todo(波動画像)
}

Skill_キャノン系::~Skill_キャノン系() {
	for( int i = 0; i < sizeof(imgBody) / sizeof(imgBody[0]); i++ )
		DeleteGraph(imgBody[i]);
}

void Skill_キャノン系::Draw() {
	int ino = ( count / 5 ) - 1;
	if( ino >= 0 ) {
		if( ino > 2 )ino = 2;
		int dx = ( myCharType == eCHAR_PLAYER ) ? 65 : -10;
		int x = BattleField::PANEL_SIZE.x * charPos.x + dx;
		int y = BattleField::BATTLE_PANEL_OFFSET_Y + BattleField::PANEL_SIZE.y * charPos.y - 45;
		DrawGraph(x, y, imgBody[ino], TRUE);
	}
}

bool Skill_キャノン系::Process() {
	count++;

	if( count >= DAMAGE_REGISTER_DELAY ) {
		// 登録時間になったらダメージを登録
		int targetType = eCHAR_ALL ^ myCharType;
		CPoint<int> targetPos = BattleCharMgr::GetInst()->GetClosestCharPosWithSameLine(charPos, targetType);
		if( targetPos.x > 0 ) {// 直線上に敵がいるなら
			BattleCharMgr::GetInst()->RegisterDamage(DamageData(targetPos, damage, targetType));
		}
		return true;
	}

	return false;
}
