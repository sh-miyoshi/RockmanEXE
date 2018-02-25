#include "include.h"
#include "battle.h"
#include "fps.h"
#include "battleCharMgr.h"
#include "battleFieldMgr.h"
#include "drawCharacter.h"

Battle::Battle(std::list<BattleEnemyInit_t> enemyInfo, bool isBoss)
	:rtnCode(eBATTLE_RTN_CONTINUE), mainActionTimeCount(0), isBoss(isBoss)
{
	BattleFieldMgr::GetInst()->Init();
	BattleCharMgr::GetInst()->Init(enemyInfo);

	stateMgr.ChangeNext(new StateBegin(this));
}

Battle::~Battle(){
	BattleCharMgr::GetInst()->End();
	BattleFieldMgr::GetInst()->End();
}

void Battle::Draw(){
	BattleFieldMgr::GetInst()->Draw();
	stateMgr.Draw();
}

BattleRtnCode Battle::Process(){
	BattleFieldMgr::GetInst()->Process();
	stateMgr.Process();
	return rtnCode;
}

//---------------------------------------------
// �e��Ԃ��Ƃ̏���
//---------------------------------------------
Battle::StateBegin::StateBegin(Battle *obj):obj(obj), actCount(0), drawNum(1){
}

Battle::StateBegin::~StateBegin(){
}

void Battle::StateBegin::Draw(){
	// �G�`��(�J�n���̓A�j���[�V���������邽�ߕ`�悵�Ȃ�)
	BattleCharMgr::GetInst()->DrawObjects(drawNum, actCount);
}

void Battle::StateBegin::Process(){
	actCount += BEGIN_COUNTUP_VALUE;
	if( actCount >= 256 ){
		actCount = 0;
		drawNum++;
		if( drawNum > ( int ) BattleCharMgr::GetInst()->GetEnemyNum() ){
			obj->stateMgr.ChangeNext(new Battle::StateChipSelect(obj));
		}
	}
}

Battle::StateChipSelect::StateChipSelect(Battle * obj):obj(obj), pointer(0){
	int HAND_NUM = 5;// debug(�i�r�J�X�Ƃ��ŕς�����悤��)
	std::vector<ChipInfo> t = BattleCharMgr::GetInst()->GetPlayerHand(HAND_NUM);
	if( t.empty() )
		pointer = BT_SEND_NO;
	else{
		playerHand.InitData(t);
	}

	const std::string position = "Battle::StateChipSelect::StateChipSelect";
	std::string fname = def::IMAGE_FILE_PATH + "battle_chip_select.png";
	imgFrame = LoadGraphWithCheckError(fname, position);

	fname = def::IMAGE_FILE_PATH + "selector.png";
	LoadDivGraphWithCheckError(imgSelector, fname, position, 2, 1, 44, 44);

	obj->sendChip.clear();
}

Battle::StateChipSelect::~StateChipSelect(){
	DeleteGraph(imgFrame);
	for( int i = 0; i < sizeof(imgSelector) / sizeof(imgSelector[0]); i++ )
		DeleteGraph(imgSelector[i]);
}

void Battle::StateChipSelect::Draw(){
	BattleCharMgr::GetInst()->DrawObjects();
	BattleFieldMgr::GetInst()->DrawBaseFrame(true);

	// todo(�J�n���ɉ�����X���C�h���Ă���悤��)
	// �I���G���A�̕`��
	DrawGraph(0, 0, imgFrame, TRUE);

	playerHand.Draw();

	// �I���ʒu�̕`��
	if( pointer == BT_SEND_NO )
		DrawGraph(180, 225, imgSelector[1], TRUE);
	else{
		int x = ( pointer % 5 ) * 32 + 12;
		int y = ( pointer / 5 ) * 20 + 200;
		DrawGraph(x, y, imgSelector[0], TRUE);
	}

	// �`�b�v�ڍ׏��
	if( pointer != BT_SEND_NO ){
		ChipData sc = ChipMgr::GetInst()->GetChipData(playerHand.GetChip(pointer));
		DrawGraph(31, 64, sc.imgInfo, TRUE);
		DrawGraph(52, 161, ChipMgr::GetInst()->GetTypeData(sc.type).image, TRUE);
		DrawCharacter::GetInst()->DrawChipCode(30, 163, sc.code);
		DrawCharacter::GetInst()->DrawString(20, 25, sc.name, BLACK);
		if( sc.power > 0 ){
			// �U���͂��`��
			DrawCharacter::GetInst()->DrawNumber(100, 163, sc.power, DrawCharacter::COL_WHITE, 3);
		}
	}
}

