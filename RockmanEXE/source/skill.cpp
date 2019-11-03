#include "include.h"
#include "skill.h"
#include "battleCharMgr.h"
#include "battleField.h"
#include "effectMgr.h"

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

	Skill_キャノン系(CPoint<int> charPos, int damage, CharType myCharType, Type skillType);
	~Skill_キャノン系();

	void Draw();
	bool Process();
};

class Skill_ショックウェーブ:public SkillData {
	static const int IMAGE_DELAY = 4;// 次の画像に移るまでのカウント
	static const int NEXT_STEP_COUNT = 7 * IMAGE_DELAY;

	CharType myCharType;
	int count;
	CPoint<int> atkPos;
	int image[7];
	int damage;
public:
	Skill_ショックウェーブ(CPoint<int> charPos, int damage, CharType myCharType);
	~Skill_ショックウェーブ();

	void Draw();
	bool Process();
};

class Skill_サンダーボール:public SkillData {
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
	Skill_サンダーボール(CPoint<int> charPos, int damage, CharType myCharType, unsigned int ariveTime);
	~Skill_サンダーボール();

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
	case eID_ショックウェーブ:
		return std::shared_ptr<Skill_ショックウェーブ>(new Skill_ショックウェーブ(args.charPos, args.power, args.myCharType));
	case eID_サンダーボール:
		return std::shared_ptr<Skill_サンダーボール>(new Skill_サンダーボール(args.charPos, args.power, args.myCharType, args.ariveTime));
	default:
		AppLogger::Error("SkillMgr::GetData wrong skill id (%d)", id);
		exit(1);
	}
	return std::shared_ptr<SkillData>();
}

std::shared_ptr<SkillData> SkillMgr::GetData(ChipData c, SkillArg args) {
	int id = -1;
	switch( c.id ) {
	case ChipMgr::eID_キャノン:
		id = eID_キャノン;
		break;
	case ChipMgr::eID_ハイキャノン:
		id = eID_ハイキャノン;
		break;
	case ChipMgr::eID_サンダーボール:
		id = eID_サンダーボール;
		break;
	case ChipMgr::eID_ストーンキューブ:
	case ChipMgr::eID_ソード:
	case ChipMgr::eID_フレイムライン:
	case ChipMgr::eID_リカバリー10:
	case ChipMgr::eID_リカバリー30:
	case ChipMgr::eID_ブーメラン:
	case ChipMgr::eID_ミニボム:
	case ChipMgr::eID_ワイドソード:
	default:
		AppLogger::Error("Chip %dに対するスキルは未実装です", c.id);
		exit(1);
	}
	return GetData(id, args);
}

//-------------------------------------------------------
// バスター
//-------------------------------------------------------
Skill_バスター::Skill_バスター(CPoint<int> charPos, int damage, CharType myCharType) {
	int targetType = eCHAR_ALL ^ myCharType;
	CPoint<int> damagePos = BattleCharMgr::GetInst()->GetClosestCharPosWithSameLine(charPos, targetType);
	if( damagePos.x > 0 ) {// 攻撃がヒットする位置に対象がいたら
		// ダメージデータの登録
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(damagePos, damage, targetType, 1));
		EffectArg args;
		args.pos = BattleField::GetPixelPos(damagePos);
		args.rndSize = 10;
		EffectMgr::GetInst()->Register(EffectMgr::eID_ロックバスター_HIT, args);
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
		int dx = ( myCharType == eCHAR_PLAYER ) ? 45 : -10;// TODO(敵offsetの調整)
		if( count >= 12 && count < 22 ) {
			dx -= 15;
		}
		CPoint<int> t = BattleField::GetPixelPos(charPos);
		DrawRotaGraph(t.x + dx, t.y - 11, 1, 0, imgBody[ino], TRUE);
	}
}

bool Skill_キャノン系::Process() {
	count++;

	if( count == DAMAGE_REGISTER_COUNT ) {
		// 登録時間になったらダメージを登録
		int targetType = eCHAR_ALL ^ myCharType ^ eCHAR_OBJECT;
		CPoint<int> targetPos = BattleCharMgr::GetInst()->GetClosestCharPosWithSameLine(charPos, targetType);
		if( targetPos.x > 0 ) {// 直線上に敵がいるなら
			BattleCharMgr::GetInst()->RegisterDamage(DamageData(targetPos, damage, targetType, 1));
		}
	}

	if( count >= SKILL_END_COUNT ) {
		return true;// 終了
	}

	return false;
}

Skill_ショックウェーブ::Skill_ショックウェーブ(CPoint<int> charPos, int damage, CharType myCharType)
	:count(0), myCharType(myCharType), image(), damage(damage) {

	std::string fname = def::SKILL_IMAGE_PATH + "ショックウェーブ.png";
	LoadDivGraphWithErrorCheck(image, fname, "Skill_ショックウェーブ::Skill_ショックウェーブ", 7, 1, 100, 140);

	if( myCharType == eCHAR_PLAYER )
		this->atkPos.x = charPos.x + 1;// 目の前の一マス
	else
		this->atkPos.x = charPos.x - 1;// 目の前の一マス
	this->atkPos.y = charPos.y;

	int targetType = eCHAR_ALL ^ myCharType;
	BattleCharMgr::GetInst()->RegisterDamage(DamageData(this->atkPos, damage, targetType, NEXT_STEP_COUNT));
}

