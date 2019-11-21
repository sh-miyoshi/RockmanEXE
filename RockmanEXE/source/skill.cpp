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

	int imgBody[3];
	CPoint<int> charPos;
public:
	enum Type {
		eTYPE_NORMAL,
		eTYPE_HIGH,
		eTYPE_MEGA,
	};

	Skill_キャノン系(Type skillType, CPoint<int> charPos, int damage, CharType myCharType);
	~Skill_キャノン系();

	void Draw();
	bool Process();
};

class Skill_ショックウェーブ:public SkillData {
	static const int IMAGE_DELAY = 4;// 次の画像に移るまでのカウント
	static const int NEXT_STEP_COUNT = 7 * IMAGE_DELAY;

	CPoint<int> atkPos;
	int image[7];
public:
	Skill_ショックウェーブ(CPoint<int> charPos, int damage, CharType myCharType);
	~Skill_ショックウェーブ();

	void Draw();
	bool Process();
};

class Skill_サンダーボール:public SkillData {
	static const int NEXT_STEP_COUNT = 80;
	static const int IMAGE_DELAY = 6;

	CPoint<int> atkPos;
	CPoint<float> startPos, drawOfs;
	def::Muki moveDirect, nextMoveDirect;
	int image[4];
	unsigned int ariveTime;
public:
	Skill_サンダーボール(CPoint<int> charPos, int damage, CharType myCharType, unsigned int ariveTime);
	~Skill_サンダーボール();

	void Draw();
	bool Process();
};

class Skill_ソード系:public SkillData {
	static const int SKILL_DELAY = 3;// 次の画像に移るまでのカウント

	int image[4];
	std::vector<CPoint<int>> atkPoses;
public:
	enum Type {
		eTYPE_ソード,
		eTYPE_ワイドソード,
	};

	Skill_ソード系(Type type, CPoint<int> charPos, int damage, CharType myCharType);
	~Skill_ソード系();

	void Draw();
	bool Process();
};

class Skill_ブーメラン:public SkillData {
public:
	enum Type {
		eTYPE_直線,
		eTYPE_周回
	};
private:
	static const int IMAGE_DELAY = 4;// 次の画像に移るまでのカウント
	static const int NEXT_STEP_COUNT = 2 * IMAGE_DELAY;



	int image[4];
	CPoint<int> atkPos;
	Type attackType;
	int muki;// 周回の時の向き(UP or DOWN)

	bool IsWayBack();
	bool IsCorner(int x, int y);
public:
	Skill_ブーメラン(Type type, CPoint<int> initAtkPos, int damage, CharType myCharType);
	~Skill_ブーメラン();

	void Draw();
	bool Process();
};

class Skill_ミニボム:public SkillData {
	static const unsigned BOMB_ROTATE_DELAY = 4;
	static const unsigned EXPLODE_DRAW_DLAY = 3;
	static const unsigned SKILL_COUNT = 40;// ボムを投げてから着弾するまでのカウント

	CPoint<int> initPos;
	CPoint<float> atkPos;
	bool isBombView;
	int imgBomb[5];
	int imgExplode[16];
	unsigned int explodeCount;
public:
	Skill_ミニボム(CPoint<int> charPos, int damage, CharType myCharType);
	~Skill_ミニボム();

	void Draw();
	bool Process();
};

class Skill_リカバリー:public SkillData {
	static const unsigned int SKILL_DELAY = 2;

