#pragma once

#include <string>
#include "point.h"

namespace def {
	const int FMX = 480;
	const int FMY = 320;

	const CPoint<int> BATTLE_PANEL_SIZE(80, 50);
	const int BATTLE_PANEL_OFFSET_Y = def::FMY - ( BATTLE_PANEL_SIZE.y * 3 ) - 30;

	enum Muki {
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

	const std::string IMAGE_FILE_PATH = "data/image_private/";// for local development
	//const std::string IMAGE_FILE_PATH = "data/image_public/";
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