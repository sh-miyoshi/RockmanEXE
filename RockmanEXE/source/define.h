#pragma once

#include <string>
#include "point.h"

namespace def{
	const int FMX = 480;
	const int FMY = 320;

	enum Muki{
		eMUKI_UP,
		eMUKI_RIGHT,
		eMUKI_DOWN,
		eMUKI_LEFT,

		eMUKI_UP_RIGHT,
		eMUKI_UP_LEFT,
		eMUKI_DOWN_RIGHT,
		eMUKI_DOWN_LEFT,

		eMUKI_MAX
	};

	enum BattleEnemyKind{
		eKIND_メットール1,
		eKIND_メットール3,
		eKIND_ビリー,
		eKIND_メットール_ビリー,
		eKIND_フォルテ,
		eKIND_戦歴,

		eKIND_MAX
	};

	enum charType{
		eCHAR_PLAYER = 0x1,
		eCHAR_ENEMY = 0x2,
		eCHAR_OBJECT = 0x4,

		eCHAR_ALL = eCHAR_PLAYER | eCHAR_ENEMY | eCHAR_OBJECT,
	};

	const std::string IMAGE_FILE_PATH = "data/image/";
	const std::string ENEMY_IMAGE_PATH = IMAGE_FILE_PATH + "敵関連/";
	const std::string SKILL_IMAGE_PATH = IMAGE_FILE_PATH + "技関連/";
	const std::string TEXT_FILE_PATH = "data/text/";
	const std::string BGM_FILE_PATH = "data/bgm/";

	const std::string RESOURCE_PASSWORD = "rockmanexepassword";// debug(Release時には消す)
}

// 色の定義
#define WHITE		(0xFFFFFF)
#define BLACK		(0x000000)
#define YELLOW		(0xFFFF00)
#define RED			(0xFF0000)
#define LIGHTGREEN	(0x00FF60)