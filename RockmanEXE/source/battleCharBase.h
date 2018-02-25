#pragma once

#include <queue>
#include <list>
#include "skill.h"
#include "battleObject.h"
#include "define.h"

class BattleCharBase:public BattleObject{
	def::charType GetDamagedType(def::charType myCharType);
	int GetMyPanel(def::charType myCharType);
protected:
	static const int DEFAULT_MUTEKI_TIME = 2 * 60;// ���G�̎���

	std::string name;
	int mutekiCnt;
	int mutekiEndCnt;// ���G���I��������J�E���g
	std::queue<int> reserveAnim;// ���ɍs���ׂ��A�j���[�V����
	int deleteCount;// �L������HP��0�ɂȂ��Ă���̃��[�V�����p�J�E���g

	bool Move(int direct, bool isMove);
	bool MoveCheck(int x, int y, bool checkPanel = true);
public:
	BattleCharBase(std::string name, int hp,def::charType myCharType);
	~BattleCharBase(){}

	virtual bool DamageProc(std::list<DamageData> &damageList);
	void Draw();
	virtual void Process();
};
