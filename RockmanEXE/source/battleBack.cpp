#include "include.h"
#include "battleBack.h"

void BattleBack::LoadImages(){
	if( isLoad ) {
		AppLogger::Error("BattleBack::LoadImages already loaded images");
		exit(1);
	}

	isLoad = true;
	count = 0;

	for( int i = 0; i < eTYPE_MAX; i++ ){
		std::string position = "BattleBackMgr::LoadImages";
		std::string fname = def::IMAGE_FILE_PATH + "back_image_";
		switch( i ){
		case eTYPE_秋原町:
			fname += "秋原町.png";
			LoadDivGraphWithErrorCheck(data[i].image, fname, position, 2, 4, 64, 64);
			data[i].backColor = GetColor(0, 0, 160);
			data[i].imageNum = 8;
			data[i].size = CPoint<unsigned int>(100, 100);
			break;
		case eTYPE_ブラックアース:
			fname += "ブラックアース.png";
			LoadDivGraphWithErrorCheck(data[i].image, fname, position, 2, 2, 512, 512);
			for( int j = 0; j < 4; j++ )
				data[i].image[j + 4] = data[i].image[3 - j];
			data[i].backColor = GetColor(0, 0, 0);
			data[i].imageNum = 8;
			data[i].size = CPoint<unsigned int>(512, 512);
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

void BattleBack::Draw(Type type){
	if( !isLoad ) {
		AppLogger::Error("BattleBack::Draw images are not loaded");
		exit(1);
	}

	if( data[type].size.x <= 0 || data[type].size.y <= 0 ) {
		AppLogger::Error(ToString("BattleBack::Draw invalied position (%d,%d)", data[type].size.x, data[type].size.y));
		exit(1);
	}

	DrawBox(0, 0, def::FMX, def::FMY, data[type].backColor, TRUE);
	for( unsigned int x = 0; x < ( def::FMX / data[type].size.x + 1 ); x++ ){
		for( unsigned int y = 0; y < ( def::FMY / data[type].size.y + 1 ); y++ ){
			unsigned int timing = 0;
			unsigned int viewx = data[type].size.x*x;
			unsigned int viewy = data[type].size.y*y;
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
