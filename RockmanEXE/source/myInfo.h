#pragma once

#include <string>
#include "state.h"
#include "selectMgr.h"

// 自キャラの情報を表示するクラス
class MyInfo{
	// トップメニューを表示するクラス
	class StateTop:public StateBase{
		enum Item{
			eITEM_ロックマン,
			eITEM_戦績,
			eITEM_Exit,
			eITEM_MAX
		};

		SelectMgr selectMgr;
		MyInfo *obj;
	public:
		StateTop(MyInfo *obj);
		~StateTop();

		void Draw();
		void Process();
	};

	// キャラ情報を表示するクラス
	class StateRockman:public StateBase{
		MyInfo *obj;
		int image;
	public:
		StateRockman(MyInfo *obj);
		~StateRockman();

		void Draw();
		void Process();
	};

	// 戦績を表示するクラス
	class StateLog:public StateBase{
		MyInfo *obj;
	public:
		StateLog(MyInfo *obj);
		~StateLog();

		void Draw();
		void Process();
	};

	bool isFinish;
	StateMgr stateMgr;
public:
	MyInfo();
	~MyInfo();

	void Draw();
	bool Process();
};