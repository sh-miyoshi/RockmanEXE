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

		// 爆破画像を登録
		if( deleteCount == 1 ){
			int x = BattleField::PANEL_SIZE.x*pos.x + BattleField::PANEL_SIZE.x / 2;
			int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*pos.y - 10;// ふちの分だけ上げる
			EffectMgr::GetInst()->Register(EffectMgr::eID_EXPLODE,CPoint<int>(x,y));
		}
	}
};

class Enemy_メットール:public EnemyBase{
	static const int Power_ショックウェーブ = 10;// debug(固定？)

	class ActMove:public AnimProcess{
		Enemy_メットール *obj;
	public:
		ActMove(Enemy_メットール *obj):obj(obj){}

		void Begin(){
			int y = BattleCharMgr::GetInst()->GetTargetPos(obj->pos, def::eCHAR_PLAYER).y;
			if( y < obj->pos.y )// ロックマンが上にいるなら
				obj->Move(def::eMUKI_UP, true);
			else if( y > obj->pos.y )
				obj->Move(def::eMUKI_DOWN, true);
		}
	};
	class Act_ショックウェーブ:public AnimProcess{
		bool regdFlag;
		int count;
		Enemy_メットール *obj;
	public:
		Act_ショックウェーブ(Enemy_メットール *obj):obj(obj), count(0), regdFlag(false){}

		void Begin(){
			regdFlag = false;
			count = 0;
		}

		void Process(){
			const int timing = 12 * obj->anim[ANIM_ATK1].imageDelay;
			if( count >= timing && !regdFlag ){// 攻撃を行うタイミングになったら
				// 攻撃の登録
				SkillArg args;
				args.atkPos = obj->pos;
				args.damage = Power_ショックウェーブ;
				BattleCharMgr::GetInst()->RegisterAttack(SkillMgr::GetData(SkillMgr::eID_ショックウェーブ, args, def::eCHAR_ENEMY));
				regdFlag = true;
			}
			count++;
		}
	};
public:
	Enemy_メットール();
	~Enemy_メットール();

	void Process();
	void LoadAnim();
};

class Enemy_ビリー:public EnemyBase{
	static const int Power_サンダーボール = 20;// debug(固定？)

	class ActMove:public AnimProcess{
		Enemy_ビリー *obj;
	public:
		ActMove(Enemy_ビリー *obj):obj(obj){}

		void Begin(){
			// 方向をランダムで決める
			int direct = -1;
			switch( rand() % 4 ){
			case 0: direct = def::eMUKI_UP; break;
			case 1: direct = def::eMUKI_DOWN; break;
			case 2: direct = def::eMUKI_RIGHT; break;
			case 3: direct = def::eMUKI_LEFT; break;
			default: ASSERT(0,ToString("Enemy_ビリー::ActMove::Begin illegal direction (%d)",direct));// 起こりえない
			}
			obj->Move(direct, true);
		}
	};
	class Act_サンダーボール:public AnimProcess{
		bool regdFlag;
		int count;
		Enemy_ビリー *obj;
	public:
		Act_サンダーボール(Enemy_ビリー *obj):obj(obj){}

		void Begin(){
			regdFlag = false;
			count = 0;
		}

		void Process(){
			const int timing = 7 * obj->anim[ANIM_ATK1].imageDelay;
			if( count >= timing && !regdFlag ){// 攻撃を行うタイミングになったら
				// 攻撃の登録
				SkillArg args;
				args.atkPos = obj->pos;
				args.damage = Power_サンダーボール;
				BattleCharMgr::GetInst()->RegisterAttack(SkillMgr::GetData(SkillMgr::eID_サンダーボール, args , def::eCHAR_ENEMY));
				regdFlag = true;
			}
			count++;
		}
	};
public:
	Enemy_ビリー();
	~Enemy_ビリー();

	void Process();
	void LoadAnim();
};

// todo(移動時の無敵時間が変)
// todo(ダークアームブレードよけれん)
class Enemy_フォルテ:public EnemyBase{
	class ActMove:public AnimProcess{
		Enemy_フォルテ *obj;
	public:
		ActMove(Enemy_フォルテ *obj):obj(obj){}

		void Begin(){
			CPoint<int> pos = obj->moveTargetPos.front();
			obj->moveTargetPos.pop();
			obj->isMoveSuccess = obj->MoveCheck(pos.x, pos.y, false);
			if( obj->isMoveSuccess ){
				obj->pos = pos;
			}
		}
	};
	class Act_シューティングバスター:public AnimProcess{
		static const int ATK_DELAY = 15;
		static const int ATK_WAIT_TIME = 30;

