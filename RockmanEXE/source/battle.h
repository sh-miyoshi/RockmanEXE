#pragma once

#include <vector>
#include "state.h"
#include "chip.h"

class Battle {
public:
	enum RtnCode {
		eRTN_CONTINUE,
		eRTN_WIN,
		eRTN_LOSE
	};
private:
	class StateBegin: public StateBase {
		static const int BEGIN_COUNTUP_VALUE = 256 / 30;// 30�J�E���g�ŃL�����`�抮��

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
		static const int BT_SEND_NO = 11;// ���M�L�[�ԍ�

		class PlayerHandMgr {
			std::vector<ChipInfo> handValue;
			std::vector<int> selectedIndexes;

			bool CanSelect(unsigned int no);
		public:
			PlayerHandMgr() {}
			~PlayerHandMgr() {}

			void Init(std::vector<ChipInfo> handValue);
			bool Select(unsigned int no);
			void Draw();
			const unsigned int GetHandNum() const { return handValue.size(); }
			ChipData GetChipData(unsigned int no);
			void EraseLastSelect();
			void SetSendChipList();
		};

		int imgSelectFrame;
		int imgPointer[2];
		Battle* obj;
		unsigned int drawCount;
		unsigned int pointer;
		PlayerHandMgr playerHand;
	public:
		StateChipSelect(Battle* obj);
		~StateChipSelect();

		void Draw();
		void Process();
	};

	class StateBattleStart: public StateBase {
		static const unsigned int DRAW_DELAY = 4;

		unsigned int count;
		int imgTitle[3];
		Battle* obj;
	public:
		StateBattleStart(Battle* obj);
		~StateBattleStart();

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
