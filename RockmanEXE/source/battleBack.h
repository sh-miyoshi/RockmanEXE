#pragma once

#include "point.h"

class BattleBack{
public:
	enum Type{
		eTYPE_�H����,
		eTYPE_�u���b�N�A�[�X,

		eTYPE_MAX
	};
private:
	typedef struct{
		int imageNum;
		int image[8];
		int backColor;// �h��Ԃ��ꍇ�̔w�i�F
		CPoint<int> size;// �ǂݍ��݃T�C�Y�Ɗ��o���l������
	}BackImage_t;

	bool isLoad;
	unsigned count;
	BackImage_t data[eTYPE_MAX];
public:
	BattleBack():count(0), isLoad(false){}
	~BattleBack(){}

	void LoadImages();
	void DeleteImages();

	void Draw(int type);
	void Process();
};