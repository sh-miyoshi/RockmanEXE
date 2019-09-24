#include "include.h"
#include "battleCharBase.h"

BattleCharBase::BattleCharBase(std::string name, int hp, int hpMax, CharType myCharType)
	:name(name), hp(hp), hpMax(hpMax), myCharType(myCharType), attachedAnimNo(ANIM_STAND) {
}

void BattleCharBase::DeleteAnim() {
	for( int i = 0; i < ANIM_PTN_MAX; i++ ) {
		anim[i].DeleteData();
	}
}

void BattleCharBase::Draw() {
	int x = def::BATTLE_PANEL_SIZE.x * pos.x + def::BATTLE_PANEL_SIZE.x / 2;
	int y = def::BATTLE_PANEL_OFFSET_Y + def::BATTLE_PANEL_SIZE.y * pos.y - 10;// Ç”ÇøÇÃï™ÇæÇØè„Ç∞ÇÈ
	anim[attachedAnimNo].Draw(x, y);
}

void BattleCharBase::Process() {
	// BaseÇÕâΩÇ‡ÇµÇ»Ç¢
}


BattleCharBase::AnimData::AnimData():imageNum(0), imageDelay(0), image(), count(0) {
	for( int i = 0; i < ANIM_NUM_MAX; i++ ) {
		image[i] = -1;
	}
}

BattleCharBase::AnimData::AnimData(const AnimData& data)
	:imageNum(data.imageNum), imageDelay(data.imageDelay), image(), count(0) {
	// TODO(copy animProc)
	for( int i = 0; i < ANIM_NUM_MAX; i++ ) {
		image[i] = data.image[i];
	}
}

BattleCharBase::AnimData::~AnimData() {
	DeleteData();
}

void BattleCharBase::AnimData::LoadData(std::string fname, CPoint<int> size, CPoint<int> num, int imageNum, int imageDelay) {
	LoadDivGraphWithErrorCheck(image, fname, "BattleCharBase::AnimData::LoadImage", num.x, num.y, size.x, size.y);
	imageFileName = fname;
	AppLogger::Debug(ToString("Success to load image: %s", imageFileName.c_str()));

	this->imageNum = imageNum;
	this->imageDelay = imageDelay;
}

void BattleCharBase::AnimData::DeleteData() {
	bool deleted = false;
	for( int i = 0; i < ANIM_NUM_MAX; i++ ) {
		if( image[i] != -1 ) {
			DeleteGraph(image[i]);
			deleted = true;
		}
	}
	if( deleted ) {
		AppLogger::Debug(ToString("Finished deleting images: %s", imageFileName.c_str()));
	}
}

void BattleCharBase::AnimData::Draw(int x, int y) {
	int n = count / imageDelay;
	if( n >= imageNum )// ç≈å„Ç‹Ç≈Ç´ÇΩÇÁç≈å„ÇÃäGÇï\é¶Çµë±ÇØÇÈ
		n = imageNum - 1;
	DrawRotaGraph(x, y, 1, 0, image[n], TRUE);
}