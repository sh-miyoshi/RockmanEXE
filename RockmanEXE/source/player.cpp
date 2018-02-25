#include <algorithm>
#include <sstream>
#include "include.h"
#include "player.h"
#include "aes.h"
#include "battleCharMgr.h"
#include "drawCharacter.h"
#include "battleField.h"

// doNext(フレイムラインがない)
// todo(AnimShotはたぶんいらない)

namespace{
	const int DEFAULT_SHOT_POWER = 1;
	const int SAVE_DATA_SIZE = 512;
	const int SAVE_HASH_VALUE = 24071;
	const std::string SAVE_FILE_NAME = "save.dat";
	const int NORMAL_CHARGE_TIME = 2 * 60;
}

void BattlePlayer::ActMove::Begin(){
	ASSERT(obj->moveDirect != -1,"BattlePlayer::ActMove::Begin BattlePlayer.moveDirect is -1");
	obj->Move(obj->moveDirect, true);
}

void BattlePlayer::ActShot::Begin(){
	int sp = 0;
	if( obj->shotState == ST_CHARGE )
		sp = obj->shotPower * 10;
	else if( obj->shotState == ST_NORMAL )
		sp = obj->shotPower;
	else
		ASSERT(0,"BattlePlayer::ActShot::Begin Wrong Shot State ("+ToString<int>(obj->shotState)+")");
	SkillArg args;
	args.atkPos = obj->pos;
	args.damage = sp;
	BattleCharMgr::GetInst()->RegisterAttack(SkillMgr::GetData(SkillMgr::eID_バスター, args, def::eCHAR_PLAYER));
	obj->shotState = ST_NULL;
}

void BattlePlayer::ActDamage::Begin(){
	obj->chargeCount = 0;// チャージ中のやつをキャンセル
}

void BattlePlayer::DamageMutekiFunc(){
	// debug(対人戦の場合,スーパーアーマー状態)
	SetAnim(ANIM_DAMAGE);// ダメージを受けた時の処理(無敵処理)
	mutekiEndCnt = DEFAULT_MUTEKI_TIME;// 無敵になる時間をセット
}

BattlePlayer::BattlePlayer(int shotPower, int mindState, int hp, int hpMax, std::vector<ChipInfo> folder)
	:chargeMaxTime(NORMAL_CHARGE_TIME), shotPower(shotPower)
	, chargeCount(0), moveDirect(-1), mindState(mindState), shotState(ST_NULL)
	, BattleCharBase("ロックマン", hp, def::eCHAR_PLAYER), folder(folder)
{
	this->hpMax = hpMax;
	for( int i = 0; i < 16; i++ )
		imgCharge[i] = -1;

	anim[ANIM_ATK2].imageDelay = 5;// キャノンモーションのディレイ
	anim[ANIM_DAMAGE].imageDelay = 2;// ダメージを受けたときのモーションディレイ
	anim[ANIM_ATK3].imageDelay = 3;// ソード系のディレイ
	anim[ANIM_ATK4].imageDelay = 3;// ボム系のディレイ

	// アニメーションの設定
	anim[ANIM_MOVE].animProc = std::shared_ptr<ActMove>(new ActMove(this));
	anim[ANIM_ATK1].animProc = std::shared_ptr<ActShot>(new ActShot(this));
	anim[ANIM_DAMAGE].animProc = std::shared_ptr<ActDamage>(new ActDamage(this));
}

BattlePlayer::~BattlePlayer(){
}

