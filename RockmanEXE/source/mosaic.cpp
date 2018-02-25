#include "include.h"
#include "mosaic.h"

Mosaic::Mosaic(CPoint<int> pos, CPoint<int> size, CPoint<int> separateSize)
	:pos(pos), size(size), separateSize(separateSize)
{
	int mosaic_num = ( size.x / separateSize.x )*( size.y / separateSize.y );
	for( int i = 0; i < mosaic_num; i++ )
		data.push_back(i);
}

Mosaic::~Mosaic(){
}

void Mosaic::Draw(){
	for( unsigned i = 0; i < data.size(); i++ ){
		int x = data[i] % ( size.x / separateSize.x );
		int y = data[i] / ( size.x / separateSize.x );
		x = pos.x + x*separateSize.x;
		y = pos.y + y*separateSize.y;
		DrawBox(x, y, x + separateSize.x, y + separateSize.y, BLACK, TRUE);
	}
}

void Mosaic::Update(int reduceNum){
	for( int i = 0; i < reduceNum; i++ ){
		if( data.size() <= 0 )
			return;
		int n = rand() % data.size();
		data.erase(data.begin() + n);
	}
}
