#include <algorithm>
#include "include.h"
#include "mosaic.h"

Mosaic::Mosaic(CPoint<int> leftUpPixel, CPoint<int> totalPixelSize, CPoint<int> separatePixelSize)
	:size(separatePixelSize) {

	int xnum = totalPixelSize.x / separatePixelSize.x;
	int ynum = totalPixelSize.y / separatePixelSize.y;
	CPoint<int> tpos;
	for( int y = 0; y < ynum; y++ ) {
		tpos.y = leftUpPixel.y + separatePixelSize.y * y;
		for( int x = 0; x < xnum; x++ ) {
			tpos.x = leftUpPixel.x + separatePixelSize.x * x;
			viewPos.push_back(tpos);
		}
	}

	std::shuffle(viewPos.begin(),viewPos.end(), rnd_generator);
}

Mosaic::~Mosaic() {
}

void Mosaic::Draw() {
	for( auto vpos : viewPos ) {
		// debug(モザイクの色は黒固定)
		DrawBox(vpos.x, vpos.y, vpos.x + size.x, vpos.y + size.y, BLACK, TRUE);
	}
}

void Mosaic::Update(unsigned int reduceNum) {
	for( unsigned int i = 0; i < reduceNum; i++ ) {
		if( viewPos.empty() ) {
			return;
		}
		viewPos.pop_back();
	}
}
