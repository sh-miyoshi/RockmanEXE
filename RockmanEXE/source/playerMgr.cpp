#include "include.h"
#include "playerMgr.h"

PlayerMgr::PlayerMgr():name("ロックマン"), hp(0), hpMax(0), battlePlayer(nullptr) {
}

PlayerMgr::~PlayerMgr() {
	if( battlePlayer ) {
		delete battlePlayer;
		battlePlayer = nullptr;
	}
}

void PlayerMgr::InitBattleChar() {
	if( battlePlayer ) {
		delete battlePlayer;
	}
	std::string fname = def::IMAGE_FILE_PATH + "player_stand.png";
	std::shared_ptr<Animation> animStand = std::shared_ptr<Animation>(new Animation());
	animStand->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(1, 1));

	battlePlayer = new BattlePlayer(name, hp, hpMax, animStand);
	battlePlayer->SetPos(1, 1);// 初期位置のセット
}

void PlayerMgr::InitPlayer() {
	// 初期データをセット
	hp = hpMax = 100;
	name = "ロックマン";
}

BattlePlayer::BattlePlayer(std::string name, unsigned int hp, unsigned int hpMax, std::shared_ptr<Animation> defaultAnim)
	: BattleCharBase(name, hp, hpMax, eCHAR_PLAYER, defaultAnim), chargeCount(0){
	// アニメーションの設定
	std::string fname = def::IMAGE_FILE_PATH + "player_move.png";
	animMove = std::shared_ptr<Animation>(new Animation());
	animMove->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(4, 1));

	fname = def::IMAGE_FILE_PATH + "player_shot.png";
	animShot = std::shared_ptr<Animation>(new Animation());
	animShot->LoadData(fname, CPoint<unsigned int>(180, 100), CPoint<unsigned int>(6, 1));
}

BattlePlayer::~BattlePlayer() {
}

void BattlePlayer::Process() {
	// 移動処理
	if( CKey::GetInst()->CheckKey(eKEY_DOWN) == 1 ) {
		if( MoveCheck(pos.x, pos.y + 1) ) {
			this->SetPos(pos.x, pos.y + 1);
			this->AttachAnim(animMove);
		}
	} else if( CKey::GetInst()->CheckKey(eKEY_UP) == 1 ) {
		if( MoveCheck(pos.x, pos.y - 1) ) {
			this->SetPos(pos.x, pos.y - 1);
			this->AttachAnim(animMove);
		}
	} else if( CKey::GetInst()->CheckKey(eKEY_LEFT) == 1 ) {
		if( MoveCheck(pos.x - 1, pos.y) ) {
			this->SetPos(pos.x - 1, pos.y);
			this->AttachAnim(animMove);
		}
	} else if( CKey::GetInst()->CheckKey(eKEY_RIGHT) == 1 ) {
		if( MoveCheck(pos.x + 1, pos.y) ) {
			this->SetPos(pos.x + 1, pos.y);
			this->AttachAnim(animMove);
		}
	}

	// ロックバスター関連
	if( CKey::GetInst()->CheckKey(eKEY_CANCEL) != 0 )// キャンセルキーでショット
		chargeCount++;
	else if( chargeCount > 0 ) {
		// TODO(チャージショット)
		this->AttachAnim(animShot);
		chargeCount = 0;
	}

	// TODO(チップを使う)

	AnimProcess();
}