		Enemy_フォルテ *obj;
		int count;
		CPoint<int> atkPos;
		int atkNum;
	public:
		static const int ATTACK_NUM = 15;
		static const int Power_シューティングバスター = 40;// debug(ダメージ)

		Act_シューティングバスター(Enemy_フォルテ *obj):obj(obj){}
		void Begin(){
			count = 0;
			atkNum = 0;

			// 初期のロックマンの位置の中からランダムで選択
			atkPos.x = rand() % BattleField::FIELD_NUM_X / 2;
			atkPos.y = rand() % BattleField::FIELD_NUM_Y;
		}

		void Process(){
			count++;
			if( count < ATK_WAIT_TIME )// 一定時間待つ
				return;

			if( count % ATK_DELAY == ATK_DELAY - 1 ){
				// 攻撃の登録
				SkillArg args;
				args.atkPos = atkPos;
				args.damage = Power_シューティングバスター;
				std::shared_ptr<SkillData> skill = SkillMgr::GetData(SkillMgr::eID_シューティングバスター, args, def::eCHAR_ENEMY);
				BattleCharMgr::GetInst()->RegisterAttack(skill);

				atkPos.x = rand() % BattleField::FIELD_NUM_X / 2;
				atkPos.y = rand() % BattleField::FIELD_NUM_Y;
				atkNum++;
			}
		}
	};
	class Act_ヘルズローリング:public AnimProcess{
		static const int POWER_ヘルズローリング = 40;// debug(ダメージ量)
		static const int IMAGE_DELAY = 8;

