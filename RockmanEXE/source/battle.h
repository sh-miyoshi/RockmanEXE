#pragma once

#include "chip.h"
#include "state.h"
#include "mosaic.h"

enum BattleRtnCode{
	eBATTLE_RTN_CONTINUE,
	eBATTLE_RTN_WIN,
	eBATTLE_RTN_LOSE,
};

typedef struct{
	int id;
	CPoint<int> pos;
}BattleEnemyInit_t;

class Battle{
	class StateBegin:public StateBase{
		static const int BEGIN_COUNTUP_VALUE = 256 / 30;// 30�J�E���g�ŃL�����`�抮��

		int drawNum;
		int actCount;
		Battle *obj;
	public:
		StateBegin(Battle *obj);
		~StateBegin();

		void Draw();
		void Process();
	};

	class StateChipSelect:public StateBase{
		static const int BT_SEND_NO = 11;// ���M�L�[�ԍ�

		class HandChip{
			std::vector<char> codes;
			std::vector<std::string> names;
			std::vector<ChipInfo> handValue;
			std::vector<unsigned> selectedIndex;

			char GetCode();// �I�����Ă���`�b�v�̋��ʃR�[�h��Ԃ�(�Ⴄ�ꍇ��'-')
			std::string GetFixedName();// �Œ薼(�������O�̃`�b�v���A�����Ă��邩)�����邩
			bool CanSelect(unsigned no);
		public:
			HandChip(){}
			~HandChip(){}

			void InitData(std::vector<ChipInfo> handValue);
			void Draw();
			void EraseLastValue();
			bool Select(unsigned no);
			std::vector<unsigned> GetSelectedChipIndex()const{ return selectedIndex; }
			ChipInfo GetChip(unsigned no);
			unsigned GetHandNum()const{ return handValue.size(); }
		};

		Battle *obj;
		int pointer;
		int imgFrame;
		int imgSelector[2];
		HandChip playerHand;
	public:
		StateChipSelect(Battle *obj);
		~StateChipSelect();

		void Draw();
		void Process();
	};

	class StateBattleStart:public StateBase{
		static const int DRAW_DELAY = 4;

		Battle *obj;
		int count;
		int imageTitle[3];
	public:
		StateBattleStart(Battle *obj);
		~StateBattleStart();

		void Draw();
		void Process();
	};

	class StateMainAction:public StateBase{
		Battle *obj;
	public:
		StateMainAction(Battle *obj);
		~StateMainAction();

		void Draw();
		void Process();
	};

	class StateWin:public StateBase{
		static const int VIEW_ITEM_COUNT = 20;

		typedef struct{
			enum{
				eTYPE_CHIP,
				eTYPE_ZENNY,
				// todo(hp,bug)
			};

			int type;
			std::string name;
			int value;
			int image;
		}ResultView_t;

		Battle *obj;
		int count;
		ResultView_t getItem;
		Mosaic mosaic;
		unsigned bustingLv;
		int imgZenny;
		int imgResultFrame;
	public:
		StateWin(Battle *obj);
		~StateWin();

		void Draw();
		void Process();
	};

	std::vector<ChipInfo> sendChip;
	BattleRtnCode rtnCode;
	StateMgr stateMgr;
	int mainActionTimeCount;
	bool isBoss;
public:
	Battle(std::list<BattleEnemyInit_t> enemyInfo, bool isBoss);
	~Battle();

	void Draw();
	BattleRtnCode Process();
};