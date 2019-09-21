#include "include.h"
#include "battleField.h"
#include "skill.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"

//-------------------------------------------------------
// 内部関数
//-------------------------------------------------------
bool TargetCondX(CPoint<int> mypos, CPoint<int> epos, bool isleft){
	if( isleft )
		return mypos.x > epos.x;
	else
		return mypos.x < epos.x;
}

//-------------------------------------------------------
// クラス定義
//-------------------------------------------------------
class Skill_バスター:public SkillData{
public:
	Skill_バスター(CPoint<int> atkPos, int damage, def::charType myCharType);
	~Skill_バスター();

	void Draw();
	void Process();
};

class Skill_ショックウェーブ:public SkillData{
	static const int IMAGE_DELAY = 4;// 次の画像に移るまでのカウント
	static const int NEXT_STEP_COUNT = 7 * IMAGE_DELAY;

	def::charType myCharType;
	int count;
	CPoint<int> atkPos;
	int image[7];
public:
	Skill_ショックウェーブ(CPoint<int> atkPos, int damage, def::charType myCharType);
	~Skill_ショックウェーブ();

	void Draw();
	void Process();
};

class Skill_キャノン:public SkillData{
	static const int DAMAGE_ENTER_COUNT = 5 * 4;// カウント後にダメージを登録

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

	Skill_キャノン(CPoint<int> atkPos, int damage, def::charType myCharType, int type);
	~Skill_キャノン();

	void Draw();
	void Process();
};

class Skill_サンダーボール:public SkillData{
	static const int MAX_MOVE_CNT = 8;// ボールが移動できる最大回数
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
	Skill_サンダーボール(CPoint<int> atkPos, int damage, def::charType myCharType);
	~Skill_サンダーボール();

	void Draw();
	void Process();
};

class Skill_リカバリー:public SkillData{
	static const int SKILL_DELAY = 2;// 次の画像に移るまでのカウント

	int count;
	CPoint<int> atkPos;
	int image[8];
public:
	Skill_リカバリー(CPoint<int> atkPos, int recover);
	~Skill_リカバリー();

	void Draw();
	void Process();
};

class Skill_ブーメラン:public SkillData{
	static const int IMAGE_DELAY = 4;// 次の画像に移るまでのカウント
	static const int NEXT_STEP_COUNT = 2 * IMAGE_DELAY;

	def::charType myCharType;
	int count;
	int image[4];
	CPoint<int> atkPos;
	int attackType;
	int muki;// 周回の時の向き(UPorDOWN)
public:
	enum{
		eTYPE_直線,
		eTYPE_周回
	};

	Skill_ブーメラン(CPoint<int> atkPos, int damage, def::charType myCharType, int type);
	~Skill_ブーメラン();

	void Draw();
	void Process();
};

class Skill_ヘルズローリング:public SkillData{
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
	static const int SKILL_DELAY = 3;// 次の画像に移るまでのカウント

	int count;
	CPoint<int> atkPos;
	CPoint<int> nextPos;
	CPoint<float> drawPos;
	int image[5];
	int moveDirectLR;
	CurveDirect curveDirect;// 一度だけ曲がる方向
	def::charType damageTargetType;
	CPoint<int> targetPos;// 攻撃対象の位置

						  // 今の描画位置を返す
	CPoint<float> GetDrawPos(CPoint<float> drawPos, CPoint<int> now, CPoint<int> dist, int maxCnt, CPoint<int> ofs){
		float dx = ( ( float ) BattleField::PANEL_SIZE.x*( dist.x - now.x ) ) / maxCnt;
		drawPos.x += dx;
		float dy = ( ( float ) BattleField::PANEL_SIZE.y*( dist.y - now.y ) ) / maxCnt;
		drawPos.y += dy;
		return drawPos;
	}
public:
	Skill_ヘルズローリング(CPoint<int> atkPos, int damage, int directLR, int directUD, def::charType myCharType);
	~Skill_ヘルズローリング();

	void Draw();
	void Process();
};

class Skill_ダークアームブレード:public SkillData{
	static const int SKILL_DELAY = 1;// 次の画像に移るまでのカウント
	static const int STOP_NUM = 5;

	int count;
	CPoint<int> atkPos;
	int image[4 + STOP_NUM];
	bool isShort;
public:
	Skill_ダークアームブレード(CPoint<int> atkPos, int damage, bool isShort, def::charType myCharType);
	~Skill_ダークアームブレード();

	void Draw();
	void Process();
};

class Skill_ミニボム:public SkillData{
protected:
	static const unsigned BOMB_ROTATE_DELAY = 4;
	static const unsigned EXPLODE_DRAW_DLAY = 3;
	static const unsigned SKILL_COUNT = 40;// ボムを投げてから着弾するまでのカウント

	CPoint<int> initPos;
	CPoint<float> atkPos;
	bool isView;
	int count;
	int imgBomb[5];
	int imgExplode[16];
	int explodeCnt;
public:
	Skill_ミニボム(CPoint<int> atkPos, int damage, def::charType myCharType);
	~Skill_ミニボム();

	void Draw();
	void Process();
};

class Skill_ソード_base:public SkillData{
protected:
	static const int SKILL_DELAY = 3;// 次の画像に移るまでのカウント

	CPoint<int> charPos;

