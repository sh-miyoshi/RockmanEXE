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
	: BattleCharBase(name, hp, hpMax, eCHAR_PLAYER, defaultAnim) {
	// アニメーションの設定
	std::string fname = def::IMAGE_FILE_PATH + "player_move.png";
	animMove = std::shared_ptr<AnimMove>(new AnimMove());
	animMove->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(4, 1));
}

BattlePlayer::~BattlePlayer() {
}

void BattlePlayer::Process() {
	AnimProcess();

	// TODO(キー入力)
	// 移動処理
	if( CKey::GetInst()->CheckKey(eKEY_DOWN) == 1 ) {
		animMove->SetMoveDirect(def::eMUKI_DOWN);
		this->AttachAnim(animMove);
	}
}