void BattlePlayer::Draw(){
	if( hp <= 0 ){
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		int x = BattleField::PANEL_SIZE.x*pos.x + BattleField::PANEL_SIZE.x / 2;
		int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*pos.y - 10;
		int image = anim[ANIM_DAMAGE].image[1];
		// todo(アニメーションの追加)
		for( int i = 0; i < 10; i++ )
			DrawRotaGraph(x + drawPosOffset.x, y + drawPosOffset.y, 1, 0, image, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		return;
	}

	BattleCharBase::Draw();
	// チャージショット
	if( chargeCount > 20 ){
		int no = ( chargeCount < chargeMaxTime ) ? 0 : 8;
		int x = BattleField::PANEL_SIZE.x*pos.x + BattleField::PANEL_SIZE.x / 2;
		int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*pos.y;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 196);// 透ける表示
		DrawRotaGraph(x, y, 1, 0, imgCharge[no + ( chargeCount / 2 ) % 8], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void BattlePlayer::DrawSendChipIcon(){
	// キャラクタの上にチップアイコン、チップ名の表示
	if( !sendChips.empty() ){
		ChipData c = ChipMgr::GetInst()->GetChipData(sendChips[0]);
		DrawCharacter::GetInst()->DrawString(10, def::FMY - 20, c.name, WHITE, BLACK);
		int size = GetDrawStringWidth(c.name.c_str(), ( int ) c.name.size());
		DrawCharacter::GetInst()->DrawNumber(size + 30, def::FMY - 25, abs(c.power), DrawCharacter::COL_RED);

		const int px = 3;
		const int max = ( const int ) sendChips.size()*px;
		unsigned m = sendChips.size();
		for( unsigned i = 0; i < m; i++ ){
			int x = BattleField::PANEL_SIZE.x*pos.x + BattleField::PANEL_SIZE.x / 2 - 2 + ( i*px ) - max;
			int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*pos.y - 81 + ( i*px ) - max;
			DrawBox(x - 1, y - 1, x + 29, y + 29, BLACK, FALSE);
			// 後ろから順に描画
			c = ChipMgr::GetInst()->GetChipData(sendChips[m - 1 - i]);
			DrawGraph(x, y, c.imgIcon, TRUE);
		}
	}
}

void BattlePlayer::LoadAnim(){
	static const std::string position = "BattlePlayer::LoadAnim";

	std::string fname = def::IMAGE_FILE_PATH + "player_move.png";
	LoadDivGraphWithCheckError(anim[ANIM_MOVE].image, fname, position, 4, 1, 100, 100);
	anim[ANIM_MOVE].imageNum = 4;

	fname = def::IMAGE_FILE_PATH + "player_damage.png";
	LoadDivGraphWithCheckError(anim[ANIM_DAMAGE].image, fname, position, 4, 1, 100, 100);
	// 1 -> 2,3  2-4 3-5
	anim[ANIM_DAMAGE].image[4] = anim[ANIM_DAMAGE].image[2];
	anim[ANIM_DAMAGE].image[5] = anim[ANIM_DAMAGE].image[3];
	anim[ANIM_DAMAGE].image[2] = anim[ANIM_DAMAGE].image[1];
	anim[ANIM_DAMAGE].image[3] = anim[ANIM_DAMAGE].image[1];
	anim[ANIM_DAMAGE].imageNum = 6;

	fname = def::IMAGE_FILE_PATH + "player_shot.png";
	LoadDivGraphWithCheckError(anim[ANIM_ATK1].image, fname, position, 6, 1, 180, 100);
	anim[ANIM_ATK1].imageNum = 6;
	// チャージ画像
	fname = def::IMAGE_FILE_PATH + "charge.png";
	LoadDivGraphWithCheckError(imgCharge, fname, position, 8, 2, 158, 150);

	fname = def::IMAGE_FILE_PATH + "player_cannon.png";
	LoadDivGraphWithCheckError(anim[ANIM_ATK2].image, fname, position, 4, 1, 100, 100);
	anim[ANIM_ATK2].imageNum = 4;

	fname = def::IMAGE_FILE_PATH + "player_sword.png";
	LoadDivGraphWithCheckError(anim[ANIM_ATK3].image, fname, position, 7, 1, 128, 128);
	anim[ANIM_ATK3].imageNum = 7;

	fname = def::IMAGE_FILE_PATH + "player_bomb.png";
	LoadDivGraphWithCheckError(anim[ANIM_ATK4].image, fname, position, 5, 1, 100, 114);
	anim[ANIM_ATK4].imageNum = 5;

	// ココロ状態画像
	fname = def::IMAGE_FILE_PATH + "mind_status.png";
	int temp[18];
	LoadDivGraphWithCheckError(temp, fname, position, 6, 3, 88, 32);
}

void BattlePlayer::DeleteAnim(){
	BattleCharBase::DeleteAnim();

	for( int i = 0; i < sizeof(imgCharge) / sizeof(imgCharge[0]); i++ )
		DeleteGraph(imgCharge[i]);
}

void BattlePlayer::Process(){
	BattleCharBase::Process();

	// デリートモーション中(操作不可)
	if( hp <= 0 ){
		return;
	}

	if( AnimProcess() ){// 行動できるなら
		if( shotState != ST_NULL )// ショットを打つなら
			SetAnim(ANIM_ATK1);
		else{// それ以外ならアクション入力待ち
			CKey *in = CKey::GetInst();
			// チップを使用する処理
			if( in->CheckKey(eKEY_ENTER) == 1 && !sendChips.empty() ){
				ChipData c = ChipMgr::GetInst()->GetChipData(sendChips[0]);
				if( c.playerAct != -1 )
					SetAnim(c.playerAct);
				SkillArg args;
				args.atkPos = pos;
				BattleCharMgr::GetInst()->RegisterAttack(SkillMgr::GetData(c, args, def::eCHAR_PLAYER));
				sendChips.erase(sendChips.begin());// 使ったチップを削除
				return;
			}

			// 行動関連
			if( in->CheckKey(eKEY_UP) == 1 ){
				moveDirect = def::eMUKI_UP;
				if( Move(def::eMUKI_UP, false) )
					SetAnim(ANIM_MOVE);
			} else if( in->CheckKey(eKEY_DOWN) == 1 ){
				moveDirect = def::eMUKI_DOWN;
				if( Move(def::eMUKI_DOWN, false) )
					SetAnim(ANIM_MOVE);
			} else if( in->CheckKey(eKEY_LEFT) == 1 ){
				moveDirect = def::eMUKI_LEFT;
				if( Move(def::eMUKI_LEFT, false) )
					SetAnim(ANIM_MOVE);
			} else if( in->CheckKey(eKEY_RIGHT) == 1 ){
				moveDirect = def::eMUKI_RIGHT;
				if( Move(def::eMUKI_RIGHT, false) )
					SetAnim(ANIM_MOVE);
			}

			// ショット関連
			if( in->CheckKey(eKEY_CANCEL) != 0 )// キャンセルキーでショット
				chargeCount++;
			else if( chargeCount > 0 ){
				if( chargeCount >= chargeMaxTime )
					shotState = ST_CHARGE;
				else
					shotState = ST_NORMAL;
				chargeCount = 0;
			}
		}
	}
}

std::vector<ChipInfo> BattlePlayer::GetHandData(unsigned max){
	std::vector<ChipInfo> result;
	if( max > folder.size() )
		max = folder.size();
	for( unsigned i = 0; i < max; i++ ){
		result.push_back(folder[i]);
	}
	return result;
}

void BattlePlayer::SendChips(std::vector<unsigned> selectedIndexed){
	sendChips.clear();
	for( auto select : selectedIndexed ){
		std::string msg = "BattlePlayer::SendChips ";
		msg += "select chip is out of folder (";
		for( auto s : selectedIndexed ){
			msg += ToString<unsigned>(s) + " ";
		}
		msg += ")";
		ASSERT(select < folder.size(),msg);
		sendChips.push_back(folder[select]);
	}

	unsigned i = 0;
	for( std::vector<ChipInfo>::iterator it = folder.begin(); it != folder.end(); i++ ){
		if( find<unsigned>(selectedIndexed, i) )
			it = folder.erase(it);
		else
			it++;
	}
}

void BattlePlayer::ShuffleFolder(){
	// debug(random_shuffle関数は非推奨)
	std::random_shuffle(folder.begin(), folder.end());
}


void PlayerMgr::ResetPlayerInfo(){
	folder.clear();
	for( int i = 0; i < def::eKIND_MAX - 1; i++ )
		for( int j = 0; j < eBT_RTN_MAX; j++ )
			battleResult[i][j] = 0;
	money = 0;
	hp = 0;
	hpMax = 0;
	shotPower = 0;
	mindState = 0;
}

int PlayerMgr::GetBattleResult(BattleResult ret, int battleEnemyKind){
	std::string msg = "PlayerMgr::GetBattleResult ";
	msg += "wrong enemy kind selected from main char select (";
	msg += ToString<int>(battleEnemyKind)+")";
	ASSERT(0 <= battleEnemyKind && battleEnemyKind < def::eKIND_MAX,msg);
	return battleResult[battleEnemyKind][ret];
}

void PlayerMgr::IncrementBattleResult(BattleResult ret, int battleEnemyKind){
	std::string msg = "PlayerMgr::IncrementBattleResult ";
	msg += "wrong enemy kind selected from main char select (";
	msg += ToString<int>(battleEnemyKind) + ")";
	ASSERT(0 <= battleEnemyKind && battleEnemyKind < def::eKIND_MAX, msg);
	battleResult[battleEnemyKind][ret]++;
	Save();// debug? 現在オートセーブ
}

void PlayerMgr::Save(){
	std::stringstream ss;
	// intが続くときは区切り文字として'#'を挿入
	ss << money << '#';
	for( int i = 0; i < FOLDER_NUM; i++ )
		ss << folder[i].id << folder[i].code;
	ss << hp << '#';
	ss << hpMax << '#';
	ss << shotPower << '#';
	ss << mindState << '#';
	int result_val = 0;
	for( int i = 0; i < def::eKIND_MAX - 1; i++ ){
		for( int j = 0; j < eBT_RTN_MAX; j++ ){
			ss << battleResult[i][j] << '#';
			result_val += battleResult[i][j];
		}
	}

	// 適当な素数で割ったハッシュも保存
	int hash = money + hp + hpMax + shotPower + mindState;
	hash += result_val;
	hash %= SAVE_HASH_VALUE;
	ss << hash;

	// sizeof(int)*7+(sizeof(int)+sizeof(char))*FOLDER_NUM=318[byte]
	char buf[SAVE_DATA_SIZE], ret[SAVE_DATA_SIZE];
	memset(buf, 0, sizeof(buf));
	strcpy_s(buf, ss.str().c_str());// データのセット
	AES aes(def::RESOURCE_PASSWORD, 128);
	__m128i iv = _mm_setzero_si128();
	for( int i = 0; i < SAVE_DATA_SIZE / 16; i++ ){
		char t[16] = { 0 };
		for( int j = 0; j < 16; j++ )
			t[j] = buf[i * 16 + j];
		__m128i data = _mm_loadu_si128(( __m128i * )t);
		data = aes.Encrypt_CBC(data, iv);
		_mm_storeu_si128(( __m128i * )( ret + ( i * 16 ) ), data);
	}
	FILE *fp;
	fopen_s(&fp, SAVE_FILE_NAME.c_str(), "wb");
	CheckFileOpen(fp, "PlayerMgr::Save", SAVE_FILE_NAME);
	fwrite(ret, sizeof(char), SAVE_DATA_SIZE, fp);
	fclose(fp);
}

void PlayerMgr::CreateNewPlayer(){
	ResetPlayerInfo();

	//	hp = 1;// debug用ステータス
	hp = 300;
	hpMax = 300;
	shotPower = DEFAULT_SHOT_POWER;
	money = 0;
	mindState = MIND_通常;

	// フォルダー初期化
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
		{ ChipMgr::eID_ミニボム,'B' },
		{ ChipMgr::eID_ミニボム,'B' },
		{ ChipMgr::eID_ミニボム,'*' },
		{ ChipMgr::eID_ミニボム,'*' },
		{ ChipMgr::eID_ソード,'S' },
		{ ChipMgr::eID_ソード,'S' },
		{ ChipMgr::eID_ソード,'S' },
		{ ChipMgr::eID_ソード,'S' },
		{ ChipMgr::eID_リカバリー10,'*' },
		{ ChipMgr::eID_リカバリー10,'*' },
		{ ChipMgr::eID_フレイムライン,'F' },
		{ ChipMgr::eID_フレイムライン,'F' },
		{ ChipMgr::eID_フレイムライン,'C' },
		{ ChipMgr::eID_フレイムライン,'C' },
		{ ChipMgr::eID_リカバリー30,'*' },
		{ ChipMgr::eID_リカバリー30,'*' },
		{ ChipMgr::eID_リカバリー30,'C' },
		{ ChipMgr::eID_リカバリー30,'C' },
		{ ChipMgr::eID_サンダーボール,'C' },
		{ ChipMgr::eID_サンダーボール,'C' },
		{ ChipMgr::eID_ワイドソード,'S' },
		{ ChipMgr::eID_ワイドソード,'S' }
	};
	for( int i = 0; i < FOLDER_NUM; i++ )
		folder.push_back(f[i]);
}

void PlayerMgr::ContinueWithSaveFile(){
	ResetPlayerInfo();

	char buf[SAVE_DATA_SIZE], ret[SAVE_DATA_SIZE];
	FILE *fp;
	fopen_s(&fp, SAVE_FILE_NAME.c_str(), "rb");
	CheckFileOpen(fp, "PlayerMgr::ContinueWithSaveFile", SAVE_FILE_NAME);
	fread(buf, sizeof(char), SAVE_DATA_SIZE, fp);
	fclose(fp);

	AES aes(def::RESOURCE_PASSWORD, 128);
	__m128i iv = _mm_setzero_si128();
	for( int i = 0; i < SAVE_DATA_SIZE / 16; i++ ){
		char t[16] = { 0 };
		for( int j = 0; j < 16; j++ )
			t[j] = buf[i * 16 + j];
		__m128i data = _mm_loadu_si128(( __m128i * )t);
		data = aes.Decrypt_CBC(data, iv);
		_mm_storeu_si128(( __m128i * )( ret + ( i * 16 ) ), data);
	}

	char dummy;
	std::istringstream is(ret);
	is >> money >> dummy;
	for( int i = 0; i < FOLDER_NUM; i++ ){
		ChipInfo folder_tmp;
		is >> folder_tmp.id >> folder_tmp.code;
		folder.push_back(folder_tmp);
	}
	is >> hp >> dummy;
	is >> hpMax >> dummy;
	is >> shotPower >> dummy;
	is >> mindState >> dummy;
	int result_val = 0;
	for( int i = 0; i < def::eKIND_MAX - 1; i++ ){
		for( int j = 0; j < eBT_RTN_MAX; j++ ){
			is >> battleResult[i][j] >> dummy;
			result_val += battleResult[i][j];
		}
	}

	int hash, calc_hash;
	calc_hash = money + hp + hpMax + shotPower + mindState;
	calc_hash += result_val;
	calc_hash %= SAVE_HASH_VALUE;
	is >> hash;
	if( hash != calc_hash ){
		std::string message;
		message += "セーブファイルに不正な改ざんがありました。";
		message += SAVE_FILE_NAME + "を削除して最初から開始してください";
		AppLogger::Log(AppLogger::eLOG_ERROR, message);
		exit(1);
	}
}

bool PlayerMgr::IsContinueOK(){
	return FileExist(SAVE_FILE_NAME);
}

std::shared_ptr<BattlePlayer> PlayerMgr::CreateBattlePlayer(){
	return std::shared_ptr<BattlePlayer>(new BattlePlayer(shotPower, mindState, hp, hpMax, folder));
}
