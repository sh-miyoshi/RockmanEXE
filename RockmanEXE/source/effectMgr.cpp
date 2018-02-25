#include "include.h"
#include "effectMgr.h"

void EffectMgr::Init(){
	int tmp[50];

	std::string fname = def::IMAGE_FILE_PATH + "explode.png";
	LoadDivGraphWithCheckError(tmp, fname, "EffectMgr::LoadImages", 8, 2, 110, 124);
	for( unsigned i = 0; i < 16; i++ )
		images[eID_EXPLODE].push_back(tmp[i]);

	// デフォルトのディレイは1
	for( auto i = 0; i < eID_MAX; i++ ){
		EFFECT_IMAGE_DELAY[i] = 1;
	}
	EFFECT_IMAGE_DELAY[eID_EXPLODE] = 4;
	EFFECT_IMAGE_OFFSET[eID_EXPLODE].x = -40;
	EFFECT_IMAGE_OFFSET[eID_EXPLODE].y = -30;
}

void EffectMgr::End(){
	for( unsigned i = 0; i < eID_MAX; i++ ){
		for( unsigned j = 0; j < images[i].size(); j++ ){
			DeleteGraph(images[i][j]);
		}
		images[i].clear();
	}
}

void EffectMgr::Process(){
	for( std::vector<EffectData_t>::iterator it = actives.begin(); it != actives.end();){
		it->count++;
		if( it->count >= it->imgDelay*EFFECT_IMAGE_DELAY[it->id] ){
			it = actives.erase(it);
		} else{
			it++;
		}
	}
}

void EffectMgr::Draw(){
	for( auto data : actives ){
		ASSERT(data.id < eID_MAX, "EffectMgr::Draw unknown id ("+ToString<int>(data.id)+")");
		unsigned ino = data.count / data.imgDelay;
		if( ino >= images[data.id].size() )
			ino = images[data.id].size() - 1;
		DrawGraph(data.pos.x+data.ofs.x, data.pos.y+data.ofs.y, images[data.id][ino], TRUE);
	}
}

bool EffectMgr::Register(int id, CPoint<int> pos){
	if( 0 <= id&&id < eID_MAX ){
		EffectData_t t;
		t.count = 0;
		t.id = id;
		t.imgDelay = EFFECT_IMAGE_DELAY[id];
		t.pos = pos;
		t.ofs = EFFECT_IMAGE_OFFSET[id];
		actives.push_back(t);
		return true;
	} else{
		AppLogger::Warn("Effectの登録に失敗しました: id=" + ToString<int>(id));
		return false;
	}
}