	int count;
	int image[4];
public:
	Skill_ソード_base(CPoint<int> charPos, int damage, def::charType myCharType);
	~Skill_ソード_base();

	void Draw();
};

class Skill_ソード:public Skill_ソード_base{
public:
	Skill_ソード(CPoint<int> charPos, int damage, def::charType myCharType);
	~Skill_ソード();

	void Process();
};

class Skill_ワイドソード:public Skill_ソード_base{
public:
	Skill_ワイドソード(CPoint<int> charPos, int damage, def::charType myCharType);
	~Skill_ワイドソード();

	void Process();
};

class Skill_フレイムライン:public SkillData{
protected:
	static const int SKILL_DELAY = 4;// 次の画像に移るまでのカウント
	static const int BODY_DRAW_DELAY = 2;

	std::vector<CPoint<int>> atkPoses;
	CPoint<int> charPos;

	int count;
	int img_body[4];
	int img_fire[5];
public:
	Skill_フレイムライン(CPoint<int> charPos, int damage, def::charType myCharType);
	~Skill_フレイムライン();

	void Draw();
};

class Skill_フレイムライン_固定:public Skill_フレイムライン{
	static const int NEXT_STEP_COUNT = 20;
public:
	Skill_フレイムライン_固定(CPoint<int> charPos, int damage, def::charType myCharType);
	~Skill_フレイムライン_固定();

	void Process();
};

class Skill_シューティングバスター:public SkillData{
	static const int SKILL_DELAY = 3;// 次の画像に移るまでのカウント

	int count;
	CPoint<int> atkPos;
	int image[5];
public:
	Skill_シューティングバスター(CPoint<int> atkPos, int damage, def::charType myCharType);
	~Skill_シューティングバスター();

	void Draw();
	void Process();
};

//-------------------------------------------------------
// 外部関数
//-------------------------------------------------------
std::shared_ptr<SkillData> SkillMgr::GetData(int id, SkillArg args, def::charType myCharType){
	const std::string position = "SkillMgr::GetData ";

	switch( id ){
	case eID_バスター:
		return std::shared_ptr<SkillData>(new Skill_バスター(args.atkPos, args.damage, myCharType));
	case eID_ショックウェーブ:
		return std::shared_ptr<SkillData>(new Skill_ショックウェーブ(args.atkPos, args.damage, myCharType));
	case eID_キャノン:
		return std::shared_ptr<SkillData>(new Skill_キャノン(args.atkPos, args.damage, myCharType, Skill_キャノン::eTYPE_NORMAL));
	case eID_ハイキャノン:
		return std::shared_ptr<SkillData>(new Skill_キャノン(args.atkPos, args.damage, myCharType, Skill_キャノン::eTYPE_HIGH));
	case eID_サンダーボール:
		return std::shared_ptr<SkillData>(new Skill_サンダーボール(args.atkPos, args.damage, myCharType));
	case eID_リカバリー:
		return std::shared_ptr<SkillData>(new Skill_リカバリー(args.atkPos, args.recover));
	case eID_ブーメラン_周回:
		return std::shared_ptr<SkillData>(new Skill_ブーメラン(args.atkPos, args.damage, myCharType, Skill_ブーメラン::eTYPE_周回));
	case eID_ブーメラン_直線:
		return std::shared_ptr<SkillData>(new Skill_ブーメラン(args.atkPos, args.damage, myCharType, Skill_ブーメラン::eTYPE_直線));
	case eID_ヘルズローリング_UP:
	{
		int dirLR = -1;
		if( myCharType == def::eCHAR_PLAYER )
			dirLR = def::eMUKI_RIGHT;
		else if( myCharType == def::eCHAR_ENEMY )
			dirLR = def::eMUKI_LEFT;
		else{
			std::string msg = position + "ヘルズローリング";
			msg += "キャラタイプがおかしいです (" + ToString<int>(myCharType) + ")";
			ASSERT(0, msg);
		}
		return std::shared_ptr<SkillData>(new Skill_ヘルズローリング(args.atkPos, args.damage, dirLR, def::eMUKI_UP, myCharType));
	}
	case eID_ヘルズローリング_DOWN:
	{
		int dirLR = -1;
		if( myCharType == def::eCHAR_PLAYER )
			dirLR = def::eMUKI_RIGHT;
		else if( myCharType == def::eCHAR_ENEMY )
			dirLR = def::eMUKI_LEFT;
		else{
			std::string msg = position + "ヘルズローリング";
			msg += "キャラタイプがおかしいです (" + ToString<int>(myCharType) + ")";
			ASSERT(0, msg);
		}
		return std::shared_ptr<SkillData>(new Skill_ヘルズローリング(args.atkPos, args.damage, dirLR, def::eMUKI_DOWN, myCharType));
	}
	case eID_ダークアームブレード_SHORT:
		return std::shared_ptr<SkillData>(new Skill_ダークアームブレード(args.atkPos, args.damage, true, myCharType));
	case eID_ミニボム:
		return std::shared_ptr<SkillData>(new Skill_ミニボム(args.atkPos, args.damage, myCharType));
	case eID_ソード:
		return std::shared_ptr<SkillData>(new Skill_ソード(args.atkPos, args.damage, myCharType));
	case eID_ワイドソード:
		return std::shared_ptr<SkillData>(new Skill_ワイドソード(args.atkPos, args.damage, myCharType));
	case eID_シューティングバスター:
		return std::shared_ptr<SkillData>(new Skill_シューティングバスター(args.atkPos, args.damage, myCharType));
	case eID_フレイムライン_固定:
		return std::shared_ptr<SkillData>(new Skill_フレイムライン_固定(args.atkPos, args.damage, myCharType));
	default:
		ASSERT(0, position + "そのようなスキルは実装されていません (" + ToString<int>(id) + ")");
	}
	return std::shared_ptr<SkillData>();
}

