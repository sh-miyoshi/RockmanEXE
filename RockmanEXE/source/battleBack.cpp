#include "include.h"
#include "battleBack.h"

void BattleBack::LoadImages(){
	ASSERT(!isLoad, "BattleBack::LoadImages already loaded images");
	isLoad = true;
	count = 0;

	for( int i = 0; i < eTYPE_MAX; i++ ){
		std::string position = "BattleBackMgr::LoadImages";
		std::string fname = def::IMAGE_FILE_PATH + "back_image_";
		switch( i ){
		case eTYPE_秋原町:
			fname += "秋原町.png";
			LoadDivGraphWithCheckError(data[i].image, fname, position, 2, 4, 64, 64);
			data[i].backColor = GetColor(0, 0, 160);
			data[i].imageNum = 8;
			data[i].size = CPoint<int>(100, 100);
			break;
		case eTYPE_ブラックアース:
			fname += "ブラックアース.png";
			LoadDivGraphWithCheckError(data[i].image, fname, position, 2, 2, 512, 512);
			for( int j = 0; j < 4; j++ )
				data[i].image[j + 4] = data[i].image[3 - j];
			data[i].backColor = GetColor(0, 0, 0);
			data[i].imageNum = 8;
			data[i].size = CPoint<int>(512, 512);
			break;
		}
	}
}

void BattleBack::DeleteImages(){
	if( isLoad ){
		isLoad = false;

		for( int i = 0; i < eTYPE_MAX; i++ )
			for( int j = 0; j < 8; j++ )
				DeleteGraph(data[i].image[j]);
	}
}

void BattleBack::Draw(int type){
	std::string position = "BattleBack::Draw ";
	ASSERT(isLoad, position + "images are not loaded");
	ASSERT(0 <= type&&type < eTYPE_MAX, position + ToString("invalied type (%d)", type));
	ASSERT(data[type].size.x > 0 && data[type].size.y > 0, position + ToString("invalied position (%d,%d)", data[type].size.x, data[type].size.y));

	DrawBox(0, 0, def::FMX, def::FMY, data[type].backColor, TRUE);

	int timing = -1;
	CPoint<int> times(def::FMX / data[type].size.x + 1, def::FMY / data[type].size.y + 1);
	for( int x = 0; x < times.x; x++ ){
		for( int y = 0; y < times.y; y++ ){
			int viewx = data[type].size.x*x;
			int viewy = data[type].size.y*y;
			switch( type ){
			case eTYPE_秋原町:
				if( x % 2 != 0 )
					viewy += 50;
				timing = ( ( count / 50 ) + x ) % data[type].imageNum;
				break;
			case eTYPE_ブラックアース:
				timing = ( count / 20 ) % data[type].imageNum;
				break;
			}
			DrawGraph(viewx, viewy, data[type].image[timing], FALSE);
		}
	}
}

void BattleBack::Process(){
	count++;
}