void Battle::StateChipSelect::Process(){
	// �`�b�v�I��
	CKey *in = CKey::GetInst();
	if( in->CheckKey(eKEY_ENTER) == 1 ){// ����L�[�������ꂽ��
		if( pointer == BT_SEND_NO ){
			BattleCharMgr::GetInst()->SendActionChips(playerHand.GetSelectedChipIndex());
			obj->stateMgr.ChangeNext(new StateBattleStart(obj));
		} else{
			playerHand.Select(pointer);
		}
	} else if( in->CheckKey(eKEY_CANCEL) == 1 ){// �L�����Z���L�[�������ꂽ��
		playerHand.EraseLastValue();
	} else{
		if( playerHand.GetHandNum() > 0 ){
			// �J�[�\���ړ�����
			if( in->CheckKey(eKEY_LEFT) == 1 ){
				if( pointer == BT_SEND_NO )
					pointer = ( playerHand.GetHandNum() > 5 ) ? 4 : playerHand.GetHandNum() - 1;
				else if( ( pointer % 5 ) == 0 )
					pointer = BT_SEND_NO;
				else
					pointer--;
			} else if( in->CheckKey(eKEY_RIGHT) == 1 ){
				if( pointer == BT_SEND_NO )
					pointer = 0;
				else if( pointer == playerHand.GetHandNum() - 1 )
					pointer = BT_SEND_NO;
				else if( playerHand.GetHandNum() > 5 && pointer == 4 )
					pointer = BT_SEND_NO;
				else
					pointer++;
			}
		}
	}
}

char Battle::StateChipSelect::HandChip::GetCode(){
	char valid = '*';
	unsigned index = 0;
	for( ; index < codes.size(); index++ ){
		if( codes[index] != '*' ){
			valid = codes[index];
			break;
		}
	}
	for( ; index < codes.size(); index++ ){
		if( codes[index] != valid&&codes[index] != '*' )
			return '-';
	}
	return valid;// ���ׂē����R�[�h(*���܂�)
}

std::string Battle::StateChipSelect::HandChip::GetFixedName(){
	if( names.size() < 2 )
		return "";// �Œ�l�[���͂Ȃ�
	for( unsigned i = 1; i < names.size(); i++ )
		if( names[0] != names[i] )
			return "-";// ���O���قȂ�
	return names[0];// �Œ�l�[��
}

void Battle::StateChipSelect::HandChip::InitData(std::vector<ChipInfo> handValue){
	this->handValue = handValue;
	selectedIndex.clear();
	codes.clear();
	names.clear();
}

void Battle::StateChipSelect::HandChip::Draw(){
	// �`�b�v�`��
	std::string err_msg = "Battle::StateChipSelect::HandChip::Draw: ";
	err_msg += "(debug) hand��" + ToString<unsigned>(handValue.size()) + "�͕`����l���ł��Ȃ�";
	ASSERT(handValue.size() <= 5, err_msg);
	for( unsigned i = 0; i < handValue.size(); i++ ){
		if( !find<unsigned>(selectedIndex, i) ){// �`�b�v�����݂��A�I������ĂȂ����
			int x = i * 32 + 17;
			ChipData c = ChipMgr::GetInst()->GetChipData(handValue[i]);
			if( CanSelect(i) ){
				DrawGraph(x, 210, c.imgIcon, TRUE);
			} else{
				DrawGraph(x, 210, c.imgIconMono, TRUE);// �I���ł��Ȃ����̂ɂ͊D�F��
			}
			DrawCharacter::GetInst()->DrawChipCode(x + 10, 240, c.code, 50);
		}
	}

	// ���M����`�b�v�ꗗ�̕\��
	for( unsigned i = 0; i < selectedIndex.size(); i++ ){
		ChipData c = ChipMgr::GetInst()->GetChipData(handValue[selectedIndex[i]]);
		DrawGraph(194, 50 + i * 32, c.imgIcon, TRUE);
	}
}

void Battle::StateChipSelect::HandChip::EraseLastValue(){
	if( !selectedIndex.empty() ){
		selectedIndex.pop_back();
		codes.pop_back();
		names.pop_back();
	}
}

bool Battle::StateChipSelect::HandChip::CanSelect(unsigned no){
	std::string err_msg = "Battle::StateChipSelect::HandChip::CanSelect: ";
	err_msg += "�`�F�b�N�ԍ����������傫���ł� " + ToString("no: %d, max: %d", no, handValue.size());
	ASSERT(no < handValue.size(), err_msg);

	if( !find<unsigned>(selectedIndex, no) ){// �܂��I������ĂȂ����
		std::string name = ChipMgr::GetInst()->GetChipData(handValue[no].id).name;
		char code = handValue[no].code;

		// �Œ�name�����邩�H ����Ζ��O���Ⴆ�΃_��
		// �Ȃ����code��*�����������ok
		std::string fn = GetFixedName();
		if( !fn.empty() ){
			if( fn != "-"&&fn != name )
				return false;// �I���ł��Ȃ�
			else if( fn == "-"&&code != '*'&&GetCode() != code )// ���O���قȂ�̂ŃR�[�h����v���邩
				return false;// �I���ł��Ȃ�
		} else if( names.size() == 1 ){
			if( names[0] != name&&code != '*'&&GetCode() != code&&codes[0] != '*' )
				return false;// �I���ł��Ȃ�
		}
		return true;// �I���ł���
	}
	return false;// �I���ł��Ȃ�
}

