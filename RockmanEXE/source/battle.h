#pragma once

#include <vector>
#include "state.h"
#include "chip.h"
#include "enemy.h"
#include "point.h"
#include "mosaic.h"

class Battle {
public:
	enum RtnCode {
		eRTN_CONTINUE,
		eRTN_WIN,
		eRTN_LOSE
	};

	typedef struct EnemyInfo {
		EnemyMgr::EnemyID id;
		CPoint<int> pos;
	}EnemyInfo;
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
		static const unsigned int WAIT_TIME = 10;
		static const unsigned int STATE_END_TIME = WAIT_TIME + DRAW_DELAY * 3 * 2 + 20;

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

	class StateEnemyDeleted: public StateBase {
		static const unsigned int DRAW_DELAY = 4;
		static const unsigned int INIT_WAIT_TIME = 10;
		static const unsigned int END_WAIT_TUIME = 60;
		static const unsigned int STATE_END_TIME = INIT_WAIT_TIME + DRAW_DELAY * 3 * 2 + END_WAIT_TUIME;

		unsigned int count;
		int imgTitle[3];
		Battle* obj;
	public:
		StateEnemyDeleted(Battle* obj);
		~StateEnemyDeleted();

		void Draw();
		void Process();

	};

	class StateWin: public StateBase {
		class ResultValue {
		public:
			enum Type {
				eTYPE_MONEY,
				eTYPE_CHIP
			};

			int image;
			std::string name;
			int value;
			Type type;

			ResultValue():image(-1), value(0), type(eTYPE_MONEY) {}
			~ResultValue() {}
		};

		static const int VIEW_ITEM_COUNT = 20;

		unsigned int count;
		unsigned int bustingLv;
		int imgResultFrame;
		int imgZenny;
		ResultValue getItem;
		Mosaic mosaic;
		Battle* obj;
	public:
		StateWin(Battle* obj);
		~StateWin();

		void Draw();
		void Process();
	};

	class StateLose: public StateBase {
		static const unsigned int END_TIME = 30;

		unsigned int count;
		Battle* obj;
	public:
		StateLose(Battle* obj);
		~StateLose();

		void Draw();
		void Process();
	};

	bool isBoss;
	unsigned int mainProcCount;
	RtnCode rtnCode;
	StateMgr stateMgr;
public:
	Battle(std::vector<EnemyInfo> enemies);
	~Battle();

	void Draw();
	RtnCode Process();
};
