#pragma once

#include <string>
#include "state.h"
#include "selectMgr.h"

// ���L�����̏���\������N���X
class MyInfo{
	// �g�b�v���j���[��\������N���X
	class StateTop:public StateBase{
		enum Item{
			eITEM_���b�N�}��,
			eITEM_���,
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

	// �L��������\������N���X
	class StateRockman:public StateBase{
		MyInfo *obj;
		int image;
	public:
		StateRockman(MyInfo *obj);
		~StateRockman();

		void Draw();
		void Process();
	};

	// ��т�\������N���X
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