std::shared_ptr<SkillData> SkillMgr::GetData(ChipData c, SkillArg args, def::charType myCharType){
	int id = -1;
	args.damage = c.power;
	switch( c.id ){
	case ChipMgr::eID_キャノン:
		id = eID_キャノン;
		break;
	case ChipMgr::eID_ハイキャノン:
		id = eID_ハイキャノン;
		break;
	case ChipMgr::eID_ストーンキューブ:
		id = eID_ストーンキューブ;
		break;
	case ChipMgr::eID_サンダーボール:
		id = eID_サンダーボール;
		break;
	case ChipMgr::eID_ソード:
		id = eID_ソード;
		break;
	case ChipMgr::eID_フレイムライン:
		id = eID_フレイムライン_固定;
		break;
	case ChipMgr::eID_リカバリー10:
	case ChipMgr::eID_リカバリー30:
		id = eID_リカバリー;
		args.damage = 0;
		args.recover = c.power;
		break;
	case ChipMgr::eID_ブーメラン:
		id = eID_ブーメラン_周回;
		args.atkPos = CPoint<int>(-1, BattleField::FIELD_NUM_Y - 1);// 左下から攻撃するように
		break;
	case ChipMgr::eID_ミニボム:
		id = eID_ミニボム;
		break;
	case ChipMgr::eID_ワイドソード:
		id = eID_ワイドソード;
		break;
	default:
		ASSERT(0, "SkillMgr::GetData チップとスキルの連動処理がありません (" + ToString<int>(id) + ")");
	}
	return GetData(id, args, myCharType);
}

//-------------------------------------------------------
// バスター
//-------------------------------------------------------
Skill_バスター::Skill_バスター(CPoint<int> atkPos, int damage, def::charType myCharType):SkillData(true, false, myCharType, damage){
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
		ASSERT(0, "Skill_バスター missing char type (" + ToString<int>(myCharType) + ")");
}

Skill_バスター::~Skill_バスター(){
}

void Skill_バスター::Draw(){
}

void Skill_バスター::Process(){
}

//-------------------------------------------------------
// ショックウェーブ
//-------------------------------------------------------
Skill_ショックウェーブ::Skill_ショックウェーブ(CPoint<int> atkPos, int damage, def::charType myCharType)
	:count(0), myCharType(myCharType), SkillData(false, true, myCharType, damage){
	std::string fname = def::SKILL_IMAGE_PATH + "ショックウェーブ.png";
	LoadDivGraphWithCheckError(image, fname, "Skill_ショックウェーブ::Skill_ショックウェーブ", 7, 1, 100, 140);

	if( myCharType == def::eCHAR_PLAYER )
		this->atkPos.x = atkPos.x + 1;// 目の前の一マス
	else
		this->atkPos.x = atkPos.x - 1;// 目の前の一マス
	this->atkPos.y = atkPos.y;

	SkillData::damageList.push_back(DamageData(this->atkPos, damagePower, myCharType, true, 1, SkillData::id, false));
}

Skill_ショックウェーブ::~Skill_ショックウェーブ(){
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ )
		DeleteGraph(image[i]);
}

void Skill_ショックウェーブ::Draw(){
	int ino = count / IMAGE_DELAY;
	int x = BattleField::PANEL_SIZE.x*atkPos.x;
	int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*atkPos.y - 100;
	DrawGraph(x, y, image[ino], TRUE);
}

void Skill_ショックウェーブ::Process(){
	if( count >= NEXT_STEP_COUNT ){// 一定カウント後一個次のマスへ
		count = 0;
		atkPos.x += ( myCharType == def::eCHAR_PLAYER ) ? 1 : -1;
		// debug(穴パネル)
		if( atkPos.x < 0 || atkPos.x >= BattleField::FIELD_NUM_X ){// 攻撃が範囲外に行けば
			delFlag = true;
			return;
		}

		// 攻撃の登録
		SkillData::damageList.push_back(DamageData(atkPos, damagePower, myCharType, true, NEXT_STEP_COUNT, SkillData::id, false));
	} else
		count++;
	BattleFieldMgr::GetInst()->AddDrawHitArea(atkPos);
}

//-------------------------------------------------------
// キャノン
//-------------------------------------------------------
Skill_キャノン::Skill_キャノン(CPoint<int> atkPos, int damage, def::charType myCharType, int type)
	:count(0), myCharType(myCharType), atkPos(atkPos), SkillData(true, false, myCharType, damage){
	int temp[9];
	std::string fname = def::SKILL_IMAGE_PATH + "キャノン_body.png";
	LoadDivGraphWithCheckError(temp, fname, "Skill_キャノン::Skill_キャノン", 3, 3, 46, 40);
	for( int i = 0, n = 0; i < 9; i++ ){
		if( ( i / 3 ) == type )
			imgBody[n++] = temp[i];
		else
			DeleteGraph(temp[i]);
	}

	// todo(波動画像)
}

