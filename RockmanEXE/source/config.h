#pragma once

#include <string>

enum KeyType{
	eKEY_ENTER,
	eKEY_CANCEL,
	eKEY_RIGHT,
	eKEY_LEFT,
	eKEY_DOWN,
	eKEY_UP,
	eKEY_MENU,
	eKEY_START,
	eKEY_DEV_L,
	eKEY_DEV_R,

	eKEY_MAX
};

class CKey{
	static const std::string CONFIG_FILE_NAME;
	static const std::string CODE_TABLE_FILE_NAME;

	std::string codeTable[256];
	unsigned int tempKey[256];
	int stateKey[eKEY_MAX];
	int keyInfo[eKEY_MAX];

	CKey();
	~CKey();
public:
	static CKey *GetInst(){
		static CKey inst;
		return &inst;
	}

	int CheckKey(KeyType handle)const{ return stateKey[handle]; }
	void Update();
	int GetNowInput();
	void Change(int handle, int code);
	std::string GetString(int handle)const{ return codeTable[keyInfo[handle]]; }
	void SaveKeySetting();
};

// 定義はmain.cpp
// ウィンドウ色
// 文字色
extern bool g_exitFlag;
