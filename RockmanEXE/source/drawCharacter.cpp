#include <list>
#include "include.h"
#include "drawCharacter.h"

namespace{
	const int NUMBER_SIZE = 15;
};

void DrawCharacter::LoadImages(){
	static const std::string position = "DrawCharacter::LoadImages";

	ASSERT(!isLoad, position + " already loaded images");// 二度読み込みの禁止
	int tempNum[COL_MAX * 10];
	isLoad = true;

	// 数字関連
	std::string fname = def::IMAGE_FILE_PATH + "number.png";
	LoadDivGraphWithCheckError(tempNum, fname, position, 10, 3, NUMBER_SIZE, 26);
	fname = def::IMAGE_FILE_PATH + "number_2.png";
	LoadDivGraphWithCheckError(tempNum + 30, fname, position, 10, 1, NUMBER_SIZE, 20);

	// 0からスタートするように並び替え
	for( int y = 0; y < COL_MAX; y++ ){
		imgNumber[y][0] = tempNum[y * 10 + 9];
		for( int x = 0; x < 9; x++ )
			imgNumber[y][x + 1] = tempNum[y * 10 + x];
	}

	// チップコード
	fname = def::IMAGE_FILE_PATH + "chip_code.png";
	LoadDivGraphWithCheckError(imgCode, fname, position, 9, 3, 20, 26);

	fontStr = CreateFontToHandle("k8x12", 22, 7);
}

void DrawCharacter::DeleteImages(){
	isLoad = false;
	for( int i = 0; i < COL_MAX; i++ )
		for( int j = 0; j < 10; j++ )
			DeleteGraph(imgNumber[i][j]);
	for( int i = 0; i < 27; i++ )
		DeleteGraph(imgCode[i]);

	DeleteFontToHandle(fontStr);
}

void DrawCharacter::DrawNumber(int x, int y, int data, DrawCharacter::Color color, int length){
	ASSERT(isLoad, "DrawCharacter::DrawNumber images are not loaded");

	if( length > 0 ){// 右揃えで書く
		x += ( length - 1 )*NUMBER_SIZE;
		for( int i = 0; i < length&&data != 0; i++ ){
			DrawGraph(x, y, imgNumber[color][data % 10], TRUE);
			data /= 10;
			x -= NUMBER_SIZE;
		}
	} else{
		std::list<int> d;
		while( data != 0 ){
			d.push_front(data % 10);
			data /= 10;
		}
		for( std::list<int>::iterator it = d.begin(); it != d.end(); it++ ){
			DrawGraph(x, y, imgNumber[color][( *it )], TRUE);
			x += NUMBER_SIZE;
		}
	}
}

void DrawCharacter::DrawNumberPadding(int x, int y, int data, int data_length, Color color){
	ASSERT(isLoad, "DrawCharacter::DrawNumberPadding images are not loaded");

	std::list<int> d;
	for( int i = 0; i < data_length; i++ ){
		d.push_front(data % 10);
		data /= 10;
	}
	for( std::list<int>::iterator it = d.begin(); it != d.end(); it++ ){
		DrawGraph(x, y, imgNumber[color][( *it )], TRUE);
		x += NUMBER_SIZE;
	}
}

void DrawCharacter::DrawNumberCenter(int x, int y, int data, DrawCharacter::Color color){
	ASSERT(isLoad, "DrawCharacter::DrawNumberCenter images are not loaded");

	int num = 0;
	for( int i = data; i != 0; num++, i /= 10 );

	// 真中を基準に右揃えで書く
	int t = ( num + 1 ) / 2;
	x += ( t - 1 )*NUMBER_SIZE;
	for( int i = 0; i < num&&data != 0; i++ ){
		DrawGraph(x, y, imgNumber[color][data % 10], TRUE);
		data /= 10;
		x -= NUMBER_SIZE;
	}
}

void DrawCharacter::DrawChipCode(int x, int y, char code, int percent){
	if( code == '\0' )
		return;

	// 小文字はなし
	const std::string position = "DrawCharacter::DrawChipCode ";
	ASSERT(isLoad, position + "images are not loaded");
	ASSERT(( 'A' <= code&&code <= 'Z' ) || code == '*', position + "invalied code (" + ToString<char>(code) + ")");

	int p = ( code == '*' ) ? 26 : ( code - 'A' );
	if( percent == -1 )
		DrawGraph(x, y, imgCode[p], FALSE);
	else
		DrawExtendGraph(x, y, x + 20 * percent / 100, y + 26 * percent / 100, imgCode[p], FALSE);
}

void DrawCharacter::DrawString(int x, int y, std::string data, unsigned int color, unsigned edgeColor){
	ASSERT(isLoad, "DrawCharacter::DrawString font is not loaded");
	DrawStringToHandle(x, y, data.c_str(), color, fontStr, edgeColor);
}