Skill_キャノン::~Skill_キャノン(){
	for( int i = 0; i < sizeof(imgBody) / sizeof(imgBody[0]); i++ )
		DeleteGraph(imgBody[i]);
}

void Skill_キャノン::Draw(){
	int ino = ( count / 5 ) - 1;
	if( ino >= 0 ){
		if( ino > 2 )ino = 2;
		int dx = ( myCharType == def::eCHAR_PLAYER ) ? 65 : -10;
		int x = BattleField::PANEL_SIZE.x*atkPos.x + dx;
		int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*atkPos.y - 45;
		DrawGraph(x, y, imgBody[ino], TRUE);
	}
}

void Skill_キャノン::Process(){
	if( count == DAMAGE_ENTER_COUNT ){
		// 登録時間になったらダメージを登録
		int target = def::eCHAR_ALL^myCharType;
		int x = BattleCharMgr::GetInst()->GetTargetX(atkPos.y, target);
		if( x > 0 )// 直線上に敵がいるなら
			SkillData::damageList.push_back(DamageData(CPoint<int>(x, atkPos.y), damagePower, myCharType, true, 1, SkillData::id, false));
		delFlag = true;
	}
	count++;
}

//-------------------------------------------------------
// サンダーボール
//-------------------------------------------------------
Skill_サンダーボール::Skill_サンダーボール(CPoint<int> atkPos, int damage, def::charType myCharType)
	:count(0), myCharType(myCharType), SkillData(false, false, myCharType, damage)
	, moveCount(0)
{
	std::string fname = def::SKILL_IMAGE_PATH + "サンダーボール.png";
	LoadDivGraphWithCheckError(image, fname, "Skill_サンダーボール::Skill_サンダーボール", 4, 1, 64, 80);

	if( myCharType == def::eCHAR_PLAYER ){
		this->atkPos.x = atkPos.x + 1;// 目の前の一マス
		moveDirect = def::eMUKI_RIGHT;
		startPos.x = -BattleField::PANEL_SIZE.x / 2.f;
	} else{
		this->atkPos.x = atkPos.x - 1;// 目の前の一マス
		moveDirect = def::eMUKI_LEFT;
		startPos.x = BattleField::PANEL_SIZE.x / 2.f;
	}
	this->atkPos.y = atkPos.y;
	drawDiff = startPos;

	// ダメージ情報登録
	SkillData::damageList.push_back(DamageData(this->atkPos, damagePower, myCharType, true, NEXT_STEP_COUNT, SkillData::id, false));
}

Skill_サンダーボール::~Skill_サンダーボール(){
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ )
		DeleteGraph(image[i]);
}

