#pragma once

#include <string>
#include "point.h"

namespace def {
	const int FMX = 480;
	const int FMY = 320;

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

	const std::string RESOURCE_PASSWORD = "rockmanexepassword";// debug(Release時には削除)
}

// 色の定義
constexpr auto WHITE = 0xFFFFFF;
constexpr auto BLACK = 0x000000;
constexpr auto YELLOW = 0xFFFF00;
constexpr auto RED = 0xFF0000;
constexpr auto LIGHTGREEN = 0x00FF60;