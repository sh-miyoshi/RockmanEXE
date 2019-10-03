#pragma once

#include <string>

class ChipInfo {
public:
	int id;
	char code;

	ChipInfo():id(0), code('*') {}
	ChipInfo(int id, char code):id(id), code(code) {}

	bool operator<(const ChipInfo& right) const {
		return id < right.id;
	}
};

// ������skill�ɔC����
class ChipData {
public:
	unsigned int id;
	int imgIcon;
	int imgIconMono;
	int imgInfo;
	std::string name;
	char code;
	unsigned int type;// ����
	unsigned int power;
	unsigned int playerAct;// �v���C���[�̍s���摜�͂ǂꂩ
	bool isPrevMotion;// �`�b�v�g�p���ɈÓ]�Ȃǂ̑O���������邩

	ChipData();
	ChipData(unsigned int id, std::string name, unsigned int type, unsigned int power, unsigned int playerAct, bool isPrevMotion);
	~ChipData() {}
};

typedef struct TypeData {
	std::string name;
	int image;
	// debug(��_�Ƃ��H)
}TypeData;

class ChipMgr {
public:
	enum Type {// ���ԌŒ�
		eTYPE_��,
		eTYPE_��,
		eTYPE_�u���C�N,
		eTYPE_�\�[�h,
		eTYPE_�n�`,
		eTYPE_�u��,
		eTYPE_��,
		eTYPE_�J�[�\��,
		eTYPE_���l�t��,
		eTYPE_��,
		eTYPE_��,
		eTYPE_�d�C,
		eTYPE_��,

		eTYPE_MAX
	};

	enum ID {
		eID_�L���m��,
		eID_�n�C�L���m��,
		eID_�X�g�[���L���[�u,
		eID_�T���_�[�{�[��,
		eID_�\�[�h,
		eID_�t���C�����C��,
		eID_���J�o���[10,
		eID_�u�[������,
		eID_�~�j�{��,
		eID_���J�o���[30,
		eID_���C�h�\�[�h,

		eID_MAX
	};
private:
	bool isLoad;
	int imgType[14];
	ChipData chipData[eID_MAX];
	TypeData typeData[eTYPE_MAX];

	ChipMgr():isLoad(false), imgType() {}
	~ChipMgr() {}
public:
	static ChipMgr* GetInst() {
		static ChipMgr inst;
		return &inst;
	}

	void LoadData();
	void DeleteData();

	TypeData GetTypeData(int typeNo);
	ChipData GetChipData(int id, char code = '*');
	ChipData GetChipData(ChipInfo c) { return GetChipData(c.id, c.code); }
};
