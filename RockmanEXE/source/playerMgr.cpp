#include "include.h"
#include "playerMgr.h"
#include "battleField.h"
#include "battleSkillMgr.h"

namespace {
	const unsigned int DEFAULT_CHARGE_TIME = 2 * 60;// 2[second]
}

PlayerMgr::PlayerMgr()
	:name("ロックマン"), hp(0), hpMax(0), battlePlayer(nullptr), busterPower(1) {
}

PlayerMgr::~PlayerMgr() {
	if( battlePlayer ) {
		delete battlePlayer;
		battlePlayer = nullptr;
	}
}

void PlayerMgr::InitBattleChar() {
	if( battlePlayer ) {
		delete battlePlayer;
	}

	battlePlayer = new BattlePlayer(name, hp, hpMax, busterPower, chipFolder);
	battlePlayer->SetPos(1, 1);// 初期位置のセット
}

void PlayerMgr::InitPlayer() {
	// 初期データをセット
	hp = hpMax = 100;
	name = "ロックマン";
	busterPower = 1;

	// 初期フォルダーの設定
	ChipInfo f[FOLDER_NUM] = {
		// id,code
		{ ChipMgr::eID_キャノン,'A' },
		{ ChipMgr::eID_キャノン,'A' },
		{ ChipMgr::eID_キャノン,'B' },
		{ ChipMgr::eID_キャノン,'B' },
		{ ChipMgr::eID_ハイキャノン,'D' },

		{ ChipMgr::eID_ハイキャノン,'D' },
		{ ChipMgr::eID_ハイキャノン,'E' },
		{ ChipMgr::eID_ハイキャノン,'E' },
		{ ChipMgr::eID_ハイキャノン,'*' },
		{ ChipMgr::eID_ハイキャノン,'*' },

		{ ChipMgr::eID_キャノン,'A' },
		{ ChipMgr::eID_キャノン,'A' },
		{ ChipMgr::eID_キャノン,'A' },
		{ ChipMgr::eID_キャノン,'F' },
		{ ChipMgr::eID_キャノン,'F' },

		{ ChipMgr::eID_キャノン,'F' },
		{ ChipMgr::eID_キャノン,'Y' },
		{ ChipMgr::eID_キャノン,'Y' },
		{ ChipMgr::eID_キャノン,'Y' },
		{ ChipMgr::eID_キャノン,'Y' },

		{ ChipMgr::eID_キャノン,'*' },
		{ ChipMgr::eID_キャノン,'*' },
		{ ChipMgr::eID_キャノン,'*' },
		{ ChipMgr::eID_キャノン,'*' },
		{ ChipMgr::eID_キャノン,'*' },
	};
	for( int i = 0; i < FOLDER_NUM; i++ )
		chipFolder.push_back(f[i]);
}

BattlePlayer::BattlePlayer(std::string name, unsigned int hp, unsigned int hpMax, unsigned int busterPower, std::vector<ChipInfo> chipFolder)
	: BattleCharBase(name, hp, hpMax, eCHAR_PLAYER), chargeCount(0), chargeMaxTime(DEFAULT_CHARGE_TIME),
	busterPower(busterPower), chipFolder(chipFolder) {

	// TODO(chargeMaxTimeを変えられるようにする)

	// アニメーションの設定
	std::string fname = def::CHARACTER_IMAGE_PATH + "player_stand.png";
	std::shared_ptr<Animation> animStand = std::shared_ptr<Animation>(new Animation());
	animStand->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(1, 1));
	BattleCharBase::SetDefaultAnim(animStand);

	anim[eANIM_NONE] = std::shared_ptr<Animation>(new Animation());

	fname = def::CHARACTER_IMAGE_PATH + "player_move.png";
	anim[eANIM_MOVE] = std::shared_ptr<Animation>(new Animation());
	anim[eANIM_MOVE]->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(4, 1));

	fname = def::CHARACTER_IMAGE_PATH + "player_shot.png";
	anim[eANIM_SHOT] = std::shared_ptr<Animation>(new Animation());
	anim[eANIM_SHOT]->LoadData(fname, CPoint<unsigned int>(180, 100), CPoint<unsigned int>(6, 1));

	fname = def::CHARACTER_IMAGE_PATH + "player_cannon.png";
	anim[eANIM_CANNON] = std::shared_ptr<Animation>(new Animation());
	anim[eANIM_CANNON]->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(4, 1), 5);

	fname = def::CHARACTER_IMAGE_PATH + "player_sword.png";
	anim[eANIM_SWORD] = std::shared_ptr<Animation>(new Animation());
	anim[eANIM_SWORD]->LoadData(fname, CPoint<unsigned int>(128, 128), CPoint<unsigned int>(7, 1), 3);

	fname = def::CHARACTER_IMAGE_PATH + "player_bomb.png";
	anim[eANIM_BOMB] = std::shared_ptr<Animation>(new Animation());
	anim[eANIM_BOMB]->LoadData(fname, CPoint<unsigned int>(100, 114), CPoint<unsigned int>(5, 1), 3);

	// チャージ画像の読み込み
	fname = def::SKILL_IMAGE_PATH + "ロックバスター_charge.png";
	LoadDivGraphWithErrorCheck(imgCharge, fname, "BattlePlayer::BattlePlayer", 8, 2, 158, 150);
}