void Skill_サンダーボール::Draw(){
	int ino = ( count / IMAGE_DELAY ) % 4;
	CPoint<int> ofs(5, -20);

	// 定位置
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

void Skill_サンダーボール::Process(){
	if( count == NEXT_STEP_COUNT / 2 ){
		// 現在地に一番近い敵を検索
		def::charType target = def::eCHAR_ALL;
		if( myCharType == def::eCHAR_PLAYER )
			target = def::eCHAR_ENEMY;
		else if( myCharType == def::eCHAR_ENEMY )
			target = def::eCHAR_PLAYER;
		else// プレイヤーか敵キャラ以外からサンダーボールが放たれることは今のところなし
			ASSERT(0, "Skill_サンダーボール::Process missing char type (" + ToString<int>(myCharType) + ")");
		CPoint<int> pos = BattleCharMgr::GetInst()->GetTargetPos(atkPos, target);

		// 移動処理(敵を追いかける)
		int x_dif = pos.x - atkPos.x;// 計算順序大事(方向決め)
		int y_dif = pos.y - atkPos.y;// 計算順序大事(方向決め)
		if( abs(x_dif) > abs(y_dif) ){// x方向に移動
			if( x_dif > 0 ){
				nextMoveDirect = def::eMUKI_RIGHT;
				startPos = CPoint<float>(-BattleField::PANEL_SIZE.x / 2.f, 0);
			} else{
				nextMoveDirect = def::eMUKI_LEFT;
				startPos = CPoint<float>(+BattleField::PANEL_SIZE.x / 2.f, 0);
			}

		} else{// y方向に移動
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
		// debug(穴パネル)

		if( atkPos.x < 0 || atkPos.x >= BattleField::FIELD_NUM_X ||
			atkPos.y < 0 || atkPos.y >= BattleField::FIELD_NUM_Y ||
			moveCount >= MAX_MOVE_CNT )
		{// 攻撃が範囲外に行けば or 最大移動回数に達したら
			delFlag = true;
			return;
		}

		// ダメージ情報登録
		SkillData::damageList.push_back(DamageData(atkPos, damagePower, myCharType, true, NEXT_STEP_COUNT, SkillData::id, false));
		moveCount++;
	}
	count++;
	BattleFieldMgr::GetInst()->AddDrawHitArea(atkPos);
}

//-------------------------------------------------------
// リカバリー
//-------------------------------------------------------
Skill_リカバリー::Skill_リカバリー(CPoint<int> atkPos, int recover)
	:count(0), atkPos(atkPos), SkillData(false, false, def::eCHAR_ALL, -recover)
{
	std::string fname = def::SKILL_IMAGE_PATH + "リカバリー.png";
	LoadDivGraphWithCheckError(image, fname, "Skill_リカバリー::Skill_リカバリー", 8, 1, 84, 144);
}

Skill_リカバリー::~Skill_リカバリー(){
	for( int i = 0; i < 8; i++ )
		DeleteGraph(image[i]);
}

void Skill_リカバリー::Draw(){
	int ino = ( count / SKILL_DELAY );
	if( ino > 7 )
		ino = 7;
	int x = BattleField::PANEL_SIZE.x*atkPos.x;
	int y = BattleField::PANEL_SIZE.y*atkPos.y + 25;
	DrawGraph(x, y, image[ino], TRUE);
}

void Skill_リカバリー::Process(){
	count++;
	if( count >= SKILL_DELAY * 8 ){
		// 自分の場所のキャラのHPを回復させる
		SkillData::damageList.push_back(DamageData(atkPos, damagePower, def::eCHAR_ALL, false, 1, SkillData::id, true));
		delFlag = true;
	}
}

//-------------------------------------------------------
// ブーメラン
//-------------------------------------------------------
Skill_ブーメラン::Skill_ブーメラン(CPoint<int> atkPos, int damage, def::charType myCharType, int type)
	:count(0), myCharType(myCharType), attackType(type), SkillData(false, true, myCharType, damage){
	std::string fname = def::SKILL_IMAGE_PATH + "ブーメラン.png";
	LoadDivGraphWithCheckError(image, fname, "Skill_ブーメラン::Skill_ブーメラン", 4, 1, 100, 80);

	if( myCharType == def::eCHAR_PLAYER )
		this->atkPos.x = atkPos.x + 1;// 目の前の一マス
	else
		this->atkPos.x = atkPos.x - 1;// 目の前の一マス
	this->atkPos.y = atkPos.y;
	if( atkPos.y == 0 )
		muki = def::eMUKI_DOWN;
	else if( atkPos.y == 2 )
		muki = def::eMUKI_UP;

	SkillData::damageList.push_back(DamageData(this->atkPos, damagePower, myCharType, true, 1, SkillData::id, false));
}

Skill_ブーメラン::~Skill_ブーメラン(){
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ )
		DeleteGraph(image[i]);
}

void Skill_ブーメラン::Draw(){
	int ino = ( count % 16 ) / 4;
	CPoint<int> ofs(-10, -20);
	int x = BattleField::PANEL_SIZE.x*atkPos.x + ofs.x;// 中心
	int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*atkPos.y + ofs.y;
	int xd = 0, yd = 0;
	switch( attackType ){
	case eTYPE_直線:
		xd = ( int ) ( ( double ) ( count%NEXT_STEP_COUNT )*BattleField::PANEL_SIZE.x / NEXT_STEP_COUNT ) - 50;
		if( myCharType != def::eCHAR_PLAYER )
			xd *= -1;
		break;
	case eTYPE_周回:
		if( ( ( myCharType == def::eCHAR_PLAYER ) && ( atkPos.x == BattleField::FIELD_NUM_X - 1 ) ) || ( ( myCharType != def::eCHAR_PLAYER ) && ( atkPos.x == 0 ) ) ){// 縦方向に回転
			yd = ( int ) ( ( double ) ( count%NEXT_STEP_COUNT )*BattleField::PANEL_SIZE.y / ( NEXT_STEP_COUNT * 2 ) );
			if( muki == def::eMUKI_UP )
				yd *= -1;
		} else{
			xd = ( int ) ( ( double ) ( count%NEXT_STEP_COUNT )*BattleField::PANEL_SIZE.x / NEXT_STEP_COUNT ) - 50;
			if( ( muki == def::eMUKI_UP&&atkPos.y == 0 ) || ( muki == def::eMUKI_DOWN&&atkPos.y == BattleField::FIELD_NUM_Y - 1 ) )// 帰り道なら
				xd *= -1;
			if( myCharType != def::eCHAR_PLAYER )
				xd *= -1;
		}
		break;
	}
	DrawGraph(x + xd, y + yd, image[ino], TRUE);
}

void Skill_ブーメラン::Process(){
	if( count%NEXT_STEP_COUNT == NEXT_STEP_COUNT - 1 ){// 一定カウント後一個次のマスへ
		switch( attackType ){
		case eTYPE_直線:
			atkPos.x += ( myCharType == def::eCHAR_PLAYER ) ? 1 : -1;
			break;
		case eTYPE_周回:
			ASSERT(muki == def::eMUKI_UP || muki == def::eMUKI_DOWN, "Skill_ブーメラン::Process wrong direction (" + ToString<int>(muki) + ")");
			if( 1 <= atkPos.x&&atkPos.x < BattleField::FIELD_NUM_X - 1 ){
				int val = ( atkPos.y == 0 ) ? -1 : 1;
				if( muki == def::eMUKI_DOWN )// 下に向かっていく時なら
					val *= -1;
				if( myCharType != def::eCHAR_PLAYER )// プレイヤーじゃなければ逆方向
					val *= -1;
				atkPos.x += val;
			} else{
				if( ( ( myCharType == def::eCHAR_PLAYER ) && ( atkPos.x == 0 ) ) || ( ( myCharType != def::eCHAR_PLAYER ) && ( atkPos.x == BattleField::FIELD_NUM_X - 1 ) ) ){// 終了位置なら
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
					} else// 縦方向の中間地点
						atkPos.y += ( muki == def::eMUKI_UP ) ? -1 : 1;
				}
			}
			break;
		}

		if( atkPos.x < 0 || atkPos.x >= BattleField::FIELD_NUM_X ){// 攻撃が範囲外に行けば
			delFlag = true;
			return;
		}

		SkillData::damageList.push_back(DamageData(atkPos, damagePower, myCharType, true, 1, SkillData::id, false));
	}
	count++;
}

