#include "include.h"
#include "animation.h"


Animation::Animation():imageNum(0), imageDelay(0), image(), count(0) {
	for( int i = 0; i < ANIM_NUM_MAX; i++ ) {
		image[i] = -1;
	}
}

Animation::Animation(const Animation& data)
	:imageNum(data.imageNum), imageDelay(data.imageDelay), image(), count(0) {
	for( int i = 0; i < ANIM_NUM_MAX; i++ ) {
		image[i] = data.image[i];
	}
}

Animation::~Animation() {
	DeleteData();
}

void Animation::LoadData(std::string fname, CPoint<unsigned int> size, CPoint<unsigned int> num, unsigned int imageDelay) {
	LoadDivGraphWithErrorCheck(image, fname, "BattleCharBase::AnimData::LoadImage", num.x, num.y, size.x, size.y);
	imageFileName = fname;
	AppLogger::Debug(ToString("Success to load image: %s", imageFileName.c_str()));

	this->imageNum = num.x * num.y;
	this->imageDelay = imageDelay;
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
	if( deleted ) {
		AppLogger::Debug(ToString("Finished deleting images: %s", imageFileName.c_str()));
	}
}

void Animation::Draw(int x, int y) {
	unsigned int n = count / imageDelay;
	if( n >= imageNum )// 最後まできたら最後の絵を表示し続ける
		n = imageNum - 1;
	DrawRotaGraph(x, y, 1, 0, image[n], TRUE);
}

bool Animation::Process() {
	return true;// 即終了
}

AnimMove::AnimMove():moveDirect(def::eMUKI_DOWN) {
}

AnimMove::~AnimMove() {
}

void AnimMove::SetMoveDirect(def::Muki direct) {
	this->moveDirect = direct;
}

bool AnimMove::Process() {
	printfDx("anim move: %d\n", moveDirect);
	return true;
}