BattlePlayer::~BattlePlayer() {
	for( int i = 0; i < sizeof(imgCharge) / sizeof(imgCharge[0]); i++ )
		DeleteGraph(imgCharge[i]);
}

void BattlePlayer::Draw() {
	BattleCharBase::Draw();

	if( chargeCount > 20 ) {
		int no = ( chargeCount < chargeMaxTime ) ? 0 : 8;
		CPoint<int> t = BattleField::GetPixelPos(pos);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 196);// 透ける表示
		DrawRotaGraph(t.x, t.y, 1, 0, imgCharge[no + ( chargeCount / 3 ) % 8], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void BattlePlayer::Process() {
	if( AnimProcess() ) {
		// 移動処理
		if( CKey::GetInst()->CheckKey(eKEY_DOWN) == 1 ) {
			if( MoveCheck(pos.x, pos.y + 1) ) {
				this->SetPos(pos.x, pos.y + 1);
				this->AttachAnim(anim[eANIM_MOVE]);
			}
		} else if( CKey::GetInst()->CheckKey(eKEY_UP) == 1 ) {
			if( MoveCheck(pos.x, pos.y - 1) ) {
				this->SetPos(pos.x, pos.y - 1);
				this->AttachAnim(anim[eANIM_MOVE]);
			}
		} else if( CKey::GetInst()->CheckKey(eKEY_LEFT) == 1 ) {
			if( MoveCheck(pos.x - 1, pos.y) ) {
				this->SetPos(pos.x - 1, pos.y);
				this->AttachAnim(anim[eANIM_MOVE]);
			}
		} else if( CKey::GetInst()->CheckKey(eKEY_RIGHT) == 1 ) {
			if( MoveCheck(pos.x + 1, pos.y) ) {
				this->SetPos(pos.x + 1, pos.y);
				this->AttachAnim(anim[eANIM_MOVE]);
			}
		}

		// ロックバスター関連
		if( CKey::GetInst()->CheckKey(eKEY_CANCEL) != 0 )// キャンセルキーでショット
			chargeCount++;
		else if( chargeCount > 0 ) {
			// チャージショット
			SkillArg arg;
			arg.charPos = pos;
			arg.power = busterPower;
			if( chargeCount >= chargeMaxTime ) {
				arg.power *= 10;
			}
			arg.myCharType = eCHAR_PLAYER;
			BattleSkillMgr::GetInst()->Register(SkillMgr::GetData(SkillMgr::eID_バスター, arg));
			this->AttachAnim(anim[eANIM_SHOT]);
			chargeCount = 0;
		}

		// チップを使う
		if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 ) {
			// debug(とりあえず今はキャノンを無限打ちする)
			SkillArg arg;
			arg.charPos = pos;
			arg.power = 40;
			arg.myCharType = eCHAR_PLAYER;
			BattleSkillMgr::GetInst()->Register(SkillMgr::GetData(SkillMgr::eID_キャノン, arg));
			this->AttachAnim(anim[eANIM_CANNON]);
		}
	}
}
