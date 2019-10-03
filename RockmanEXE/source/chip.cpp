#include <vector>
#include <algorithm>
#include <fstream>
#include "include.h"
#include "chip.h"

ChipData::ChipData()
	:id(0), type(0), power(0), playerAct(0), isPrevMotion(false),
	imgIcon(-1), imgIconMono(-1), imgInfo(-1), code('*') {
}

ChipData::ChipData(unsigned int id, std::string name, unsigned int type, unsigned int power, unsigned int playerAct, bool isPrevMotion)
	: id(id), name(name), type(type), power(power), playerAct(playerAct), isPrevMotion(isPrevMotion),
	imgIcon(-1), imgIconMono(-1), imgInfo(-1), code('*') {
}

//�@�摜�ƃ`�b�v���̓ǂݍ���
void ChipMgr::LoadData() {
	const std::string position = "ChipMgr::LoadData";

	if( isLoad ) {// ��x�ǂݍ��݂̋֎~
		AppLogger::Error("Chip Data is already loaded");
		exit(1);
	}

	// �`�b�v���̐ݒ�
	chipData[eID_�L���m��] = ChipData(0, "�L���m��", 0, 40, 2, false);
	chipData[eID_�n�C�L���m��] = ChipData(1, "�n�C�L���m��", 0, 80, 2, false);
	chipData[eID_�T���_�[�{�[��] = ChipData(14, "�T���_�[�{�[��", 0, 40, -1, false);
	chipData[eID_�t���C�����C��] = ChipData(20, "�t���C�����C��", 0, 80, 2, false);
	chipData[eID_�~�j�{��] = ChipData(43, "�~�j�{��", 0, 50, 4, false);
	chipData[eID_�\�[�h] = ChipData(53, "�\�[�h", 0, 80, 3, false);
	chipData[eID_���C�h�\�[�h] = ChipData(54, "���C�h�\�[�h", 0, 80, 3, false);
	chipData[eID_�u�[������] = ChipData(68, "�u�[������", 0, 60, -1, false);
	chipData[eID_���J�o���[10] = ChipData(108, "���J�o���[10", 0, 10, -1, false);
	chipData[eID_���J�o���[30] = ChipData(109, "���J�o���[30", 0, 30, -1, false);
	chipData[eID_�X�g�[���L���[�u] = ChipData(125, "�X�g�[���L���[�u", 0, 0, -1, true);

	// �`�b�v�A�C�R���摜�̓ǂݍ��݂Ɛݒ�
	std::string fname = def::IMAGE_FILE_PATH + "chip_icon.png";
	int t1[240], t2[240];
	LoadDivGraphWithErrorCheck(t1, fname, position, 30, 8, 28, 28);
	LoadDivGraph(fname.c_str(), 240, 30, 8, 28, 28, t2);// �����t�@�C������Ȃ̂œǂݍ��߂�̂͊m��

	for( auto i = 0; i < eID_MAX; i++ ) {
		unsigned int id = chipData[i].id;
		chipData[i].imgIcon = t1[id];
		chipData[i].imgIconMono = t2[id];
		GraphFilter(chipData[i].imgIconMono, DX_GRAPH_FILTER_MONO, 0, 0);// ���m�N����
	}

	std::vector<int> used;// �g�p���Ă���A�C�R���̔ԍ�
	for( auto i = 0; i < eID_MAX; i++ ) {
		used.push_back(chipData[i].id);
	}

	// �g��Ȃ����̂͊J�����Ă���
	for( int i = 0; i < 240; i++ ) {
		if( std::find(used.begin(), used.end(), i) == used.end() ) {
			DeleteGraph(t1[i]);
			DeleteGraph(t2[i]);
		}
	}

	// �`�b�v�ڍ׉摜
	std::string fdir = def::IMAGE_FILE_PATH + "infoImage/";
	fname = fdir + "s1.png";
	chipData[eID_�L���m��].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s2.png";
	chipData[eID_�n�C�L���m��].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s95.png";
	chipData[eID_�X�g�[���L���[�u].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s15.png";
	chipData[eID_�T���_�[�{�[��].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s54.png";
	chipData[eID_�\�[�h].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s21.png";
	chipData[eID_�t���C�����C��].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s109.png";
	chipData[eID_���J�o���[10].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s69.png";
	chipData[eID_�u�[������].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s44.png";
	chipData[eID_�~�j�{��].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s110.png";
	chipData[eID_���J�o���[30].imgInfo = LoadGraphWithErrorCheck(fname, position);
	fname = fdir + "s55.png";
	chipData[eID_���C�h�\�[�h].imgInfo = LoadGraphWithErrorCheck(fname, position);

	// �`�b�v�^�C�v�摜
	fname = def::IMAGE_FILE_PATH + "chip_type.png";
	LoadDivGraphWithErrorCheck(imgType, fname, position, 7, 2, 28, 28);

	// �������
	static const std::string TYPE_NAME[eTYPE_MAX] = {
		"��","��","�u���C�N","�\�[�h","�n�`","�u��","��","�J�[�\��","���l�t��","��","��","�d�C","��"
	};
	for( int i = 0; i < eTYPE_MAX; i++ ) {
		typeData[i].name = TYPE_NAME[i];
		typeData[i].image = imgType[i];
	}

	isLoad = true;
}

void ChipMgr::DeleteData() {
	if( isLoad ) {
		isLoad = false;

		for( int i = 0; i < eID_MAX; i++ ) {
			DeleteGraph(chipData[i].imgIcon);
			DeleteGraph(chipData[i].imgIconMono);
			DeleteGraph(chipData[i].imgInfo);
		}

		for( int i = 0; i < eTYPE_MAX; i++ ) {
			DeleteGraph(imgType[i]);
		}
	}
}

TypeData ChipMgr::GetTypeData(int typeNo) {
	if( !isLoad ) {
		AppLogger::Error("ChipData are not loaded yet");
		exit(1);
	}
	if( !( 0 <= typeNo && typeNo < eTYPE_MAX ) ) {
		AppLogger::Error("Invalid Type Number(%d). typeNo must be between 0 and %d", typeNo, eTYPE_MAX);
		exit(1);
	}
	return typeData[typeNo];
}

ChipData ChipMgr::GetChipData(int id, char code) {
	if( !isLoad ) {
		AppLogger::Error("ChipData are not loaded yet");
		exit(1);
	}
	if( !( 0 <= id && id < eID_MAX ) ) {
		AppLogger::Error("Invalid Chip ID(%d). ID must be between 0 and %d", id, eID_MAX);
		exit(1);
	}
	ChipData ret = chipData[id];
	ret.code = code;
	return ret;
}

