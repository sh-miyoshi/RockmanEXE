#pragma once

#include "state.h"

class Battle{
	class BattleChipSelect: public StateBase {
		int imgSelectFrame;
		class Battle* obj;
	public:
		BattleChipSelect(class Battle* obj);
		~BattleChipSelect();

		void Draw();
		void Process();
	};

	class BattleMain: public StateBase {
		class Battle* obj;
	public:
		BattleMain(class Battle* obj);
		~BattleMain();

		void Draw();
		void Process();
	};

	class BattleWin: public StateBase {
		class Battle* obj;
	public:
		BattleWin(class Battle* obj);
		~BattleWin();

		void Draw();
		void Process();
	};

	StateMgr stateMgr;
public:
	enum RtnCode {
		eRTN_CONTINUE,
		eRTN_WIN,
		eRTN_LOSE
	};

	Battle();
	~Battle();

	void Draw();
	RtnCode Process();
};