//-------------------------------------------------------
// ヘルズローリング
//-------------------------------------------------------
Skill_ヘルズローリング::Skill_ヘルズローリング(CPoint<int> atkPos, int damage, int directLR, int directUD, def::charType myCharType)
	:count(0), SkillData(false, true, myCharType, damage), moveDirectLR(directLR), atkPos(atkPos)
{
	std::string msg = "Skill_ヘルズローリング wrong direction ";
	ASSERT(directLR == def::eMUKI_RIGHT || directLR == def::eMUKI_LEFT, msg + "directLR = " + ToString<int>(directLR));
	ASSERT(directUD == def::eMUKI_UP || directUD == def::eMUKI_DOWN, msg + "directUD = " + ToString<int>(directUD));

	int t[10];
	std::string fname = def::SKILL_IMAGE_PATH + "ヘルズローリング.png";
	LoadDivGraphWithCheckError(t, fname, "Skill_ヘルズローリング::Skill_ヘルズローリング", 5, 2, 128, 164);
	for( int i = 0; i < 5; i++ ){
		image[4 - i] = t[5 + i];
		DeleteGraph(t[i]);
	}

	if( myCharType == def::eCHAR_ENEMY )
		damageTargetType = def::eCHAR_PLAYER;
	else if( myCharType == def::eCHAR_PLAYER )
		damageTargetType = def::eCHAR_ENEMY;
	else{
		ASSERT(0, "Skill_ヘルズローリング missing char type (" + ToString<int>(myCharType) + ")");
	}

	// 最初は自分のマス、次は斜め 上or下
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

Skill_ヘルズローリング::~Skill_ヘルズローリング(){
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ )
		DeleteGraph(image[i]);
}

void Skill_ヘルズローリング::Draw(){
	int ino = ( count / SKILL_DELAY );
	if( ino > 4 )
		ino = 3 + ( ino / 3 ) % 2;

	drawPos = GetDrawPos(drawPos, atkPos, nextPos, NEXT_STEP_COUNT, CPoint<int>(-20, -90));
	if( moveDirectLR == def::eMUKI_RIGHT )
		DrawTurnGraphF(drawPos.x, drawPos.y, image[ino], TRUE);
	else
		DrawGraphF(drawPos.x, drawPos.y, image[ino], TRUE);

	// debug(影つけたい)
}

void Skill_ヘルズローリング::Process(){
	count++;
	if( count%NEXT_STEP_COUNT == 0 ){
		// 基本まっすぐ、一度だけ敵方向へ曲がる
		atkPos = nextPos;
		nextPos.x += ( moveDirectLR == def::eMUKI_RIGHT ) ? +1 : -1;
		if( !curveDirect.IsSet() ){
			int x = abs(targetPos.x - atkPos.x);
			int y = abs(targetPos.y - atkPos.y);
			if( ( x == 1 && y == 1 ) || ( x == 2 && y == 2 ) )
				curveDirect.SetValue(targetPos.y - atkPos.y);
			nextPos.y += curveDirect.GetAddValue();
		} else
			nextPos.y += curveDirect.GetAddValue();// 上下に曲がり続ける

		if( atkPos.x < 0 || atkPos.x>5 || atkPos.y < 0 || atkPos.y>2 )
			delFlag = true;

		if( !delFlag ){
			// ダメージ情報登録
			SkillData::damageList.push_back(DamageData(atkPos, damagePower, myCharType, true, NEXT_STEP_COUNT, SkillData::id, true));
		}
	}
}