	int image[8];
	CPoint<int> charPos;
public:
	Skill_リカバリー(CPoint<int> charPos, int power, CharType myCharType);
	~Skill_リカバリー();

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
		return std::shared_ptr<Skill_キャノン系>(new Skill_キャノン系(Skill_キャノン系::eTYPE_NORMAL, args.charPos, args.power, args.myCharType));
	case eID_ハイキャノン:
		return std::shared_ptr<Skill_キャノン系>(new Skill_キャノン系(Skill_キャノン系::eTYPE_HIGH, args.charPos, args.power, args.myCharType));
	case eID_ショックウェーブ:
		return std::shared_ptr<Skill_ショックウェーブ>(new Skill_ショックウェーブ(args.charPos, args.power, args.myCharType));
	case eID_サンダーボール:
		return std::shared_ptr<Skill_サンダーボール>(new Skill_サンダーボール(args.charPos, args.power, args.myCharType, args.ariveTime));
	case eID_ソード:
		return std::shared_ptr<Skill_ソード系>(new Skill_ソード系(Skill_ソード系::eTYPE_ソード, args.charPos, args.power, args.myCharType));
	case eID_ワイドソード:
		return std::shared_ptr<Skill_ソード系>(new Skill_ソード系(Skill_ソード系::eTYPE_ワイドソード, args.charPos, args.power, args.myCharType));
	case eID_ブーメラン_周回:
		return std::shared_ptr<Skill_ブーメラン>(new Skill_ブーメラン(Skill_ブーメラン::eTYPE_周回, args.charPos, args.power, args.myCharType));
	case eID_ブーメラン_直線:
		return std::shared_ptr<Skill_ブーメラン>(new Skill_ブーメラン(Skill_ブーメラン::eTYPE_直線, args.charPos, args.power, args.myCharType));
	case eID_ミニボム:
		return std::shared_ptr<Skill_ミニボム>(new Skill_ミニボム(args.charPos, args.power, args.myCharType));
	case eID_リカバリー:
		return std::shared_ptr<Skill_リカバリー>(new Skill_リカバリー(args.charPos, args.power, args.myCharType));
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
		id = eID_ストーンキューブ;
		break;
	case ChipMgr::eID_ソード:
		id = eID_ソード;
		break;
	case ChipMgr::eID_ワイドソード:
		id = eID_ワイドソード;
		break;
	case ChipMgr::eID_フレイムライン:
		id = eID_フレイムライン_固定;
		break;
	case ChipMgr::eID_リカバリー10:
		id = eID_リカバリー;
		args.power = 10;
		break;
	case ChipMgr::eID_リカバリー30:
		id = eID_リカバリー;
		args.power = 30;
		break;
	case ChipMgr::eID_ブーメラン:
		id = eID_ブーメラン_周回;
		args.charPos = CPoint<int>(0, BattleField::FIELD_NUM_Y - 1);// チップの場合は左下から攻撃
		break;
	case ChipMgr::eID_ミニボム:
		id = eID_ミニボム;
		break;
	default:
		AppLogger::Error("Chip %dに対するスキルは未実装です", c.id);
		exit(1);
	}
	return GetData(id, args);
}

//-------------------------------------------------------
// バスター
//-------------------------------------------------------
Skill_バスター::Skill_バスター(CPoint<int> charPos, int damage, CharType myCharType)
	:SkillData(damage, myCharType, false) {

	int targetType = eCHAR_ALL ^ myCharType;
	CPoint<int> damagePos = BattleCharMgr::GetInst()->GetClosestCharPosWithSameLine(charPos, targetType);
	if( damagePos.x > 0 ) {// 攻撃がヒットする位置に対象がいたら
		// ダメージデータの登録
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(damagePos, damage, targetType, 1, GetObjectID()));
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
Skill_キャノン系::Skill_キャノン系(Type skillType, CPoint<int> charPos, int damage, CharType myCharType)
	:charPos(charPos), imgBody(), SkillData(damage, myCharType, false) {

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
			BattleCharMgr::GetInst()->RegisterDamage(DamageData(targetPos, damage, targetType, 1, GetObjectID()));
		}
	}

	if( count >= SKILL_END_COUNT ) {
		return true;// 終了
	}

	return false;
}

//-------------------------------------------------------
// ショックウェーブ
//-------------------------------------------------------
Skill_ショックウェーブ::Skill_ショックウェーブ(CPoint<int> charPos, int damage, CharType myCharType)
	:image(), SkillData(damage, myCharType, true) {

	std::string fname = def::SKILL_IMAGE_PATH + "ショックウェーブ.png";
	LoadDivGraphWithErrorCheck(image, fname, "Skill_ショックウェーブ::Skill_ショックウェーブ", 7, 1, 100, 140);

	if( myCharType == eCHAR_PLAYER )
		this->atkPos.x = charPos.x + 1;// 目の前の一マス
	else
		this->atkPos.x = charPos.x - 1;// 目の前の一マス
	this->atkPos.y = charPos.y;

	int targetType = eCHAR_ALL ^ myCharType;
	BattleCharMgr::GetInst()->RegisterDamage(DamageData(this->atkPos, damage, targetType, NEXT_STEP_COUNT, GetObjectID()));
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
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(this->atkPos, damage, targetType, NEXT_STEP_COUNT, GetObjectID()));
	} else
		count++;
	// TODO(HitAREAの描画)
	return false;
}