bool Battle::StateChipSelect::HandChip::Select(unsigned no){
	if( CanSelect(no) ){
		std::string name = ChipMgr::GetInst()->GetChipData(handValue[no].id).name;
		char code = handValue[no].code;

		names.push_back(name);
		codes.push_back(code);
		selectedIndex.push_back(no);
		return true;// �I������
	}
	return false;// �I���Ɏ��s
}

ChipInfo Battle::StateChipSelect::HandChip::GetChip(unsigned no){
	std::string err_msg = "Battle::StateChipSelect::HandChip::CanSelect: ";
	err_msg += "Get�ԍ����������傫���ł� " + ToString("no: %d, max: %d", no, handValue.size());
	ASSERT(no < handValue.size(), err_msg);
	return handValue[no];
}

Battle::StateBattleStart::StateBattleStart(Battle * obj):obj(obj), count(0){
	const std::string position = "Battle::StateBattleStart::StateBattleStart";
	std::string fname = def::IMAGE_FILE_PATH + "battle_begin.png";
	LoadDivGraphWithCheckError(imageTitle, fname, position, 1, 3, 274, 32);
}

Battle::StateBattleStart::~StateBattleStart(){
	for( int i = 0; i < 3; i++ )
		DeleteGraph(imageTitle[i]);
}

void Battle::StateBattleStart::Draw(){
	BattleFieldMgr::GetInst()->DrawBaseFrame(false);
	BattleCharMgr::GetInst()->DrawObjects();

	int n = count / DRAW_DELAY;
	if( n > 2 )
		n = 2;
	DrawGraph(105, 125, imageTitle[n], TRUE);
}

void Battle::StateBattleStart::Process(){
	count++;
	if( count >= 3 * DRAW_DELAY + 20 )
		obj->stateMgr.ChangeNext(new StateMainAction(obj));
}


Battle::StateMainAction::StateMainAction(Battle * obj):obj(obj){
	BattleFieldMgr::GetInst()->ResetGaugeCount();
}

Battle::StateMainAction::~StateMainAction(){
}

void Battle::StateMainAction::Draw(){
	BattleCharMgr::GetInst()->DrawObjects();
	BattleFieldMgr::GetInst()->DrawBaseFrame(false);

	BattleCharMgr::GetInst()->DrawPlayerSendChipIcon();
}

void Battle::StateMainAction::Process(){
	obj->mainActionTimeCount++;

	BattleFieldMgr::GetInst()->GaugeProcess();

	switch( BattleCharMgr::GetInst()->Process() ){
	case eBATTLE_RTN_WIN:
		obj->stateMgr.ChangeNext(new StateWin(obj));
		return;
	case eBATTLE_RTN_LOSE:
		obj->rtnCode = eBATTLE_RTN_LOSE;
		return;
	}

	// �J�X�^���Q�[�W�����^����L,R�L�[�������ꂽ��
	if( BattleFieldMgr::GetInst()->IsGaugeCharged() ){
		if( CKey::GetInst()->CheckKey(eKEY_DEV_L) == 1 || CKey::GetInst()->CheckKey(eKEY_DEV_R) == 1 ){
			obj->stateMgr.ChangeNext(new StateChipSelect(obj));
		}
	}
}

