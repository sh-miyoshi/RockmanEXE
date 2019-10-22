#include "include.h"
#include "animation.h"


Animation::Animation():imageNum(0), imageDelay(0), image(), count(0), endCount(1) {
	for( int i = 0; i < ANIM_NUM_MAX; i++ ) {
		image[i] = -1;
	}
}

Animation::Animation(const Animation& data)
	:imageNum(data.imageNum), imageDelay(data.imageDelay), image(), count(0), endCount(1) {
	for( int i = 0; i < ANIM_NUM_MAX; i++ ) {
		image[i] = data.image[i];
	}
}

Animation::~Animation() {
	DeleteData();
}

void Animation::LoadData(std::string fname, CPoint<unsigned int> size, CPoint<unsigned int> num, unsigned int imageDelay, unsigned int extraImageNum) {
	LoadDivGraphWithErrorCheck(image, fname, "BattleCharBase::AnimData::LoadImage", num.x, num.y, size.x, size.y);

	this->imageNum = num.x * num.y;
	this->imageDelay = imageDelay;
	this->endCount = ( this->imageNum + extraImageNum ) * this->imageDelay;
}

void Animation::DeleteData() {
	bool deleted = false;
	for( int i = 0; i < ANIM_NUM_MAX; i++ ) {
		if( image[i] != -1 ) {
			DeleteGraph(image[i]);
			image[i] = -1;
			deleted = true;
		}
	}
}

void Animation::Draw(int x, int y) {
	unsigned int n = count / imageDelay;
	if( n >= imageNum )// ÅŒã‚Ü‚Å‚«‚½‚çÅŒã‚ÌŠG‚ð•\Ž¦‚µ‘±‚¯‚é
		n = imageNum - 1;
	DrawRotaGraph(x, y, 1, 0, image[n], TRUE);
}

bool Animation::Process() {
	count++;

	if( count >= endCount ) {
		return true;
	}
	return false;
}