		Enemy_フォルテ *obj;
		int count;
		int drawCount;
		bool drawFlag;
	public:
		Act_ヘルズローリング(Enemy_フォルテ *obj)
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
				int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*obj->pos.y - 10;// ふちの分だけ上げる
				int no = obj->imgヘルズローリング[drawCount / IMAGE_DELAY];
				DrawRotaGraph(x - 50, y - 30, 1, 0, no, TRUE);
			}
		}

		void Process(){
			if( count == 45 ){
				// 攻撃の登録
				SkillArg args;
				args.atkPos = obj->pos;
				args.damage = POWER_ヘルズローリング;
				std::shared_ptr<SkillData> skill = SkillMgr::GetData(SkillMgr::eID_ヘルズローリング_UP, args, def::eCHAR_ENEMY);
				BattleCharMgr::GetInst()->RegisterAttack(skill);
				drawFlag = true;
			} else if( count == 75 ){
				// 攻撃の登録
				SkillArg args;
				args.atkPos = obj->pos;
				args.damage = POWER_ヘルズローリング;
				std::shared_ptr<SkillData> skill = SkillMgr::GetData(SkillMgr::eID_ヘルズローリング_DOWN, args, def::eCHAR_ENEMY);
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
	class Act_ダークアームブレード:public AnimProcess{
		static const int POWER_ダークアームブレード = 40;// debug(ダメージ量)

		Enemy_フォルテ *obj;
	public:
		Act_ダークアームブレード(Enemy_フォルテ *obj):obj(obj){}

		void Begin(){
			// 攻撃の登録
			CPoint<int> pos = obj->pos;
			pos.x--;// todo(後ろからの攻撃)
			SkillArg args;
			args.atkPos = pos;
			args.damage = POWER_ダークアームブレード;
			std::shared_ptr<SkillData> skill = SkillMgr::GetData(SkillMgr::eID_ダークアームブレード_SHORT, args, def::eCHAR_ENEMY);
			BattleCharMgr::GetInst()->RegisterAttack(skill);
		}
	};

	std::queue<CPoint<int>> moveTargetPos;
	CPoint<int> prevPos;
	int nextMoveNum;
	bool isMoveSuccess;
	int imgヘルズローリング[3];

	CPoint<int> GetMoveTargetPos();
	void Attack_シューティングバスター();
	void Attack_ヘルズローリング();
	void Attack_ダークアームブレード_1();
	void Attack_ダークアームブレード_3();
	void Attack_ダークネスオーバーロード();
public:
	Enemy_フォルテ();
	~Enemy_フォルテ();

	void Process();
	void LoadAnim();
	virtual void DeleteAnim();
	virtual void Draw();
};

//-------------------------------------------------------
// 外部関数
//-------------------------------------------------------
std::shared_ptr<BattleCharBase> EnemyMgr::GetData(int id){
	switch( id ){
	case ID_メットール:
		return std::shared_ptr<BattleCharBase>(new Enemy_メットール());
	case ID_ビリー:
		return std::shared_ptr<BattleCharBase>(new Enemy_ビリー());
	case ID_フォルテ:
		return std::shared_ptr<BattleCharBase>(new Enemy_フォルテ());
	default:
		ASSERT(0,ToString("EnemyMgr::GetData wrong char id (%d)",id));
	}
	return std::shared_ptr<BattleCharBase>();
}

std::list<EnemyMgr::EnemyAttackChip_t> EnemyMgr::GetEnemyChip(int id){
	static std::list<EnemyAttackChip_t> result;
	EnemyAttackChip_t t;
	t.charID = id;

	// 各キャラごとにチップ情報をセット
	switch( id ){
	case ID_ビリー:
		t.chip.id = ChipMgr::eID_サンダーボール;
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
// メットール
//-------------------------------------------------------
Enemy_メットール::Enemy_メットール():EnemyBase("メットール", 40){
	anim[ANIM_ATK1].imageDelay = 4;// ショックウェーブモーションのディレイ

							 // アニメーションの設定
	anim[ANIM_MOVE].animProc = std::shared_ptr<ActMove>(new ActMove(this));
	anim[ANIM_ATK1].animProc = std::shared_ptr<Act_ショックウェーブ>(new Act_ショックウェーブ(this));
}

Enemy_メットール::~Enemy_メットール(){
}

void Enemy_メットール::Process(){
	EnemyBase::Process();
	// デリートモーション中(操作不可)
	if( hp <= 0 ){
		return;
	}

	static const int WAIT_COUNT = 1 * 60;// 最初の何もしないカウント
	static const int MOVE_COUNT = 1 * 60;
	static const int ATK1_COUNT = 30;

	//-------------------------------
	// 最初のnカウント:	何もしない
	// 以降ループ
	//	プレイヤー探索
	//	if y座標が同じ&一定時間
	//		攻撃登録
	//-------------------------------
	if( AnimProcess() ){
		if( count >= WAIT_COUNT ){
			int y = BattleCharMgr::GetInst()->GetTargetPos(pos, def::eCHAR_PLAYER).y;// ロックマンの位置
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

void Enemy_メットール::LoadAnim(){
	static const std::string position = "Enemy_メットール:LoadAnim";

	std::string fname = def::ENEMY_IMAGE_PATH + "メットール_move.png";
	LoadDivGraphWithCheckError(anim[ANIM_MOVE].image, fname, position, 1, 1, 100, 100);
	anim[ANIM_MOVE].imageNum = 1;

	fname = def::ENEMY_IMAGE_PATH + "メットール_atk.png";
	LoadDivGraphWithCheckError(anim[ANIM_ATK1].image, fname, position, 15, 1, 100, 140);
	anim[ANIM_ATK1].imageNum = 15;
}

//-------------------------------------------------------
// ビリー
//-------------------------------------------------------
Enemy_ビリー::Enemy_ビリー():EnemyBase("ビリー", 50){
	anim[ANIM_ATK1].imageDelay = 6;// debug(適当)

							 // アニメーションの設定
	anim[ANIM_MOVE].animProc = std::shared_ptr<ActMove>(new ActMove(this));
	anim[ANIM_ATK1].animProc = std::shared_ptr<Act_サンダーボール>(new Act_サンダーボール(this));
}

Enemy_ビリー::~Enemy_ビリー(){
}

void Enemy_ビリー::Process(){
	EnemyBase::Process();
	// デリートモーション中(操作不可)
	if( hp <= 0 ){
		return;
	}

	static const int WAIT_COUNT = 90;// 最初の何もしないカウント
	static const int ACT_COUNT = 1 * 60;

	//-------------------------------
	// 最初のnカウント:	何もしない
	// 以降ループ
	//	4/5: 適当に移動
	//	1/5: 攻撃
	//-------------------------------

	if( AnimProcess() ){
		if( count >= WAIT_COUNT ){
			if( count%ACT_COUNT == 0 ){// 行動する
				if( atkCount < 4 ){//移動処理
					SetAnim(ANIM_MOVE);
					atkCount++;
				} else{// 攻撃処理
					// todo(自分の攻撃が存在しないなら、BattleMgrから強制モーション設定で回避)
					SetAnim(ANIM_ATK1);
					atkCount = 0;
					count = 0;
				}
			}
		}
		count++;
	}
}

void Enemy_ビリー::LoadAnim(){
	static const std::string position = "Enemy_ビリー:LoadAnim";
	int temp[8 * 3];
	std::string fname = def::ENEMY_IMAGE_PATH + "ビリー.png";
	LoadDivGraphWithCheckError(temp, fname, position, 8, 3, 112, 114);
	for( int i = 0; i < 8; i++ )// 最初の8個は使わない
		DeleteGraph(temp[i]);
	for( int i = 0; i < 6; i++ )// 次の6個は移動
		anim[ANIM_MOVE].image[i] = temp[i + 8];
	DeleteGraph(temp[6 + 8]);
	DeleteGraph(temp[7 + 8]);
	for( int i = 0; i < 8; i++ )// 最後の8個は攻撃
		anim[ANIM_ATK1].image[i] = temp[i + 16];
	anim[ANIM_MOVE].imageNum = 6;
	anim[ANIM_ATK1].imageNum = 8;
}

//-------------------------------------------------------
// フォルテ
//-------------------------------------------------------
Enemy_フォルテ::Enemy_フォルテ()
	:EnemyBase("フォルテ", 1000), prevPos(-1, -1), nextMoveNum(1)
{
	// アニメーションの設定
	anim[ANIM_MOVE].animProc = std::shared_ptr<ActMove>(new ActMove(this));
	anim[ANIM_ATK1].animProc = std::shared_ptr<Act_シューティングバスター>(new Act_シューティングバスター(this));
	anim[ANIM_ATK2].animProc = std::shared_ptr<Act_ヘルズローリング>(new Act_ヘルズローリング(this));
	anim[ANIM_ATK3].animProc = std::shared_ptr<Act_ダークアームブレード>(new Act_ダークアームブレード(this));

	anim[ANIM_MOVE].imageDelay = 2;
	anim[ANIM_ATK1].imageDelay = 6;
	anim[ANIM_ATK2].imageDelay = 6;
	anim[ANIM_ATK3].imageDelay = 2;

	drawPosOffset.y = -30;
}

Enemy_フォルテ::~Enemy_フォルテ(){
}

void Enemy_フォルテ::Draw(){
	// 定位置座標
	int x = BattleField::PANEL_SIZE.x*pos.x + BattleField::PANEL_SIZE.x / 2;
	int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*pos.y - 10;// ふちの分だけ上げる
	int ino = -1;

	if( hp <= 0 ){
		SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
		int image = anim[ANIM_DAMAGE].image[0];
		// todo(アニメーションの追加)
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
			ty = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*ty / ( anim[ANIM_MOVE].imageNum - 1 ) - 10;// ふちの分だけ上げる
			ino = anim[ANIM_MOVE].image[anim[ANIM_MOVE].imageNum - 1 - i];
			DrawRotaGraph(tx, ty + drawPosOffset.y, 1, 0, ino, TRUE);
		}
	}
	break;
	case ANIM_ATK1:// シューティングバスター
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

void Enemy_フォルテ::Process(){
	EnemyBase::Process();
	// デリートモーション中(操作不可)
	if( hp <= 0 ){
		return;
	}

	static const int WAIT_COUNT = 30;

	if( AnimProcess() ){
		// 行動失敗時は予約をすべて消す
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
				// 移動処理
				nextMoveNum--;
				moveTargetPos.push(GetMoveTargetPos());
				prevPos = pos;
				SetAnim(ANIM_MOVE);
			} else{
				nextMoveNum = 3 + rand() % 3;
				// 攻撃処理(HPの減り具合から乱数で攻撃決定)
				// シューティングバスター(S),ヘルズローリング(H),ダークアームブレード(D1or3),ダークネスオーバーロード(O)
				// HP: MAX～1/2 -> D1(60%), D3(20%), H(10%), S(10%)
				// HP: 1/2～1/4 -> D1(30%), D3(20%), H(20%), S(25%), O(5%)
				// HP: 1/4～0   -> D1(10%), D3(25%), H(20%), S(35%), O(10%)
				int prob = rand() % 100;
				int half_hp = hpMax / 2;
				int quarter_hp = hpMax / 4;
				// debug(攻撃スケジューリングは未完成)
				if( hp>half_hp ){
					if( prob < 60 ){
						Attack_ダークアームブレード_1();
					} else if( prob < 90 ){
						Attack_ヘルズローリング();
					} else{
						Attack_シューティングバスター();
					}
				} else if( hp>quarter_hp ){
					if( prob < 40 ){
						Attack_ダークアームブレード_1();
					} else if( prob < 70 ){
						Attack_ヘルズローリング();
					} else{
						Attack_シューティングバスター();
					}
				} else{
					if( prob < 20 ){
						Attack_ダークアームブレード_1();
					} else if( prob < 60 ){
						Attack_ヘルズローリング();
					} else{
						Attack_シューティングバスター();
					}
				}
			}
		}
	}
}

void Enemy_フォルテ::LoadAnim(){
	static const std::string position = "Enemy_フォルテ:LoadAnim";

	int temp[9 * 5];
	std::string fname = def::ENEMY_IMAGE_PATH + "フォルテ.png";
	LoadDivGraphWithCheckError(temp, fname, position, 9, 5, 136, 172);
	for( int i = 0; i < 4; i++ )// 最初の4個は移動
		anim[ANIM_MOVE].image[i] = temp[i];
	for( int i = 4; i < 9; i++ )
		DeleteGraph(temp[i]);
	for( int i = 0; i < 9; i++ )// 次の9個はシューティングバスター
		anim[ANIM_ATK1].image[i] = temp[i + 9];
	for( int i = 0; i < 9; i++ )// 次の9個はヘルズローリング
		anim[ANIM_ATK2].image[i] = temp[i + 18];
	for( int i = 9; i < 18; i++ )// ショット処理を延長
		anim[ANIM_ATK2].image[i] = anim[ANIM_ATK2].image[7 + ( i % 2 )];
	for( int i = 0; i < 3; i++ )// 次の3個はダークアームブレード(1 or 3発)
		anim[ANIM_ATK3].image[i] = temp[i + 27];
	for( int i = 3; i < 9; i++ )
		DeleteGraph(temp[i + 27]);
	for( int i = 0; i < 1; i++ )// 最後の1個はダメージ
		anim[ANIM_DAMAGE].image[i] = temp[i + 36];
	for( int i = 1; i < 9; i++ )
		DeleteGraph(temp[i + 36]);
	// debug(ダークネスオーバーロードの行動画像はヘルズローリングと一緒)

	anim[ANIM_MOVE].imageNum = 4;
	anim[ANIM_ATK1].imageNum = 9 + Act_シューティングバスター::ATTACK_NUM * 2;
	anim[ANIM_ATK2].imageNum = 18;
	anim[ANIM_ATK3].imageNum = 3;
	anim[ANIM_DAMAGE].imageNum = 1;

	int t[10];
	fname = def::SKILL_IMAGE_PATH + "ヘルズローリング.png";
	LoadDivGraphWithCheckError(t, fname, position, 5, 2, 128, 164);
	for( int i = 0; i < 3; i++ )
		imgヘルズローリング[i] = t[i];
	for( int i = 3; i < 10; i++ )
		DeleteGraph(t[i]);
}

void Enemy_フォルテ::DeleteAnim(){
	BattleObject::DeleteAnim();
	for( int i = 0; i < 3; i++ )
		DeleteGraph(imgヘルズローリング[i]);
}

CPoint<int> Enemy_フォルテ::GetMoveTargetPos(){
	std::vector<CPoint<int>> move_targets;
	int t[BattleField::LAYER_MAX];
	for( int x = 0; x < BattleField::FIELD_NUM_X; x++ ){
		for( int y = 0; y < BattleField::FIELD_NUM_Y; y++ ){
			BattleFieldMgr::GetInst()->GetPanelInfo(t, x, y);
			// 移動可能な位置をプッシュ
			if( t[BattleField::LAYER_USER] == BattleField::USER_ENEMY )
				if( !BattleCharMgr::GetInst()->IsAnyChar(CPoint<int>(x, y)) )
					move_targets.push_back(CPoint<int>(x, y));
		}
	}
	int i = rand() % move_targets.size();
	return move_targets[i];
}

void Enemy_フォルテ::Attack_シューティングバスター(){
	SetAnim(ANIM_ATK1);
}

void Enemy_フォルテ::Attack_ヘルズローリング(){
	moveTargetPos.push(CPoint<int>(5, 1));// 最奥中心地に移動
	reserveAnim.push(ANIM_MOVE);
	reserveAnim.push(ANIM_ATK2);
}

void Enemy_フォルテ::Attack_ダークアームブレード_1(){
	// todo(移動前に帰る場所のフラグを立てる)

	// プレイヤーの正面に移動
	CPoint<int> pos = BattleCharMgr::GetInst()->GetTargetPos(this->pos, def::eCHAR_PLAYER);
	pos.x++;
	moveTargetPos.push(pos);
	reserveAnim.push(ANIM_MOVE);

	// 攻撃
	reserveAnim.push(ANIM_ATK3);

	// 元の場所に帰る
	moveTargetPos.push(this->pos);
	reserveAnim.push(ANIM_MOVE);
}

void Enemy_フォルテ::Attack_ダークアームブレード_3(){
	ASSERT(0,"Enemy_フォルテ::Attack_ダークアームブレード_3 Unimplemented Skill");// todo
}

void Enemy_フォルテ::Attack_ダークネスオーバーロード(){
	ASSERT(0, "Enemy_フォルテ::Attack_ダークネスオーバーロード Unimplemented Skill");// todo
}
