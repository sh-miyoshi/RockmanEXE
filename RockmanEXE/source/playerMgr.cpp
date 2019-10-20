#include <random>
#include "include.h"
#include "playerMgr.h"
#include "battleField.h"
#include "battleSkillMgr.h"
#include "drawCharacter.h"

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

void PlayerMgr::CreateNewPlayer() {
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

bool PlayerMgr::IsContinueOK() {
	// TODO(未実装)
	return false;
}

BattlePlayer::BattlePlayer(std::string name, unsigned int hp, unsigned int hpMax, unsigned int busterPower, std::vector<ChipInfo> chipFolder)
	: BattleCharBase(name, hp, hpMax, eCHAR_PLAYER), chargeCount(0), chargeMaxTime(DEFAULT_CHARGE_TIME),
	busterPower(busterPower), chipFolder(chipFolder), mindStatus(eSTATUS_通常) {

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
	anim[eANIM_CANNON]->LoadData(fname, CPoint<unsigned int>(100, 100), CPoint<unsigned int>(6, 1), 4, 1);

	fname = def::CHARACTER_IMAGE_PATH + "player_sword.png";
	anim[eANIM_SWORD] = std::shared_ptr<Animation>(new Animation());
	anim[eANIM_SWORD]->LoadData(fname, CPoint<unsigned int>(128, 128), CPoint<unsigned int>(7, 1), 3);

	fname = def::CHARACTER_IMAGE_PATH + "player_bomb.png";
	anim[eANIM_BOMB] = std::shared_ptr<Animation>(new Animation());
	anim[eANIM_BOMB]->LoadData(fname, CPoint<unsigned int>(100, 114), CPoint<unsigned int>(5, 1), 3);

	// チャージ画像の読み込み
	fname = def::SKILL_IMAGE_PATH + "ロックバスター_charge.png";
	LoadDivGraphWithErrorCheck(imgCharge, fname, "BattlePlayer::BattlePlayer", 8, 2, 158, 150);

	// フォルダーのセットアップ
	std::shuffle(this->chipFolder.begin(), this->chipFolder.end(), rnd_generator);
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

	// チップの描画
	if( !sendChipList.empty() ) {
		auto it = sendChipList.begin();
		ChipData c = ChipMgr::GetInst()->GetChipData(it->id);
		DrawCharacter::GetInst()->DrawString(10, def::FMY - 25, c.name, WHITE, BLACK);
		int size = GetDrawStringWidth(c.name.c_str(), ( int ) c.name.size());
		DrawCharacter::GetInst()->DrawNumber(size + 20, def::FMY - 25, c.power, DrawCharacter::COL_RED);

		const unsigned int px = 3;
		const unsigned int max = sendChipList.size() * px;
		auto rit = sendChipList.rbegin();
		for( unsigned int i = 0; i < sendChipList.size(); i++ ) {
			if( rit == sendChipList.rend() ) {
				AppLogger::Warn("SendChipをすべて描画できていません i: %d, max: %d", i, sendChipList.size());
				break;
			}
			CPoint<int> tpos = BattleField::GetPixelPos(pos);
			tpos.x += ( i * px ) - 2 - max;
			tpos.y += ( i * px ) - 81 - max;
			DrawBox(tpos.x - 1, tpos.y - 1, tpos.x + 29, tpos.y + 29, BLACK, FALSE);
			// 後ろから順に描画
			c = ChipMgr::GetInst()->GetChipData(rit->id);
			DrawGraph(tpos.x, tpos.y, c.imgIcon, TRUE);
			rit++;
		}
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
#if 0
			if( !sendChipList.empty() ) {
				auto it = sendChipList.begin();
				ChipData c = ChipMgr::GetInst()->GetChipData(it->id);
				// TODO(暗転処理)
				SkillArg arg;
				arg.charPos = pos;
				arg.power = c.power;
				arg.myCharType = eCHAR_PLAYER;
				BattleSkillMgr::GetInst()->Register(SkillMgr::GetData(c, arg));
				if( c.playerAct != eANIM_NONE ) {
					this->AttachAnim(anim[c.playerAct]);
				}
				sendChipList.pop_front();
			}
#else
			// debug(デバッグ用処理: 特定のチップを使い続ける)
			ChipData c = ChipMgr::GetInst()->GetChipData(ChipMgr::eID_キャノン);
			SkillArg arg;
			arg.charPos = pos;
			arg.power = c.power;
			arg.myCharType = eCHAR_PLAYER;
			BattleSkillMgr::GetInst()->Register(SkillMgr::GetData(c, arg));
			if( c.playerAct != eANIM_NONE ) {
				this->AttachAnim(anim[c.playerAct]);
			}
#endif
		}
	}
}

std::vector<ChipInfo> BattlePlayer::GetHandData(unsigned max) {
	std::vector<ChipInfo> result;
	if( max > chipFolder.size() )
		max = chipFolder.size();
	for( unsigned i = 0; i < max; i++ ) {
		// この時点ではfolderから削除はしない(削除は実際に使った時だけ)
		result.push_back(chipFolder[i]);
	}
	return result;
}

void BattlePlayer::SetSendChipList(std::vector<int> selectedIndexes) {
	std::list<ChipInfo> chipList;
	for( auto si : selectedIndexes ) {
		chipList.push_back(chipFolder[si]);
	}

	std::string msg = "[ ";
	for( auto c : chipList ) {
		msg += ToString("( ID: %d, Code: %c ), ", c.id, c.code);
	}
	msg += "]";
	AppLogger::Info("Send Chips %s to BattleStateMain", msg.c_str());
	this->sendChipList = chipList;

	// ここで選択したチップは削除する
	// chipFolderから適切にindexで削除するために後ろから順に削除する
	std::sort(selectedIndexes.begin(), selectedIndexes.end(), std::greater<int>());//降順ソート
	for( auto si : selectedIndexes ) {
		chipFolder.erase(chipFolder.begin() + si);
	}
}
