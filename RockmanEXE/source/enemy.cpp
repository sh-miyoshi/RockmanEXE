#include "include.h"
#include "enemy.h"
#include "battleCharMgr.h"

class Enemy_メットール:public BattleCharBase {
	static const int Power_ショックウェーブ = 10;// debug(固定？)

	class AnimMove:public Animation {
		Enemy_メットール* obj;
	public:
		AnimMove(Enemy_メットール* obj):obj(obj) {}

		void Begin() {
			int y = BattleCharMgr::GetInst()->GetClosestCharPos(obj->pos, eCHAR_PLAYER).y;
			if( y < obj->pos.y ) {// ロックマンが上にいるなら
				if( obj->MoveCheck(obj->pos.x, obj->pos.y - 1) ) {
					obj->SetPos(obj->pos.x, obj->pos.y - 1);
				}
			} else if( y > obj->pos.y ) {
				if( obj->MoveCheck(obj->pos.x, obj->pos.y + 1) ) {
					obj->SetPos(obj->pos.x, obj->pos.y + 1);
				}
			}
		}
	};

	unsigned int count;
	unsigned int waveCount;
	std::shared_ptr<Animation> animStand;
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
	case ID_メットール:
		return std::shared_ptr<Enemy_メットール>(new Enemy_メットール());
	default:
		AppLogger::Error(ToString("EnemyMgr::GetData wrong char id (%d)", id));
		exit(1);
	}
	return std::shared_ptr<BattleCharBase>();
}

//-------------------------------------------------------
// メットール
//-------------------------------------------------------
Enemy_メットール::Enemy_メットール()
	:BattleCharBase("メットール", 40, 40, eCHAR_ENEMY, animStand), count(0), waveCount(0) {
	std::string fname = def::ENEMY_IMAGE_PATH + "メットール_stand.png";
	animStand->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(1, 1));

	fname = def::ENEMY_IMAGE_PATH + "メットール_move.png";
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

	AnimProcess();
}