//-------------------------------------------------------
// サンダーボール
//-------------------------------------------------------
Skill_サンダーボール::Skill_サンダーボール(CPoint<int> charPos, int damage, CharType myCharType, unsigned int ariveTime)
	:image(), ariveTime(ariveTime), SkillData(damage, myCharType, true) {

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
	BattleCharMgr::GetInst()->RegisterDamage(DamageData(this->atkPos, damage, targetType, NEXT_STEP_COUNT, GetObjectID()));
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
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(this->atkPos, damage, targetType, NEXT_STEP_COUNT, GetObjectID()));
	}

	count++;
	return false;
}

//-------------------------------------------------------
// ソード系
//-------------------------------------------------------
Skill_ソード系::Skill_ソード系(Type type, CPoint<int> charPos, int damage, CharType myCharType)
	:image(), SkillData(damage, myCharType, true) {

	// Set Image
	std::string fname = def::SKILL_IMAGE_PATH + "ソード.png";
	int t[12];
	LoadDivGraphWithErrorCheck(t, fname, "Skill_ソード", 4, 3, 160, 150);
	for( int i = 0; i < 4; i++ ) {
		switch( type ) {
		case eTYPE_ソード:
			image[i] = t[i];
			DeleteGraph(t[i + 4]);
			DeleteGraph(t[i + 8]);
			break;
		case eTYPE_ワイドソード:
			image[i] = t[i + 8];
			DeleteGraph(t[i]);
			DeleteGraph(t[i + 4]);
			break;
		default:
			AppLogger::Error("予期せぬソードタイプが選択されました. type: %d", type);
			break;
		}
	}

	// TODO(攻撃位置のセット)
	CPoint<int> pos = charPos;
	switch( myCharType ) {
	case eCHAR_PLAYER:
		pos.x++;
		break;
	case eCHAR_ENEMY:
		pos.x--;
		break;
	default:
		AppLogger::Error("予期せぬキャラタイプが選択されました. type: %d", myCharType);
		break;
	}
	atkPoses.push_back(pos);// とりあえず目の前の一マスは攻撃

	if( type == eTYPE_ワイドソード ) {
		// 上下も攻撃
		atkPoses.push_back(CPoint<int>(pos.x, pos.y - 1));
		atkPoses.push_back(CPoint<int>(pos.x, pos.y + 1));
	}
}

Skill_ソード系::~Skill_ソード系() {
	for( int i = 0; i < 4; i++ ) {
		DeleteGraph(image[i]);
	}
}

void Skill_ソード系::Draw() {
	int ino = ( count / SKILL_DELAY );
	if( ino > 4 ) {
		ino = 4;
	}
	CPoint<int> pos = BattleField::GetPixelPos(atkPoses[0]);
	DrawRotaGraph(pos.x, pos.y, 1, 0, image[ino], TRUE);
}

bool Skill_ソード系::Process() {
	count++;
	if( count == SKILL_DELAY * 1 ) {
		// ダメージデータの登録
		int targetType = eCHAR_ALL ^ myCharType;
		for( auto pos : atkPoses ) {
			BattleCharMgr::GetInst()->RegisterDamage(DamageData(pos, damage, targetType, 1, GetObjectID()));
		}
	} else if( count >= SKILL_DELAY * 4 ) {
		return true;
	}
	return false;
}

bool Skill_ブーメラン::IsWayBack() {
	return ( muki == def::eMUKI_UP && atkPos.y == 0 ) || ( muki == def::eMUKI_DOWN && atkPos.y == BattleField::FIELD_NUM_Y - 1 );
}

bool Skill_ブーメラン::IsCorner(int x, int y) {
	int trgX = ( myCharType == eCHAR_PLAYER ) ? BattleField::FIELD_NUM_X - 1 : 0;
	int trgY = ( muki == def::eMUKI_UP ) ? 0 : BattleField::FIELD_NUM_Y - 1;
	return ( x == trgX ) && ( y == trgY );
}

