#include "include.h"
#include "effectMgr.h"

//-------------------------------------------------------
// グローバルメソッド
//-------------------------------------------------------
void EffectMgr::LoadImages() {
	const std::string position = "EffectMgr::LoadImages";
	int tmp[20]; // debug(数は最大値に合わせて決めていく)

	// ロックバスターヒット
	std::string fname = def::SKILL_IMAGE_PATH + "ロックバスター_hit.png";
	LoadDivGraphWithErrorCheck(tmp, fname, position, 6, 1, 60, 50);
	for( auto i = 0; i < 6; i++ ) {
		images[eID_ロックバスター_HIT].push_back(tmp[i]);
	}

	// 敵撃破
	fname = def::SKILL_IMAGE_PATH + "enemy_deleted.png";
	LoadDivGraphWithErrorCheck(tmp, fname, position, 8, 2, 110, 124);
	for( int i = 0; i < 16; i++ ) {
		images[eID_EnemyDeleted].push_back(tmp[i]);
	}
}

void EffectMgr::DeleteImages() {
	for( unsigned int i = 0; i < eID_MAX; i++ ) {
		for( unsigned int j = 0; j < images[i].size(); j++ ) {
			DeleteGraph(images[i][j]);
		}
	}
}

void EffectMgr::Draw() {
	for( auto e : effectList ) {
		e->Draw();
	}
}

void EffectMgr::Process() {
	for( auto it = effectList.begin(); it != effectList.end();) {
		if( ( *it )->Process() ) {
			it = effectList.erase(it);
		} else {
			it++;
		}
	}
}

void EffectMgr::Register(EffectID id, EffectArg args) {
	switch( id ) {
	case eID_ロックバスター_HIT:
	case eID_EnemyDeleted:
		effectList.push_back(std::shared_ptr<EffectData>(new EffectData(args, images[id])));
		break;
	default:
		AppLogger::Error("Effect %d is not implemented yet", id);
		break;
	}
}

EffectMgr::EffectData::EffectData(EffectArg args, std::vector<int> images)
	:images(images), count(0) {

	// 描画位置の決定
	for( int i = 0; i < args.num; i++ ) {
		int rndX = rnd_generator() % ( args.rndSize * 2 );
		int rndY = rnd_generator() % ( args.rndSize * 2 );
		CPoint<int> pos;
		pos.x = args.pos.x - args.rndSize + rndX;
		pos.y = args.pos.y - args.rndSize + rndY;
		drawPositions.push_back(pos);
	}
}

EffectMgr::EffectData::~EffectData() {
}

void EffectMgr::EffectData::Draw() {
	unsigned int no = count / images.size();
	if( no >= drawPositions.size() )
		no = drawPositions.size() - 1;
	unsigned int ino = count % images.size();
	DrawRotaGraph(drawPositions[no].x, drawPositions[no].y, 1, 0, images[ino], TRUE);
}

bool EffectMgr::EffectData::Process() {
	count++;
	if( count >= ( drawPositions.size() * images.size() ) ) {
		return true;
	}
	return false;
}
