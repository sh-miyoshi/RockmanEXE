#include "include.h"
#include "enemy.h"
#include "battleCharMgr.h"
#include "battleField.h"

class EnemyBase:public BattleCharBase {
protected:
	unsigned int count;
	int imgNumber[10];
public:
	EnemyBase(std::string name, unsigned int hp);
	~EnemyBase();

	virtual void Draw();
};

class Enemy_的:public EnemyBase {
public:
	Enemy_的();
	~Enemy_的();
};

class Enemy_メットール:public EnemyBase {
	static const int Power_ショックウェーブ = 10;// debug(固定？)

	class AnimMove:public Animation {
		Enemy_メットール* obj;
	public:
		AnimMove(Enemy_メットール* obj):obj(obj) {}

		void Begin() {
			int y = BattleCharMgr::GetInst()->GetClosestCharPos(obj->pos, eCHAR_PLAYER).y;
			if( y < 0 ) {// ロックマンが見つからなかった
				// 何もしない
			} else if( y < obj->pos.y ) {// ロックマンが上にいるなら
				if( obj->MoveCheck(obj->pos.x, obj->pos.y - 1) ) {
					obj->SetPos(obj->pos.x, obj->pos.y - 1);
				}
			} else {
				if( obj->MoveCheck(obj->pos.x, obj->pos.y + 1) ) {
					obj->SetPos(obj->pos.x, obj->pos.y + 1);
				}
			}
		}
	};

	unsigned int waveCount;
	std::shared_ptr<AnimMove> animMove;
public:
	Enemy_メットール();
	~Enemy_メットール();

	virtual void Process();
};

//-------------------------------------------------------
// 全体処理
//-------------------------------------------------------
std::shared_ptr<BattleCharBase> EnemyMgr::GetData(int id) {
	switch( id ) {
	case ID_的:
		return std::shared_ptr<Enemy_的>(new Enemy_的());
	case ID_メットール:
		return std::shared_ptr<Enemy_メットール>(new Enemy_メットール());
	default:
		AppLogger::Error("EnemyMgr::GetData wrong char id (%d)", id);
		exit(1);
	}
	return std::shared_ptr<BattleCharBase>();
}

//-------------------------------------------------------
// 敵関連の基底クラス
//-------------------------------------------------------
EnemyBase::EnemyBase(std::string name, unsigned int hp)
	:count(0), BattleCharBase(name, hp, hp, eCHAR_ENEMY), imgNumber() {
	std::string fname = def::IMAGE_FILE_PATH + "その他/number_2.png";
	LoadDivGraphWithErrorCheck(imgNumber, fname, "EnemyBase::EnemyBase", 10, 1, 15, 20);
}

EnemyBase::~EnemyBase() {
	for( auto i = 0; i < 10; i++ ) {
		DeleteGraph(imgNumber[i]);
	}
}

void EnemyBase::Draw() {
	// TODO(HPが0ならSetDrawBlendMode)
	BattleCharBase::Draw();

	// HPの描画
	CPoint<int> t = BattleField::GetPixelPos(CPoint<int>(pos.x, pos.y + 1));
	std::list<unsigned int> hpList;
	for( unsigned int h = this->hp; h > 0; h /= 10 ) {
		hpList.push_front(h % 10);
	}
	const int numSize = 15;
	t.x = t.x - numSize * hpList.size() / 2 + 7;
	for( auto h : hpList ) {
		unsigned pointer = ( h == 0 ) ? 9 : h - 1;
		DrawRotaGraph(t.x, t.y, 1, 0, imgNumber[pointer], TRUE);
		t.x += numSize;
	}
}


//-------------------------------------------------------
// テスト用の的
//-------------------------------------------------------
Enemy_的::Enemy_的():EnemyBase("的", 10) {
	std::string fname = def::CHARACTER_IMAGE_PATH + "的.png";
	std::shared_ptr<Animation> animStand = std::shared_ptr<Animation>(new Animation());
	animStand->LoadData(fname, CPoint<unsigned int>(100, 117), CPoint<unsigned int>(1, 1));
	BattleCharBase::SetDefaultAnim(animStand);
}

Enemy_的::~Enemy_的() {
}

//-------------------------------------------------------
// メットール
//-------------------------------------------------------
Enemy_メットール::Enemy_メットール()
	:EnemyBase("メットール", 40), waveCount(0) {
	std::string fname = def::CHARACTER_IMAGE_PATH + "メットール_stand.png";
	std::shared_ptr<Animation> animStand = std::shared_ptr<Animation>(new Animation());
	animStand->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(1, 1));
	BattleCharBase::SetDefaultAnim(animStand);

	fname = def::CHARACTER_IMAGE_PATH + "メットール_move.png";
	animMove = std::shared_ptr<AnimMove>(new AnimMove(this));
	animMove->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(1, 1));
}

Enemy_メットール::~Enemy_メットール() {
}

void Enemy_メットール::Process() {
	// デリートモーション中(操作不可)
	if( hp <= 0 ) {
		return;
	}

	static const int WAIT_COUNT = 1 * 60;// 最初の何もしないカウント
	static const int MOVE_COUNT = 1 * 60;
	static const int SHOCK_WAVE_COUNT = 30;

	//-------------------------------
	// 最初のnカウント:	何もしない
	// 以降ループ
	//	プレイヤー探索
	//	if y座標が同じ&一定時間
	//		攻撃登録
	//-------------------------------
	if( AnimProcess() ) {
		if( count >= WAIT_COUNT ) {
			int y = BattleCharMgr::GetInst()->GetClosestCharPos(pos, eCHAR_PLAYER).y;// ロックマンの位置
			if( pos.y == y )
				waveCount++;
			else
				waveCount = 0;

			if( waveCount >= SHOCK_WAVE_COUNT ) {
				// TODO(ショックウェーブを打つ)
				waveCount = 0;
				count = 0;
			} else if( count % MOVE_COUNT == 0 ) {
				this->AttachAnim(animMove);
			}
		}
		count++;
	}
}
