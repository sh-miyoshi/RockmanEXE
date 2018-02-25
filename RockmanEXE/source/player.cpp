#include <algorithm>
#include <sstream>
#include "include.h"
#include "player.h"
#include "aes.h"
#include "battleCharMgr.h"
#include "drawCharacter.h"
#include "battleField.h"

// doNext(�t���C�����C�����Ȃ�)
// todo(AnimShot�͂��Ԃ񂢂�Ȃ�)

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
	BattleCharMgr::GetInst()->RegisterAttack(SkillMgr::GetData(SkillMgr::eID_�o�X�^�[, args, def::eCHAR_PLAYER));
	obj->shotState = ST_NULL;
}

void BattlePlayer::ActDamage::Begin(){
	obj->chargeCount = 0;// �`���[�W���̂���L�����Z��
}

void BattlePlayer::DamageMutekiFunc(){
	// debug(�ΐl��̏ꍇ,�X�[�p�[�A�[�}�[���)
	SetAnim(ANIM_DAMAGE);// �_���[�W���󂯂����̏���(���G����)
	mutekiEndCnt = DEFAULT_MUTEKI_TIME;// ���G�ɂȂ鎞�Ԃ��Z�b�g
}

BattlePlayer::BattlePlayer(int shotPower, int mindState, int hp, int hpMax, std::vector<ChipInfo> folder)
	:chargeMaxTime(NORMAL_CHARGE_TIME), shotPower(shotPower)
	, chargeCount(0), moveDirect(-1), mindState(mindState), shotState(ST_NULL)
	, BattleCharBase("���b�N�}��", hp, def::eCHAR_PLAYER), folder(folder)
{
	this->hpMax = hpMax;
	for( int i = 0; i < 16; i++ )
		imgCharge[i] = -1;

	anim[ANIM_ATK2].imageDelay = 5;// �L���m�����[�V�����̃f�B���C
	anim[ANIM_DAMAGE].imageDelay = 2;// �_���[�W���󂯂��Ƃ��̃��[�V�����f�B���C
	anim[ANIM_ATK3].imageDelay = 3;// �\�[�h�n�̃f�B���C
	anim[ANIM_ATK4].imageDelay = 3;// �{���n�̃f�B���C

	// �A�j���[�V�����̐ݒ�
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
		// todo(�A�j���[�V�����̒ǉ�)
		for( int i = 0; i < 10; i++ )
			DrawRotaGraph(x + drawPosOffset.x, y + drawPosOffset.y, 1, 0, image, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		return;
	}

	BattleCharBase::Draw();
	// �`���[�W�V���b�g
	if( chargeCount > 20 ){
		int no = ( chargeCount < chargeMaxTime ) ? 0 : 8;
		int x = BattleField::PANEL_SIZE.x*pos.x + BattleField::PANEL_SIZE.x / 2;
		int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*pos.y;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 196);// ������\��
		DrawRotaGraph(x, y, 1, 0, imgCharge[no + ( chargeCount / 2 ) % 8], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void BattlePlayer::DrawSendChipIcon(){
	// �L�����N�^�̏�Ƀ`�b�v�A�C�R���A�`�b�v���̕\��
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
			// ��납�珇�ɕ`��
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
	// �`���[�W�摜
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

	// �R�R����ԉ摜
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

	// �f���[�g���[�V������(����s��)
	if( hp <= 0 ){
		return;
	}

	if( AnimProcess() ){// �s���ł���Ȃ�
		if( shotState != ST_NULL )// �V���b�g��łȂ�
			SetAnim(ANIM_ATK1);
		else{// ����ȊO�Ȃ�A�N�V�������͑҂�
			CKey *in = CKey::GetInst();
			// �`�b�v���g�p���鏈��
			if( in->CheckKey(eKEY_ENTER) == 1 && !sendChips.empty() ){
				ChipData c = ChipMgr::GetInst()->GetChipData(sendChips[0]);
				if( c.playerAct != -1 )
					SetAnim(c.playerAct);
				SkillArg args;
				args.atkPos = pos;
				BattleCharMgr::GetInst()->RegisterAttack(SkillMgr::GetData(c, args, def::eCHAR_PLAYER));
				sendChips.erase(sendChips.begin());// �g�����`�b�v���폜
				return;
			}

			// �s���֘A
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

			// �V���b�g�֘A
			if( in->CheckKey(eKEY_CANCEL) != 0 )// �L�����Z���L�[�ŃV���b�g
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
	// debug(random_shuffle�֐��͔񐄏�)
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
	Save();// debug? ���݃I�[�g�Z�[�u
}

void PlayerMgr::Save(){
	std::stringstream ss;
	// int�������Ƃ��͋�؂蕶���Ƃ���'#'��}��
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

	// �K���ȑf���Ŋ������n�b�V�����ۑ�
	int hash = money + hp + hpMax + shotPower + mindState;
	hash += result_val;
	hash %= SAVE_HASH_VALUE;
	ss << hash;

	// sizeof(int)*7+(sizeof(int)+sizeof(char))*FOLDER_NUM=318[byte]
	char buf[SAVE_DATA_SIZE], ret[SAVE_DATA_SIZE];
	memset(buf, 0, sizeof(buf));
	strcpy_s(buf, ss.str().c_str());// �f�[�^�̃Z�b�g
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

	//	hp = 1;// debug�p�X�e�[�^�X
	hp = 300;
	hpMax = 300;
	shotPower = DEFAULT_SHOT_POWER;
	money = 0;
	mindState = MIND_�ʏ�;

	// �t�H���_�[������
	ChipInfo f[FOLDER_NUM] = {
		// id,code
		{ ChipMgr::eID_�L���m��,'A' },
		{ ChipMgr::eID_�L���m��,'A' },
		{ ChipMgr::eID_�L���m��,'B' },
		{ ChipMgr::eID_�L���m��,'B' },
		{ ChipMgr::eID_�n�C�L���m��,'D' },
		{ ChipMgr::eID_�n�C�L���m��,'D' },
		{ ChipMgr::eID_�n�C�L���m��,'E' },
		{ ChipMgr::eID_�n�C�L���m��,'E' },
		{ ChipMgr::eID_�~�j�{��,'B' },
		{ ChipMgr::eID_�~�j�{��,'B' },
		{ ChipMgr::eID_�~�j�{��,'*' },
		{ ChipMgr::eID_�~�j�{��,'*' },
		{ ChipMgr::eID_�\�[�h,'S' },
		{ ChipMgr::eID_�\�[�h,'S' },
		{ ChipMgr::eID_�\�[�h,'S' },
		{ ChipMgr::eID_�\�[�h,'S' },
		{ ChipMgr::eID_���J�o���[10,'*' },
		{ ChipMgr::eID_���J�o���[10,'*' },
		{ ChipMgr::eID_�t���C�����C��,'F' },
		{ ChipMgr::eID_�t���C�����C��,'F' },
		{ ChipMgr::eID_�t���C�����C��,'C' },
		{ ChipMgr::eID_�t���C�����C��,'C' },
		{ ChipMgr::eID_���J�o���[30,'*' },
		{ ChipMgr::eID_���J�o���[30,'*' },
		{ ChipMgr::eID_���J�o���[30,'C' },
		{ ChipMgr::eID_���J�o���[30,'C' },
		{ ChipMgr::eID_�T���_�[�{�[��,'C' },
		{ ChipMgr::eID_�T���_�[�{�[��,'C' },
		{ ChipMgr::eID_���C�h�\�[�h,'S' },
		{ ChipMgr::eID_���C�h�\�[�h,'S' }
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
		message += "�Z�[�u�t�@�C���ɕs���ȉ����񂪂���܂����B";
		message += SAVE_FILE_NAME + "���폜���čŏ�����J�n���Ă�������";
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
