#pragma once

#include "state.h"
#include "fade.h"
#include "selecter.h"

class Title {
	class StateTitleBegin:public StateBase {
		static const unsigned int ALLOW_SELECT_COUNT = 60;

		Title* obj;
		unsigned int count;
		Fade fade;
	public:
		StateTitleBegin(Title* obj);
		~StateTitleBegin();

		void Draw();
		void Process();
	};

	class StateTitleSelect:public StateBase {
		Title* obj;
		Selecter selecter;
	public:
		StateTitleSelect(Title* obj);
		~StateTitleSelect();

		void Draw();
		void Process();
	};

	int imgBack;
	StateMgr stateMgr;
	bool isFinish;
public:
	Title();
	~Title();

	void Draw();
	bool Process();
};