//-------------------------------------------------------
// ブーメラン
//-------------------------------------------------------
Skill_ブーメラン::Skill_ブーメラン(Type type, CPoint<int> initAtkPos, int damage, CharType myCharType)
	:SkillData(damage, myCharType, true), image(), atkPos(initAtkPos), attackType(type), muki(def::eMUKI_UP) {

	// 画像の読み込み
	std::string fname = def::SKILL_IMAGE_PATH + "ブーメラン.png";
	LoadDivGraphWithErrorCheck(image, fname, "Skill_ブーメラン::Skill_ブーメラン", 4, 1, 100, 80);

	// 向きのセット
	if( type == eTYPE_周回 ) {
		if( atkPos.y == 0 ) {
			muki = def::eMUKI_DOWN;
		} else if( atkPos.y == BattleField::FIELD_NUM_Y - 1 ) {
			muki = def::eMUKI_UP;
		} else {
			AppLogger::Error("ブーメラン周回タイプで無効な位置がセットされています. y: %d", atkPos.y);
		}
	}

	int targetType = eCHAR_ALL ^ myCharType;
	BattleCharMgr::GetInst()->RegisterDamage(DamageData(atkPos, damage, targetType, 1, GetObjectID()));
}

Skill_ブーメラン::~Skill_ブーメラン() {
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ ) {
		DeleteGraph(image[i]);
	}
}

void Skill_ブーメラン::Draw() {
	int ino = ( count % 16 ) / 4;
	const CPoint<int> ofs(0, 20);
	int xd = 0, yd = 0;
	switch( attackType ) {
	case eTYPE_直線:
		xd = ( int ) ( ( double ) ( count % NEXT_STEP_COUNT ) * BattleField::PANEL_SIZE.x / NEXT_STEP_COUNT ) - 50;
		if( myCharType == eCHAR_ENEMY )
			xd *= -1;
		break;
	case eTYPE_周回:
		if( ( ( myCharType == eCHAR_PLAYER ) && ( atkPos.x == BattleField::FIELD_NUM_X - 1 ) ) || ( ( myCharType == eCHAR_ENEMY ) && ( atkPos.x == 0 ) ) ) {// 縦方向に回転
			yd = ( int ) ( ( double ) ( count % NEXT_STEP_COUNT ) * BattleField::PANEL_SIZE.y / ( NEXT_STEP_COUNT * 2 ) );
			if( muki == def::eMUKI_UP )
				yd *= -1;
		} else {
			xd = ( int ) ( ( double ) ( count % NEXT_STEP_COUNT ) * BattleField::PANEL_SIZE.x / NEXT_STEP_COUNT ) - 50;
			if( IsWayBack() )// 帰り道なら反転
				xd *= -1;
			if( myCharType == eCHAR_ENEMY )
				xd *= -1;
		}
		break;
	}
	CPoint<int> pos = BattleField::GetPixelPos(atkPos);
	DrawRotaGraph(pos.x + ofs.x + xd, pos.y + ofs.y + yd, 1, 0, image[ino], TRUE);
}

bool Skill_ブーメラン::Process() {
	if( count % NEXT_STEP_COUNT == NEXT_STEP_COUNT - 1 ) {// 一定カウント後一個次のマスへ
		switch( attackType ) {
		case eTYPE_直線:
			atkPos.x += ( myCharType == eCHAR_PLAYER ) ? 1 : -1;
			break;
		case eTYPE_周回:
			if(
				( myCharType == eCHAR_PLAYER && atkPos.x == BattleField::FIELD_NUM_X - 1 && !IsCorner(atkPos.x, atkPos.y) ) ||
				( myCharType == eCHAR_ENEMY && atkPos.x == 0 && !IsCorner(atkPos.x, atkPos.y) )
				) {// Y方向に進むとき

				atkPos.y += ( muki == def::eMUKI_UP ) ? -1 : 1;
			} else {// X方向に進むとき

				int nextX = 1;
				// 帰り道なら反転
				if( ( muki == def::eMUKI_UP && atkPos.y == 0 ) || ( muki == def::eMUKI_DOWN && atkPos.y == BattleField::FIELD_NUM_Y - 1 ) ) {
					nextX = -1;
				}
				if( myCharType == eCHAR_ENEMY )// 敵の攻撃なら逆方向
					nextX *= -1;
				atkPos.x += nextX;
			}
			break;
		}

		if( atkPos.x < 0 || atkPos.x >= BattleField::FIELD_NUM_X ) {// 攻撃が範囲外に行けば
			return true;
		}

		int targetType = eCHAR_ALL ^ myCharType;
		BattleCharMgr::GetInst()->RegisterDamage(DamageData(atkPos, damage, targetType, 1, GetObjectID()));
	}
	count++;
	return false;
}

