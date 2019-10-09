#include <algorithm>
#include "include.h"
#include "battle.h"
#include "battleCharMgr.h"
#include "playerMgr.h"
#include "drawCharacter.h"
#include "battleFieldMgr.h"

bool Battle::StateChipSelect::PlayerHandMgr::CanSelect(unsigned int no) {
	if( selectedIndexes.size() < 1 ) {// �܂������I������ĂȂ��Ȃ�
		return true;
	}

	auto res = std::find(selectedIndexes.begin(), selectedIndexes.end(),no);
	if( res != selectedIndexes.end() ) {// ���łɑI������Ă���Ȃ�
		return false;
	}

	// ���ׂĂ̖��O����v�Ȃ�I���ł���
	std::string name = ChipMgr::GetInst()->GetChipData(handValue[no]).name;
	bool isSameName = true;
	for( auto i : selectedIndexes ) {
		ChipData c = ChipMgr::GetInst()->GetChipData(handValue[i]);
		if( name != c.name ) {
			isSameName = false;
			break;
		}
	}
	if( isSameName ) {
		return true;
	}

	// �R�[�h����v���Ă���ΑI���ł���
	char code = handValue[no].code;
	bool isSameCode = true;
	for( auto i : selectedIndexes ) {
		char c = handValue[i].code;
		if( code == '*' ) {
			code = c;
		} else if( code != c && c!='*' ) {
			isSameCode = false;
			break;
		}
	}
	if( isSameCode ) {
		return true;
	}

	return false;
}

void Battle::StateChipSelect::PlayerHandMgr::Init(std::vector<ChipInfo> handValue) {
	this->handValue = handValue;
	selectedIndexes.clear();
}

bool Battle::StateChipSelect::PlayerHandMgr::Select(unsigned int no) {
	if( CanSelect(no) ) {// ���̃`�b�v���I���\�Ȃ�
		selectedIndexes.push_back(no);
		return true;
	}
	return false;
}

void Battle::StateChipSelect::PlayerHandMgr::Draw() {
	BattleFieldMgr::GetInst()->DrawBaseFrame(BattleFieldMgr::eSTATE_CHIP_SELECT);

	if( handValue.size() > 5 ) {
		AppLogger::Error("[������]Hand�� %d�͕`����l���ł��ĂȂ�", handValue.size());// TODO
		exit(1);
	}

	for( size_t i = 0; i < handValue.size(); i++ ) {
		auto res = std::find(selectedIndexes.begin(), selectedIndexes.end(), i);
		if( res == selectedIndexes.end() ) {// �`�b�v�����݂��A�I������ĂȂ����
			int x = i * 32 + 17;
			ChipData c = ChipMgr::GetInst()->GetChipData(handValue[i]);
			if( CanSelect(i) ) {
				DrawGraph(x, 210, c.imgIcon, TRUE);
			} else {
				DrawGraph(x, 210, c.imgIconMono, TRUE);// �I���ł��Ȃ����̂ɂ͊D�F��
			}
			// �`�b�v�R�[�h�̕`��
			DrawCharacter::GetInst()->DrawChipCode(x + 10, 240, c.code, 50);
		}
	}

	// ���M����`�b�v�ꗗ�̕\��
	for( size_t i = 0; i < selectedIndexes.size(); i++ ) {
		ChipData c = ChipMgr::GetInst()->GetChipData(handValue[selectedIndexes[i]]);
		DrawGraph(194, 50 + i * 32, c.imgIcon, TRUE);
	}
}

ChipData Battle::StateChipSelect::PlayerHandMgr::GetChipData(unsigned int no){
	if(no>=handValue.size()){
		AppLogger::Error("�I�����悤�Ƃ��Ă���ꏊ���ُ�ł�. size: %d, no: %d", handValue.size(), no);
	}
	return ChipMgr::GetInst()->GetChipData(handValue[no]);
}

void Battle::StateChipSelect::PlayerHandMgr::EraseLastSelect() {
	if( !selectedIndexes.empty() ) {
		selectedIndexes.pop_back();
	}
}

void Battle::StateChipSelect::PlayerHandMgr::SetSendChipList() {
	PlayerMgr::GetInst()->GetBattleChar()->SetSendChipList(selectedIndexes);
}

