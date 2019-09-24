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
	battlePlayer = new BattlePlayer(name, hp, hpMax);
	battlePlayer->SetPos(1, 1);// 初期位置のセット
	battlePlayer->LoadAnim();
}

void PlayerMgr::InitPlayer() {
	// 初期データをセット
	hp = hpMax = 100;
	name = "ロックマン";
}

BattlePlayer::BattlePlayer(std::string name, int hp, int hpMax): BattleCharBase(name, hp, hpMax, eCHAR_PLAYER) {
}

BattlePlayer::~BattlePlayer() {
}

void BattlePlayer::LoadAnim() {
	static const std::string position = "BattlePlayer::LoadAnim";

	std::string fname = def::IMAGE_FILE_PATH + "player_stand.png";
	anim[ANIM_STAND].LoadData(fname, CPoint<int>(100, 100), CPoint<int>(1, 1), 1);

	fname = def::IMAGE_FILE_PATH + "player_move.png";
	anim[ANIM_MOVE].LoadData(fname,CPoint<int>(100,100),CPoint<int>(4,1),4);

	//fname = def::IMAGE_FILE_PATH + "player_damage.png";
	//LoadDivGraphWithCheckError(anim[ANIM_DAMAGE].image, fname, position, 4, 1, 100, 100);
	//// 1 -> 2,3  2-4 3-5
	//anim[ANIM_DAMAGE].image[4] = anim[ANIM_DAMAGE].image[2];
	//anim[ANIM_DAMAGE].image[5] = anim[ANIM_DAMAGE].image[3];
	//anim[ANIM_DAMAGE].image[2] = anim[ANIM_DAMAGE].image[1];
	//anim[ANIM_DAMAGE].image[3] = anim[ANIM_DAMAGE].image[1];
	//anim[ANIM_DAMAGE].imageNum = 6;

	//fname = def::IMAGE_FILE_PATH + "player_shot.png";
	//LoadDivGraphWithCheckError(anim[ANIM_ATK1].image, fname, position, 6, 1, 180, 100);
	//anim[ANIM_ATK1].imageNum = 6;
	//// チャージ画像
	//fname = def::IMAGE_FILE_PATH + "charge.png";
	//LoadDivGraphWithCheckError(imgCharge, fname, position, 8, 2, 158, 150);

	//fname = def::IMAGE_FILE_PATH + "player_cannon.png";
	//LoadDivGraphWithCheckError(anim[ANIM_ATK2].image, fname, position, 4, 1, 100, 100);
	//anim[ANIM_ATK2].imageNum = 4;

	//fname = def::IMAGE_FILE_PATH + "player_sword.png";
	//LoadDivGraphWithCheckError(anim[ANIM_ATK3].image, fname, position, 7, 1, 128, 128);
	//anim[ANIM_ATK3].imageNum = 7;

	//fname = def::IMAGE_FILE_PATH + "player_bomb.png";
	//LoadDivGraphWithCheckError(anim[ANIM_ATK4].image, fname, position, 5, 1, 100, 114);
	//anim[ANIM_ATK4].imageNum = 5;

	//// ココロ状態画像
	//fname = def::IMAGE_FILE_PATH + "mind_status.png";
	//int temp[18];
	//LoadDivGraphWithCheckError(temp, fname, position, 6, 3, 88, 32);

}
