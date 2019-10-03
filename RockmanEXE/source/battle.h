#pragma once

#include "state.h"

class Battle {
public:
	enum RtnCode {
		eRTN_CONTINUE,
		eRTN_WIN,
		eRTN_LOSE
	};
private:
	class StateBegin: public StateBase {
		static const int BEGIN_COUNTUP_VALUE = 256 / 30;// 30カウントでキャラ描画完了

		unsigned int drawNo;
		unsigned int count;
		Battle* obj;
	public:
		StateBegin(Battle* obj);
		~StateBegin();

		void Draw();
		void Process();
	};

	class StateChipSelect: public StateBase {
		static const int BT_SEND_NO = 11;// 送信キー番号

		int imgSelectFrame;
		Battle* obj;
	public:
		StateChipSelect(Battle* obj);
		~StateChipSelect();

		void Draw();
		void Process();
	};

	class StateMain: public StateBase {
		Battle* obj;
	public:
		StateMain(Battle* obj);
		~StateMain();

		void Draw();
		void Process();
	};

	class StateWin: public StateBase {
		unsigned int count;
		int imgResultFrame;
		Battle* obj;
	public:
		StateWin(Battle* obj);
		~StateWin();

		void Draw();
		void Process();
	};

	RtnCode rtnCode;
	StateMgr stateMgr;
public:
	Battle();
	~Battle();

	void Draw();
	RtnCode Process();
};