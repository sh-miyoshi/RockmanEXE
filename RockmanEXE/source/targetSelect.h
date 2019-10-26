#pragma once

#include <vector>
#include <string>
#include "fade.h"
#include "selecter.h"
#include "point.h"
#include "enemy.h"
#include "battle.h"

class TargetSelect {
	enum {
		eTRG_メットールx1,

		eTRG_MAX
	};

	class CharInfo {
		EnemyMgr::EnemyID id;
		CPoint<int> viewPos;
		CPoint<int> battleInitPos;
	public:

		CharInfo(EnemyMgr::EnemyID id, CPoint<int> viewPos, CPoint<int> battleInitPos);
		~CharInfo();

		void Draw();
		Battle::EnemyInfo GetInfo();
	};

	class TargetInfo {
	public:
		std::string name;
		std::vector<CharInfo> info;

		TargetInfo(std::string name, std::vector<CharInfo> info)
			:name(name), info(info) {
		}
		~TargetInfo() {}
	};

	Selecter selecter;
	std::vector<TargetInfo> targetInfo;
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
	std::vector<Battle::EnemyInfo> GetEnemyInfo();
};