//-------------------------------------------------------
// ミニボム
//-------------------------------------------------------
Skill_ミニボム::Skill_ミニボム(CPoint<int> charPos, int damage, CharType myCharType)
	:explodeCount(0), isBombView(false), imgBomb(), imgExplode()
	, SkillData(damage, myCharType, false), initPos(charPos) {

	std::string fname = def::SKILL_IMAGE_PATH + "ミニボム.png";
	LoadDivGraphWithErrorCheck(imgBomb, fname, "Skill_ミニボム", 5, 1, 40, 30);

	fname = def::SKILL_IMAGE_PATH + "explode.png";
	LoadDivGraphWithErrorCheck(imgExplode, fname, "Skill_ミニボム", 8, 2, 110, 124);

}

Skill_ミニボム::~Skill_ミニボム() {
	for( int i = 0; i < 5; i++ ) {
		DeleteGraph(imgBomb[i]);
	}

	for( int i = 0; i < 16; i++ ) {
		DeleteGraph(imgExplode[i]);
	}
}

void Skill_ミニボム::Draw() {
	if( isBombView ) {
		int ino = ( count / BOMB_ROTATE_DELAY ) % 4;
		int x = BattleField::PANEL_SIZE.x * initPos.x + 20 + ( int ) atkPos.x;
		int y = BattleField::PANEL_SIZE.y * initPos.y + 170 + ( int ) atkPos.y;
		DrawGraph(x, y, imgBomb[ino], TRUE);
	}

	// 落ちた時にボム処理
	if( explodeCount > 0 ) {
		int ino = ( explodeCount / EXPLODE_DRAW_DLAY ) * 2;
		if( ino > 15 )
			ino = 15;
		int x = BattleField::PANEL_SIZE.x * ( initPos.x + 3 ) - 10;
		int y = BattleField::PANEL_SIZE.y * initPos.y + 90;
		DrawGraph(x, y, imgExplode[ino], TRUE);
	}
}

bool Skill_ミニボム::Process() {
	count++;

	if( explodeCount == 0 ) {
		// 放物線の計算
		atkPos.x += ( BattleField::PANEL_SIZE.x * 3.f / SKILL_COUNT );
		if( atkPos.x > BattleField::PANEL_SIZE.x / 2 )
			isBombView = true;
		if( atkPos.x >= BattleField::PANEL_SIZE.x * 3.f ) {
			// 着弾時
			isBombView = false;
			explodeCount = 1;
			// todo(不発の時はここでreturn true)

			// ダメージの登録(投げた位置から3マス前に着弾) TODO(任意の位置に投げられるようにする)
			CPoint<int> pos = initPos;
			if( myCharType == eCHAR_PLAYER ) {
				pos.x += 3;
			} else if( myCharType == eCHAR_ENEMY ) {
				pos.x -= 3;
			}

			int targetType = eCHAR_ALL ^ myCharType;
			BattleCharMgr::GetInst()->RegisterDamage(DamageData(pos, damage, targetType, 1, GetObjectID()));
		}
		atkPos.y = atkPos.x * ( ( atkPos.x / 120 ) - 2 );// y = x^2/120 - 2x
	} else {
		// 着弾後の処理
		explodeCount++;
		if( explodeCount > EXPLODE_DRAW_DLAY * 15 / 2 ) {
			return true;
		}
	}

	return false;
}

Skill_リカバリー::Skill_リカバリー(CPoint<int> charPos, int power, CharType myCharType)
	:SkillData(-power, myCharType, false), charPos(charPos) {

	std::string fname = def::SKILL_IMAGE_PATH + "リカバリー.png";
	LoadDivGraphWithErrorCheck(image, fname, "Skill_リカバリー::Skill_リカバリー", 8, 1, 84, 144);

	BattleCharMgr::GetInst()->RegisterRecovery(charPos, power, myCharType);
}

Skill_リカバリー::~Skill_リカバリー() {
	for( int i = 0; i < 8; i++ ) {
		DeleteGraph(image[i]);
	}
}

void Skill_リカバリー::Draw() {
	int ino = ( count / SKILL_DELAY );
	if( ino > 7 ) {
		ino = 7;
	}

	CPoint<int> pos = BattleField::GetPixelPos(charPos);
	DrawRotaGraph(pos.x, pos.y - 10, 1, 0, image[ino], TRUE);
}

bool Skill_リカバリー::Process() {
	count++;
	if( count >= SKILL_DELAY * 8 ) {
		return true;
	}
	return false;
}