Battle::StateWin::StateWin(Battle * obj)
	:obj(obj), count(0), bustingLv(0),
	mosaic(CPoint<int>(268, 189), CPoint<int>(112, 96), CPoint<int>(16, 16))
{
	const std::string position = "Battle::StateWin::StateWin";
	std::string fname = def::IMAGE_FILE_PATH + "zenny.png";
	imgZenny = LoadGraphWithCheckError(fname, position);
	fname = def::IMAGE_FILE_PATH + "battle_result_frame.png";
	imgResultFrame = LoadGraphWithCheckError(fname, position);

	// �o�X�e�B���O���x���̌���
	// �E�B���X��
	//   �` 5�b:	7point
	//   �`12�b:	6point
	//   �`36�b:	5point
	//   ����ȍ~:	4point
	// �i�r��
	//   �`30�b:	10point
	//   �`40�b:	 8point
	//   �`50�b:	 6point
	//   ����ȍ~:	 4point
	// �U�����󂯂���(�̂���������)
	//   0��:		+1point
	//   1��:		+0point
	//   2��:		-1point
	//   3��:		-2point
	//   4��ȏ�:	-3point
	// �ړ������}�X
	//   �`2�}�X:	1point
	//   3�}�X�ȏ�:	0point
	// �����ɓ|��
	//   2�̓���:	2point
	//   3�̓���:	4point

	// �|�������Ԃɂ��|�C���g���Z
	int time = obj->mainActionTimeCount / Fps::FPS;
	bustingLv = 4;
	if( obj->isBoss ){
		const int deadline[4] = { 50,40,30,-1 };
		for( int i = 0; i < 4; i++ ){
			if( time > deadline[i] ){
				bustingLv += ( i * 2 );
				break;
			}
		}
	} else{
		const int deadline[4] = { 36,12,5,-1 };
		for( int i = 0; i < 4; i++ ){
			if( time > deadline[i] ){
				bustingLv += i;
				break;
			}
		}
	}

	// debug(�����͎��Ԃ̂�)

	std::vector<ResultView_t> getItems;

	// �`�b�v�̌���
	std::list<EnemyMgr::EnemyAttackChip_t> enemyChips = BattleCharMgr::GetInst()->GetEnemyChips();
	for( std::list<EnemyMgr::EnemyAttackChip_t>::iterator it = enemyChips.begin(); it != enemyChips.end(); it++ ){
		if( bustingLv >= ( unsigned ) it->bustingLevel ){// ���g�̃o�X�e�B���O���x�����K�v���x���𒴂��Ă�����
			ResultView_t t;
			t.type = ResultView_t::eTYPE_CHIP;
			ChipData t2 = ChipMgr::GetInst()->GetChipData(it->chip.id, it->chip.code);
			t.name = t2.name;
			t.value = ( int ) t2.code;
			t.image = t2.imgInfo;
			getItems.push_back(t);
		}
	}

	// �[�j�[�̌���
	// 30,50,100,200,400,500,1000,2000
	ResultView_t t;
	t.type = ResultView_t::eTYPE_ZENNY;
	t.name = "�[�j�[";
	t.image = imgZenny;
	switch( bustingLv ){
	case  4: t.value = 30; break;
	case  5: t.value = 50; break;
	case  6: t.value = 100; break;
	case  7: t.value = 200; break;
	case  8: t.value = 400; break;
	case  9: t.value = 500; break;
	case 10: t.value = 1000; break;
	default:
		t.value = 2000;
		break;
	}
	getItems.push_back(t);

	// todo(�d�݂���������)
	// �����_���őI��
	getItem = getItems[rand() % getItems.size()];
	for( auto d : getItems ){
		std::string msg = "Battle::StateWin::StateWin ";
		msg += d.name + " " + ToString<int>(d.value);
		AppLogger::Debug(msg);
	}
}

Battle::StateWin::~StateWin(){
	DeleteGraph(imgZenny);
	DeleteGraph(imgResultFrame);
}

void Battle::StateWin::Draw(){
	BattleFieldMgr::GetInst()->DrawBaseFrame(false);

	// �t���[���͉�����o�Ă���
	int x = count * 5;
	if( x > 40 )
		x = 40;
	DrawGraph(x, 45, imgResultFrame, TRUE);

	if( count >= VIEW_ITEM_COUNT ){
		// �f���[�g���Ԃ̕`��
		int time = obj->mainActionTimeCount / Fps::FPS;
		int min = time / 60;
		int sec = time % 60;
		if( min > 99 )
			min = 99;
		DrawCharacter::GetInst()->DrawNumberPadding(305, 92, min, 2);
		DrawCharacter::GetInst()->DrawString(340, 95, "�F", WHITE);
		DrawCharacter::GetInst()->DrawNumberPadding(355, 92, sec, 2);

		// �o�X�e�B���O���x���̕`��
		DrawCharacter::GetInst()->DrawNumber(365, 140, bustingLv);

		DrawGraph(268, 189, getItem.image, TRUE);
		mosaic.Draw();
		// ���O�\��
		if( count >= VIEW_ITEM_COUNT + 40 ){
			DrawCharacter::GetInst()->DrawString(100, 245, getItem.name, WHITE);
			switch( getItem.type ){
			case ResultView_t::eTYPE_CHIP:
				DrawCharacter::GetInst()->DrawString(230, 245, ToString("%c", getItem.value), WHITE);
				break;
			case ResultView_t::eTYPE_ZENNY:
				DrawCharacter::GetInst()->DrawString(100 + 90, 245, ToString("%4dZ", getItem.value), WHITE);
				break;
			}
		}
	}
}

void Battle::StateWin::Process(){
	if( count >= VIEW_ITEM_COUNT ){
		if( count % 5 == 0 )
			mosaic.Update(10);
		if( count >= VIEW_ITEM_COUNT + 50 ){
			if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 )
				obj->rtnCode = eBATTLE_RTN_WIN;
		}
	}
	count++;
}
