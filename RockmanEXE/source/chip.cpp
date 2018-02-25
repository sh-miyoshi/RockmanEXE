#include <vector>
#include <algorithm>
#include "include.h"
#include "chip.h"
#include "csv.h"

//�@�摜�ƃ`�b�v���̓ǂݍ���
void ChipMgr::LoadData(){
	const std::string position = "ChipMgr::LoadData";

	ASSERT(!isLoad, position + " already loaded data");// ��x�ǂݍ��݂̋֎~
	isLoad = true;

	// �`�b�v�A�C�R���摜
	std::string fname = def::IMAGE_FILE_PATH + "chip_icon.png";
	int t1[240], t2[240];
	LoadDivGraphWithCheckError(t1, fname, position, 30, 8, 28, 28);
	LoadDivGraph(fname.c_str(), 240, 30, 8, 28, 28, t2);// �����t�@�C������Ȃ̂œǂݍ��߂�̂͊m��

	// �A�C�R���͊�{�I�Ɏ蓮�Őݒ�
	imgIcon[eID_�L���m��] = t1[0];
	imgIconMono[eID_�L���m��] = t2[0];
	imgIcon[eID_�n�C�L���m��] = t1[1];
	imgIconMono[eID_�n�C�L���m��] = t2[1];
	imgIcon[eID_�X�g�[���L���[�u] = t1[125];
	imgIconMono[eID_�X�g�[���L���[�u] = t2[125];
	imgIcon[eID_�T���_�[�{�[��] = t1[14];
	imgIconMono[eID_�T���_�[�{�[��] = t2[14];
	imgIcon[eID_�\�[�h] = t1[53];
	imgIconMono[eID_�\�[�h] = t2[53];
	imgIcon[eID_�t���C�����C��] = t1[20];
	imgIconMono[eID_�t���C�����C��] = t2[20];
	imgIcon[eID_���J�o���[10] = t1[108];
	imgIconMono[eID_���J�o���[10] = t2[108];
	imgIcon[eID_�u�[������] = t1[68];
	imgIconMono[eID_�u�[������] = t2[68];
	imgIcon[eID_�~�j�{��] = t1[43];
	imgIconMono[eID_�~�j�{��] = t2[43];
	imgIcon[eID_���J�o���[30] = t1[109];
	imgIconMono[eID_���J�o���[30] = t2[109];
	imgIcon[eID_���C�h�\�[�h] = t1[54];
	imgIconMono[eID_���C�h�\�[�h] = t2[54];

	std::vector<int> used;// �g�p���Ă���A�C�R���̔ԍ�
	used.push_back(0);
	used.push_back(1);
	used.push_back(125);
	used.push_back(14);
	used.push_back(53);
	used.push_back(20);
	used.push_back(108);
	used.push_back(68);
	used.push_back(43);
	used.push_back(109);
	used.push_back(54);

	// ���m�N����
	for( int i = 0; i < eID_MAX; i++ )
		GraphFilter(imgIconMono[i], DX_GRAPH_FILTER_MONO, 0, 0);

	// �g��Ȃ����̂͊J�����Ă���
	for( int i = 0; i < 240; i++ ){
		if( std::find(used.begin(), used.end(), i) == used.end() ){
			DeleteGraph(t1[i]);
			DeleteGraph(t2[i]);
		}
	}

	// �`�b�v�ڍ׉摜
	std::string fdir = def::IMAGE_FILE_PATH + "infoImage/";
	fname = fdir + "s1.png";
	imgInfo[eID_�L���m��] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s2.png";
	imgInfo[eID_�n�C�L���m��] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s95.png";
	imgInfo[eID_�X�g�[���L���[�u] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s15.png";
	imgInfo[eID_�T���_�[�{�[��] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s54.png";
	imgInfo[eID_�\�[�h] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s21.png";
	imgInfo[eID_�t���C�����C��] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s109.png";
	imgInfo[eID_���J�o���[10] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s69.png";
	imgInfo[eID_�u�[������] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s44.png";
	imgInfo[eID_�~�j�{��] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s110.png";
	imgInfo[eID_���J�o���[30] = LoadGraphWithCheckError(fname, position);
	fname = fdir + "s55.png";
	imgInfo[eID_���C�h�\�[�h] = LoadGraphWithCheckError(fname, position);

	// �`�b�v�^�C�v�摜
	fname = def::IMAGE_FILE_PATH + "chip_type.png";
	LoadDivGraphWithCheckError(imgType, fname, position, 7, 2, 28, 28);

	// �`�b�v���
	fname = def::TEXT_FILE_PATH + "chip_data.csv";
	std::string buf[CSV::COLUMN_MAX];
	CSV input(fname, CSV::MODE_READ);
	for( int i = 0; i < eID_MAX&&input.getData(buf); i++ ){
		chipData[i].id = i;
		chipData[i].imgInfo = imgInfo[i];
		chipData[i].imgIcon = imgIcon[i];
		chipData[i].imgIconMono = imgIconMono[i];
		chipData[i].name = buf[0];
		chipData[i].code = '*';// �K��
		chipData[i].type = atoi(buf[1].c_str());
		chipData[i].power = atoi(buf[2].c_str());
		chipData[i].playerAct = atoi(buf[3].c_str());
		chipData[i].isPrevMotion = atoi(buf[4].c_str()) != 0;
	}

	// �������
	static const std::string TYPE_NAME[eTYPE_MAX] = {
		"��","��","�u���C�N","�\�[�h","�n�`","�u��","��","�J�[�\��","���l�t��","��","��","�d�C","��"
	};
	for( int i = 0; i < eTYPE_MAX; i++ ){
		typeData[i].name = TYPE_NAME[i];
		typeData[i].image = imgType[i];
	}
}

void ChipMgr::DeleteData(){
	isLoad = false;

	for( int i = 0; i < eID_MAX; i++ ){
		DeleteGraph(imgIcon[i]);
		DeleteGraph(imgIconMono[i]);
		DeleteGraph(imgInfo[i]);
	}

	for( int i = 0; i < 14; i++ )
		DeleteGraph(imgType[i]);
}

TypeData ChipMgr::GetTypeData(int type){
	ASSERT(isLoad, "ChipMgr::GetTypeData data are not loaded");
	ASSERT(0 <= type&&type < eTYPE_MAX, ToString("ChipMgr::GetTypeData invalied type (%d)", type));
	return typeData[type];
}

ChipData ChipMgr::GetChipData(int id, char code){
	ASSERT(isLoad, "ChipMgr::GetChipData data are not loaded");
	ASSERT(0 <= id&&id < eID_MAX, ToString("ChipMgr::GetTypeData invalied chip id (%d)", id));
	ChipData ret = chipData[id];
	ret.code = code;
	return ret;
}

ChipData ChipMgr::GetChipData(ChipInfo info){
	return GetChipData(info.id, info.code);
}