//-------------------------------------------------------
// ダークアームブレード
//-------------------------------------------------------
Skill_ダークアームブレード::Skill_ダークアームブレード(CPoint<int> atkPos, int damage, bool isShort, def::charType myCharType)
	:count(0), atkPos(atkPos), isShort(isShort), SkillData(true, true, myCharType, damage)
{
	int tmp[8];
	std::string fname = def::SKILL_IMAGE_PATH + "ダークアームブレード.png";
	LoadDivGraphWithCheckError(tmp, fname, "Skill_ダークアームブレード::Skill_ダークアームブレード", 4, 2, 188, 150);

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

Skill_ダークアームブレード::~Skill_ダークアームブレード(){
	for( int i = 0; i < sizeof(image) / sizeof(image[0]); i++ ){
		DeleteGraph(image[i]);
		image[i] = -1;
	}
}

void Skill_ダークアームブレード::Draw(){
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

void Skill_ダークアームブレード::Process(){
	count++;
	if( count == SKILL_DELAY * 1 ){
		// ダメージの登録
		SkillData::damageList.push_back(DamageData(atkPos, damagePower, myCharType, true, 1, SkillData::id, true));
	}
	if( count >= SKILL_DELAY * ( 3 + STOP_NUM ) )
		delFlag = true;
}

//-------------------------------------------------------
// ミニボム
//-------------------------------------------------------
Skill_ミニボム::Skill_ミニボム(CPoint<int> atkPos, int damage, def::charType myCharType)
	:SkillData(true, false, myCharType, damage), count(0), initPos(atkPos)
	, isView(false), explodeCnt(0)
{
	ASSERT(myCharType == def::eCHAR_PLAYER, "Skill_ミニボム missing char type (" + ToString<int>(myCharType) + ")");

	std::string fname = def::SKILL_IMAGE_PATH + "ミニボム.png";
	LoadDivGraphWithCheckError(imgBomb, fname, "Skill_ミニボム", 5, 1, 40, 30);

	fname = def::IMAGE_FILE_PATH + "explode.png";
	LoadDivGraphWithCheckError(imgExplode, fname, "Skill_ミニボム", 8, 2, 110, 124);
}

Skill_ミニボム::~Skill_ミニボム(){
	for( int i = 0; i < 5; i++ )
		DeleteGraph(imgBomb[i]);
	for( int i = 0; i < 16; i++ )
		DeleteGraph(imgExplode[i]);
}

void Skill_ミニボム::Draw(){
	if( isView ){
		int ino = ( count / BOMB_ROTATE_DELAY ) % 4;
		int x = BattleField::PANEL_SIZE.x*initPos.x + 20 + ( int ) atkPos.x;
		int y = BattleField::PANEL_SIZE.y*initPos.y + 170 + ( int ) atkPos.y;
		DrawGraph(x, y, imgBomb[ino], TRUE);
	}

	// 落ちた時にボム処理
	if( explodeCnt > 0 ){
		int ino = ( explodeCnt / EXPLODE_DRAW_DLAY ) * 2;
		if( ino > 15 )
			ino = 15;
		int x = BattleField::PANEL_SIZE.x*( initPos.x + 3 ) - 10;
		int y = BattleField::PANEL_SIZE.y*initPos.y + 90;
		DrawGraph(x, y, imgExplode[ino], TRUE);
	}
}

void Skill_ミニボム::Process(){
	count++;

	if( explodeCnt == 0 ){
		// 放物線の描画
		atkPos.x += ( BattleField::PANEL_SIZE.x*3.f / SKILL_COUNT );
		if( atkPos.x > BattleField::PANEL_SIZE.x / 2 )
			isView = true;
		if( atkPos.x > BattleField::PANEL_SIZE.x*3.f ){
			isView = false;
			explodeCnt = 1;
			// todo(不発の時はdelFlag=true)

			// ダメージの登録
			CPoint<int> pos = initPos;
			pos.x += 3;// todo(charTypeによって+-を反転)
			SkillData::damageList.push_back(DamageData(pos, damagePower, myCharType, true, 1, SkillData::id, false));
		}
		atkPos.y = atkPos.x*( ( atkPos.x / 120 ) - 2 );// y = x^2/120 - 2x
	} else{
		explodeCnt++;
		if( explodeCnt > EXPLODE_DRAW_DLAY * 15 / 2 )
			delFlag = true;
	}
}

Skill_ソード_base::Skill_ソード_base(CPoint<int> charPos, int damage, def::charType myCharType)
	:count(0), charPos(charPos), SkillData(true, true, myCharType, damage)
{
	for( int i = 0; i < 4; i++ )
		image[i] = -1;
}

Skill_ソード_base::~Skill_ソード_base(){
	for( int i = 0; i < 4; i++ )
		DeleteGraph(image[i]);
}

void Skill_ソード_base::Draw(){
	int ino = ( count / SKILL_DELAY );
	if( ino > 4 )
		ino = 4;
	int x = BattleField::PANEL_SIZE.x*charPos.x + 40;
	int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*charPos.y - 80;
	if( image[ino] != -1 ){
		ASSERT(myCharType == def::eCHAR_PLAYER, "Skill_ソード_base::Draw missing char type (" + ToString<int>(myCharType) + ")");
		DrawGraph(x, y, image[ino], TRUE);
	}
}

//-------------------------------------------------------
// ソード
//-------------------------------------------------------
Skill_ソード::Skill_ソード(CPoint<int> charPos, int damage, def::charType myCharType)
	:Skill_ソード_base(charPos, damage, myCharType)
{
	std::string fname = def::SKILL_IMAGE_PATH + "ソード.png";
	int t[12];
	LoadDivGraphWithCheckError(t, fname, "Skill_ソード", 4, 3, 160, 150);
	for( int i = 0; i < 4; i++ ){
		image[i] = t[i];
		DeleteGraph(t[i + 4]);
		DeleteGraph(t[i + 8]);
	}
}

Skill_ソード::~Skill_ソード(){
}

void Skill_ソード::Process(){
	count++;
	if( count == SKILL_DELAY * 1 ){
		// 目の前のマスにダメージの登録
		CPoint<int> pos = charPos;
		if( myCharType == def::eCHAR_PLAYER )
			pos.x += 1;
		else if( myCharType == def::eCHAR_ENEMY )
			pos.x--;
		else
			AppLogger::Error("Skill_ソード: ソードを使用するキャラタイプがおかしいです charType = " + ToString<int>(myCharType));
		SkillData::damageList.push_back(DamageData(pos, damagePower, myCharType, true, 1, SkillData::id, true));
	}
	if( count >= SKILL_DELAY * 4 )
		delFlag = true;
}

//-------------------------------------------------------
// ワイドソード
//-------------------------------------------------------
Skill_ワイドソード::Skill_ワイドソード(CPoint<int> charPos, int damage, def::charType myCharType)
	:Skill_ソード_base(charPos, damage, myCharType)
{
	std::string fname = def::SKILL_IMAGE_PATH + "ソード.png";
	int t[12];
	LoadDivGraphWithCheckError(t, fname, "Skill_ソード", 4, 3, 160, 150);
	for( int i = 0; i < 4; i++ ){
		image[i] = t[i + 8];
		DeleteGraph(t[i]);
		DeleteGraph(t[i + 4]);
	}
}

Skill_ワイドソード::~Skill_ワイドソード(){
}

void Skill_ワイドソード::Process(){
	count++;
	if( count == SKILL_DELAY * 1 ){
		// 目の前の3マスにダメージの登録
		CPoint<int> pos = charPos;
		if( myCharType == def::eCHAR_PLAYER )
			pos.x += 1;
		else if( myCharType == def::eCHAR_ENEMY )
			pos.x--;
		else
			AppLogger::Error("Skill_ソード: ソードを使用するキャラタイプがおかしいです charType = " + ToString<int>(myCharType));
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

Skill_シューティングバスター::Skill_シューティングバスター(CPoint<int> atkPos, int damage, def::charType myCharType)
	:SkillData(false, true, myCharType, damage), count(0), atkPos(atkPos)
{
	std::string fname = def::SKILL_IMAGE_PATH + "シューティングバスター.png";
	LoadDivGraphWithCheckError(image, fname, "Skill_シューティングバスター", 5, 1, 112, 96);
}

Skill_シューティングバスター::~Skill_シューティングバスター(){
	for( int i = 0; i < 5; i++ )
		DeleteGraph(image[i]);
}

void Skill_シューティングバスター::Draw(){
	int ino = ( count / SKILL_DELAY );
	if( ino > 4 )
		ino = 4;
	int x = BattleField::PANEL_SIZE.x*atkPos.x - 15;
	int y = BattleField::PANEL_SIZE.y*atkPos.y + 100;
	DrawGraph(x, y, image[ino], TRUE);
}

void Skill_シューティングバスター::Process(){
	count++;
	if( count == SKILL_DELAY * 1 ){
		SkillData::damageList.push_back(DamageData(atkPos, damagePower, myCharType, true, 1, SkillData::id, true));
	}

	if( count >= SKILL_DELAY * 5 )
		delFlag = true;
	BattleFieldMgr::GetInst()->AddDrawHitArea(atkPos);
}

Skill_フレイムライン::Skill_フレイムライン(CPoint<int> charPos, int damage, def::charType myCharType)
	:count(0), SkillData(false, true, myCharType, damage), charPos(charPos)
{
	std::string fname = def::SKILL_IMAGE_PATH + "フレイムライン_火柱.png";
	LoadDivGraphWithCheckError(img_fire, fname, "Skill_フレイムライン", 5, 1, 72, 120);
	if( myCharType == def::eCHAR_PLAYER ){// プレイヤーならbodyも描画
		// 画像読み込み
		fname = def::SKILL_IMAGE_PATH + "フレイムライン_body.png";
		LoadDivGraphWithCheckError(img_body, fname, "Skill_フレイムライン", 4, 1, 64, 64);
	} else{
		for( int i = 0; i < 4; i++ )
			img_body[i] = -1;
	}
}

Skill_フレイムライン::~Skill_フレイムライン(){
	for( int i = 0; i < 4; i++ )
		DeleteGraph(img_body[i]);
	for( int i = 0; i < 5; i++ )
		DeleteGraph(img_fire[i]);
}

void Skill_フレイムライン::Draw(){
	int ino = ( count / SKILL_DELAY );
	if( ino > 4 )
		ino = 4;
	for( std::vector<CPoint<int>>::iterator it = atkPoses.begin(); it != atkPoses.end(); it++ ){
		int x = BattleField::PANEL_SIZE.x*it->x + 7;
		int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*it->y -70;
		DrawGraph(x, y, img_fire[ino], TRUE);
	}

	// bodyを描画するなら
	if( img_body[0] != -1 ){
		ino = ( count / BODY_DRAW_DELAY );
		if( ino > 3 )
			ino = 3;
		int x = BattleField::PANEL_SIZE.x*charPos.x + 60;
		int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*charPos.y -60;
		if( myCharType == def::eCHAR_PLAYER ){
			// 攻撃ボディの描画
			DrawGraph(x, y, img_body[ino], TRUE);
		} else{
			// todo(敵の攻撃時のオフセットを決める)
			ASSERT(0);// 未実装
		}
	}
}

Skill_フレイムライン_固定::Skill_フレイムライン_固定(CPoint<int> charPos, int damage, def::charType myCharType)
	:Skill_フレイムライン(charPos,damage,myCharType)
{
	CPoint<int> tp;
	// 登録の順番(奥から手前に)
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

Skill_フレイムライン_固定::~Skill_フレイムライン_固定(){
}

void Skill_フレイムライン_固定::Process(){
	count++;
	if( count >= 40 )// debug
		delFlag = true;
}
