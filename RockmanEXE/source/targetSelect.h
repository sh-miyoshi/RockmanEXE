#pragma once

#include <vector>
#include <string>
#include "fade.h"
#include "selecter.h"
#include "point.h"

class TargetSelect {
	enum {
		eTRG_ÉÅÉbÉgÅ[Éãx1,

		eTRG_MAX
	};

	class EnemyInfo{
	public:
		int id;// TODO(use enum)
		CPoint<int> viewPos;
		CPoint<int> battleInitPos;

		EnemyInfo(int id, CPoint<int> viewPos, CPoint<int> battleInitPos)
			:id(id), viewPos(viewPos), battleInitPos(battleInitPos){}
		~EnemyInfo(){}
	};

	class TargetInfo{
	public:
		std::string name;
		std::vector<EnemyInfo> info;

		TargetInfo(std::string name, std::vector<EnemyInfo> info)
			:name(name), info(info){}
		~TargetInfo(){}
	};

	Selecter selecter;
public:
	enum RtnCode {
		eRTN_CONTINUE,
		eRTN_START_BATTLE,
		eRTN_MY_INFO
	};

	TargetSelect();
	~TargetSelect();

	void Draw();
	RtnCode Process();
	// TODO(GetEnemyInfo())
};