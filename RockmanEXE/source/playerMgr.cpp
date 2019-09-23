#include "include.h"
#include "playerMgr.h"

PlayerMgr::PlayerMgr():name("ÉçÉbÉNÉ}Éì"), hp(0), hpMax(0), battlePlayer(nullptr) {
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
}

BattlePlayer::BattlePlayer(std::string name, int hp, int hpMax): BattleCharBase(name, hp, hpMax, eCHAR_PLAYER) {
}

BattlePlayer::~BattlePlayer() {
}
