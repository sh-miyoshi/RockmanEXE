#pragma once

#include "point.h"

class BattleBack{
public:
	enum Type{
		eTYPE_秋原町,
		eTYPE_ブラックアース,

		eTYPE_MAX
	};
private:
	typedef struct{
		unsigned int imageNum;
		int image[8];
		int backColor;// 塗りつぶす場合の背景色
		CPoint<unsigned int> size;// 読み込みサイズと感覚を考慮して
	}BackImage_t;

	bool isLoad;
	unsigned count;
	BackImage_t data[eTYPE_MAX];
public:
	BattleBack():count(0), isLoad(false), data(){}
	~BattleBack(){}

	void LoadImages();
	void DeleteImages();

	void Draw(Type type);
	void Process();
};