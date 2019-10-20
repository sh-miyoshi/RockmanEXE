#pragma once

#include "state.h"
#include "fade.h"

class Title {
public:
	enum RtnCode {
		eRTN_CONTINUE,
		eRTN_START_‚Í‚¶‚ß‚©‚ç,
		eRTN_START_‚Â‚Ã‚«‚©‚ç,
	};
private:
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
	public:
		StateTitleSelect(Title* obj);
		~StateTitleSelect();

		void Draw();
		void Process();
	};

	int imgBack;
	StateMgr stateMgr;
	RtnCode rtnCode;
public:
	Title();
	~Title();

	void Draw();
	RtnCode Process();
};