Skill_ショックウェーブ::~Skill_ショックウェーブ() {
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ ) {
		DeleteGraph(image[i]);
	}
}

void Skill_ショックウェーブ::Draw() {
	int ino = count / IMAGE_DELAY;
	CPoint<int> pos = BattleField::GetPixelPos(atkPos);
	DrawRotaGraph(pos.x, pos.y, 1, 0, image[ino], TRUE);
}

bool Skill_ショックウェーブ::Process() {
	if( count >= NEXT_STEP_COUNT ) {// 一定カウント後一個次のマスへ
		count = 0;
		atkPos.x += ( myCharType == eCHAR_PLAYER ) ? 1 : -1;
		// debug(穴パネル)
		if( atkPos.x < 0 || atkPos.x >= BattleField::FIELD_NUM_X ) {// 攻撃が範囲外に行けば
			return true;
		}

		// 攻撃の登録
		int targetType = eCHAR_ALL ^ myCharType;
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(this->atkPos, damage, targetType, NEXT_STEP_COUNT));
	} else
		count++;
	// TODO(HitAREAの描画)
	return false;
}

Skill_サンダーボール::Skill_サンダーボール(CPoint<int> charPos, int damage, CharType myCharType, unsigned int ariveTime)
	:myCharType(myCharType), count(0), image(), damage(damage), ariveTime(ariveTime) {

	std::string fname = def::SKILL_IMAGE_PATH + "サンダーボール.png";
	LoadDivGraphWithErrorCheck(image, fname, "Skill_サンダーボール::Skill_サンダーボール", 4, 1, 64, 80);

	if( myCharType == eCHAR_PLAYER ) {
		this->atkPos.x = charPos.x + 1;// 目の前の一マス
		moveDirect = def::eMUKI_RIGHT;
		startPos.x = -( float ) BattleField::PANEL_SIZE.x / 2.f;
	} else {
		this->atkPos.x = charPos.x - 1;// 目の前の一マス
		moveDirect = def::eMUKI_LEFT;
		startPos.x = ( float ) BattleField::PANEL_SIZE.x / 2.f;
	}
	this->atkPos.y = charPos.y;
	drawOfs = startPos;

	// ダメージ登録
	int targetType = eCHAR_ALL ^ myCharType;
	BattleCharMgr::GetInst()->RegisterDamage(DamageData(this->atkPos, damage, targetType, NEXT_STEP_COUNT));
}

Skill_サンダーボール::~Skill_サンダーボール() {
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ ) {
		DeleteGraph(image[i]);
	}
}

void Skill_サンダーボール::Draw() {
	CPoint<int> pos = BattleField::GetPixelPos(atkPos);
	int ino = ( count / IMAGE_DELAY ) % 4;
	DrawRotaGraph(pos.x + ( int ) drawOfs.x, pos.y + ( int ) drawOfs.y + 20, 1, 0, image[ino], TRUE);
}

bool Skill_サンダーボール::Process() {
	// 描画用のOffsetの更新
	int direct = ( count <= NEXT_STEP_COUNT / 2 ) ? moveDirect : nextMoveDirect;
	switch( direct ) {
	case def::eMUKI_RIGHT:	drawOfs.x += ( float ) BattleField::PANEL_SIZE.x / NEXT_STEP_COUNT;	break;
	case def::eMUKI_LEFT:	drawOfs.x -= ( float ) BattleField::PANEL_SIZE.x / NEXT_STEP_COUNT;	break;
	case def::eMUKI_DOWN:	drawOfs.y += ( float ) BattleField::PANEL_SIZE.y / NEXT_STEP_COUNT;	break;
	case def::eMUKI_UP:		drawOfs.y -= ( float ) BattleField::PANEL_SIZE.y / NEXT_STEP_COUNT;	break;
	}

	// 次の移動先は中央に来た段階で決定する
	if( count == NEXT_STEP_COUNT / 2 ) {
		int targetType = eCHAR_ALL ^ myCharType ^ eCHAR_OBJECT;
		CPoint<int> pos = BattleCharMgr::GetInst()->GetClosestCharPos(atkPos, targetType);

		// 移動処理(敵を追いかける)
		int x_dif = pos.x - atkPos.x;// 計算順序大事(方向決め)
		int y_dif = pos.y - atkPos.y;// 計算順序大事(方向決め)
		if( abs(x_dif) > abs(y_dif) ) {// x方向に移動
			if( x_dif > 0 ) {
				nextMoveDirect = def::eMUKI_RIGHT;
				startPos = CPoint<float>(-( float ) BattleField::PANEL_SIZE.x / 2.f, 0);
			} else {
				nextMoveDirect = def::eMUKI_LEFT;
				startPos = CPoint<float>(+( float ) BattleField::PANEL_SIZE.x / 2.f, 0);
			}
		} else {// y方向に移動
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
		// debug(穴パネル)
		// 攻撃が範囲外に行けば
		if( atkPos.x < 0 || atkPos.x >= BattleField::FIELD_NUM_X || atkPos.y < 0 || atkPos.y >= BattleField::FIELD_NUM_Y ) {
			return true;
		}

		// ダメージ情報登録
		int targetType = eCHAR_ALL ^ myCharType;
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(this->atkPos, damage, targetType, NEXT_STEP_COUNT));
	}

	count++;
	return false;
}
