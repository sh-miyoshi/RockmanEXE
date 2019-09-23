#include "battleCharBase.h"

BattleCharBase::BattleCharBase(std::string name, int hp, int hpMax, CharType myCharType)
	:name(name), hp(hp), hpMax(hpMax), myCharType(myCharType) {
}

void BattleCharBase::Draw() {
}

void BattleCharBase::Process() {
	// Base‚Í‰½‚à‚µ‚È‚¢
}
