#pragma once

#include <string>

class DrawCharacter{
public:
	enum Color{
		COL_WHITE,
		COL_RED,
		COL_GREEN,
		COL_WHITE_SMALL,

		COL_MAX
	};
private:
	bool isLoad;
	int imgNumber[COL_MAX][10];
	int imgCode[27];
	// debug(imgCharacter)
	int fontStr;// ���{��p�t�H���g, debug(�����t�H���g������)

	DrawCharacter():isLoad(false){}
	~DrawCharacter(){}
public:
	static DrawCharacter *GetInst(){
		static DrawCharacter inst;
		return &inst;
	}

	void LoadImages();
	void DeleteImages();

	// �����t�H���g�C���[�W�ŕ`��(length: �w�莞�͉E�����ŕ`��)
	void DrawNumber(int x,int y,int data,Color color=COL_WHITE,int length=0);
	void DrawNumberPadding(int x,int y,int data,int data_length,Color color=COL_WHITE);
	void DrawNumberCenter(int x,int y,int data,Color color);
	void DrawChipCode(int x,int y,char code,int percent=-1);
	void DrawString(int x,int y,std::string data,unsigned int color,unsigned edgeColor=-1);
};