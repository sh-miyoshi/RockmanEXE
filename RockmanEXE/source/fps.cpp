#include "DxLib.h"
#include "fps.h"

Fps::Fps(){
	ave = 0.0f;
	count = 0;
	baseCnt = 0;
	frame = new int[FPS];
}

Fps::~Fps(){
	delete[] frame;
}

void Fps::Draw(int x, int y){
	if( ave != 0 )
		DrawFormatString(x, y, GetColor(255, 0, 0), "[%.1f]", 1000 / ave);
}

void Fps::Process(){
	int term;
	static int t = 0;

	if( count == 0 )
		term = ( t == 0 ) ? 0 : baseCnt + 1000 - GetNowCount();
	else
		term = ( int ) ( baseCnt + count*( 1000.0 / FPS ) ) - GetNowCount();

	if( term > 0 )
		Sleep(term);

	int gnt = GetNowCount();
	if( count == 0 )
		baseCnt = gnt;
	frame[count] = gnt - t;
	t = gnt;
	if( count == FPS - 1 ){
		ave = 0;
		for( int i = 0; i < FPS; i++ )
			ave += frame[i];
		ave /= FPS;
	}
	count = ( ++count ) % FPS;
}