Battle::StateChipSelect::StateChipSelect(Battle* obj)
	:obj(obj), imgSelectFrame(-1), pointer(0), imgPointer(), drawCount(0) {

	// �摜�̓ǂݍ���
	const std::string position = "Battle::StateChipSelect::StateChipSelect";
	std::string fname = def::IMAGE_FILE_PATH + "battle/�t���[��/battle_chip_select.png";
	imgSelectFrame = LoadGraphWithErrorCheck(fname, position);

	fname = def::IMAGE_FILE_PATH + "battle/�t���[��/pointer.png";
	LoadDivGraphWithErrorCheck(imgPointer, fname, position, 2, 1, 44, 44);

	// pointer�̏����ʒu�Z�b�g
	int HAND_NUM = 5;// TODO(�i�r�J�X�ȂǂŕύX�ł���悤�ɂ���)
	auto hands = PlayerMgr::GetInst()->GetBattleChar()->GetHandData(HAND_NUM);
	if( hands.size() == 0 ) {
		pointer = BT_SEND_NO;
	} else {
		// �n���h�f�[�^���Z�b�g����
		playerHand.Init(hands);
	}
}

Battle::StateChipSelect::~StateChipSelect() {
	DeleteGraph(imgSelectFrame);
	for( int i = 0; i < 2; i++ ) {
		DeleteGraph(imgPointer[i]);
	}
}

void Battle::StateChipSelect::Draw() {
	BattleCharMgr::GetInst()->Draw();

	// todo(�J�n���ɉ�����X���C�h���Ă���悤��)
	// �I���G���A�̕`��
	DrawGraph(0, 0, imgSelectFrame, TRUE);

	playerHand.Draw();

	// �I���ʒu�̕`��
	static const int DRAW_DELAY = 40;
	if( ( drawCount / DRAW_DELAY ) % 2 == 0 ) {
		if( pointer == BT_SEND_NO )
			DrawGraph(180, 225, imgPointer[1], TRUE);
		else {
			int x = ( pointer % 5 ) * 32 + 12;
			int y = ( pointer / 5 ) * 20 + 200;
			DrawGraph(x, y, imgPointer[0], TRUE);
		}
	}

	// �`�b�v�ڍ׏��̕\��
	if( 0<=pointer && pointer < playerHand.GetHandNum() ){
		ChipData c = playerHand.GetChipData(pointer);
		DrawGraph(31, 64, c.imgInfo, TRUE);
		DrawGraph(52, 161, ChipMgr::GetInst()->GetTypeData(c.type).image, TRUE);
		DrawCharacter::GetInst()->DrawChipCode(30, 163, c.code);
		DrawCharacter::GetInst()->DrawString(20, 20, c.name, BLACK);
		if( c.power > 0 ){
			// �U���͂��`��
			DrawCharacter::GetInst()->DrawNumber(100, 163, c.power, DrawCharacter::COL_WHITE, 3);
		}
	}
}

void Battle::StateChipSelect::Process() {
	if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 ) {
		if( pointer == BT_SEND_NO ) {// �`�b�v�I������
			playerHand.SetSendChipList();// �I�񂾃`�b�v��BattleStatemain�ֈ����n��
			obj->stateMgr.ChangeNext(new Battle::StateMain(obj));
		} else {
			playerHand.Select(pointer);
		}
	} else if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ) {
		// �I�����Ă���`�b�v�̍Ō�̒l���폜����
		playerHand.EraseLastSelect();
	}else if( playerHand.GetHandNum() > 0 ) {
		// �J�[�\���ړ�����
		if( CKey::GetInst()->CheckKey(eKEY_LEFT) == 1 ) {
			if( pointer == BT_SEND_NO )
				pointer = ( playerHand.GetHandNum() > 5 ) ? 4 : playerHand.GetHandNum() - 1;
			else if( ( pointer % 5 ) == 0 )
				pointer = BT_SEND_NO;
			else
				pointer--;
		} else if( CKey::GetInst()->CheckKey(eKEY_RIGHT) == 1 ) {
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

	drawCount++;
}
