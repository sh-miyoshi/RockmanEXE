#pragma once

#include <string>
#include <list>
#include <vector>
#include <memory>
#include "point.h"

class WinData{
	static const int ROW_MAX = 20;

	bool isAlive;
	int x, y;
	int colNum, rowNum;
	std::vector<int> imgFace;
	int msgSpeed;
	std::string msg;
	std::string outMsg[ROW_MAX];
	int nowRow;

	void DrawScrollMark();
public:
	WinData(int x, int y, int colNum, int rowNum, std::string msg);
	~WinData(){}

	void Draw();
	bool IsAlive()const{ return isAlive; }
	void Kill(){ isAlive = false; }
	void SetFaceImage(std::string fname, CPoint<int> size, CPoint<int> num);
	void DeleteFaceImage();
};

class WinMgr{
	int active;
	std::vector<std::shared_ptr<WinData>> data;
public:
	enum{
		eFACE_NULL,
		eFACE_ロックマン,
		eFACE_フォルテ,
	};

	WinMgr();
	~WinMgr();

	//<size 38:46>
	int Create(int x, int y, int colNum, int rowNum, std::string msg);
	void Delete(int &no);
	void AllDelete(){
		data.clear();
		active = -1;
	}

	int GetActive()const{ return active; }
	bool SetActive(int active);
	void Draw();
	// todo(UpdateMassage)
};