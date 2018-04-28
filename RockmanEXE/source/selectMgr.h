#pragma once

#include <string>
#include <list>
#include <memory>
#include <vector>

typedef struct{
	int strColor;
	int pointerColor;
}SelectColor;

class SelectData{
	bool isAlive;
	int x, y;
	int pointer;
	std::list<std::string> data;
	SelectColor color;
	std::list<int> unSelectables;
public:
	SelectData(int x,int y, std::list<std::string> data);
	~SelectData(){}

	void Draw();
	int Process();

	bool IsAlive()const{ return isAlive; }
	void Kill(){ isAlive = false; }

	void SetColor(SelectColor color){ this->color = color; }
	void SetUnSelectable(int index);
	void SetPointer(int pointer);
};

class SelectMgr{
	int active;
	std::vector<SelectData> data;
public:
	SelectMgr();
	~SelectMgr();

	int Create(int x, int y, std::list<std::string> data);
	int Create(int x,int y, const std::string *data, unsigned size);
	void Delete(int &no);
	void AllDelete(){
		data.clear();
		active = -1;
	}

	int GetActive()const{ return active; }
	bool SetActive(int active);
	void Draw();
	int Process();
	void SetColor(SelectColor color, int active = -1);
	void SetUnSelectable(int index,int active = -1);
	void SetPointer(int pointer, int active = -1);
};