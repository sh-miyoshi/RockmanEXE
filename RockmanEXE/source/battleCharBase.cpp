#include "include.h"
#include "battleCharBase.h"

void BattleCharBase::AnimProcess() {
	if( animQueue.empty() ) {
		defaultAnim->Process();
	} else {
		// TODO(未実装)
	}
}

BattleCharBase::BattleCharBase(std::string name, unsigned int hp, unsigned int hpMax, CharType myCharType, std::shared_ptr<Animation> defaultAnim)
	:name(name), hp(hp), hpMax(hpMax), myCharType(myCharType), defaultAnim(defaultAnim) {
}

unsigned int BattleCharBase::AddAnimDefine(std::shared_ptr<Animation> anim) {
	animData.push_back(anim);
	return animData.size() - 1;// 登録したアニメーションのindexを返す
}

void BattleCharBase::Draw() {
	int x = def::BATTLE_PANEL_SIZE.x * pos.x + def::BATTLE_PANEL_SIZE.x / 2;
	int y = def::BATTLE_PANEL_OFFSET_Y + def::BATTLE_PANEL_SIZE.y * pos.y - 10;// ふちの分だけ上げる

	if( animQueue.empty() ) {
		defaultAnim->Draw(x, y);
	} else {
		std::shared_ptr<Animation> anim = animQueue.front();
		anim->Draw(x, y);
	}
}

void BattleCharBase::Process() {
	// Baseは何もしない
}


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