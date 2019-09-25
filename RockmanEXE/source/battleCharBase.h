#pragma once

#include <list>
#include <vector>
#include <memory>
#include <queue>
#include "define.h"
#include "point.h"

enum CharType {
	eCHAR_PLAYER = 0x1,
	eCHAR_ENEMY = 0x2,
	eCHAR_OBJECT = 0x4,

	eCHAR_ALL = eCHAR_PLAYER | eCHAR_ENEMY | eCHAR_OBJECT,
};

class Animation {
	static const unsigned int ANIM_NUM_MAX = 50;

	unsigned int count;
	int image[ANIM_NUM_MAX];
	unsigned int imageNum;
	unsigned int imageDelay;
	std::string imageFileName;
public:
	Animation();
	Animation(const Animation& data);
	~Animation();

	void LoadData(std::string fname, CPoint<unsigned int> size, CPoint<unsigned int> num, unsigned int imageDelay = 1);
	void DeleteData();
	
	virtual void Begin() {}
	virtual void End() {}
	void Draw(int x, int y);
	virtual void Process() {}
};

class BattleCharBase {
protected:
	std::string name;
	unsigned int hp, hpMax;
	CPoint<int> pos;
	CharType myCharType;
	std::vector<std::shared_ptr<Animation>> animData;
	std::shared_ptr<Animation> defaultAnim;
	std::queue<std::shared_ptr<Animation>> animQueue;

	void AnimProcess();
public:
	BattleCharBase(std::string name, unsigned int hp, unsigned int hpMax, CharType myCharType, std::shared_ptr<Animation> defaultAnim);
	~BattleCharBase() {}

	unsigned int AddAnimDefine(std::shared_ptr<Animation> anim);

	// virtual bool DamageProc(std::list<DamageData>& damageList);// TODO(未実装)
	void Draw();
	virtual void Process();// キャラの行動

	void SetPos(int x, int y) { pos.x = x; pos.y = y; }
	void AttachAnim(unsigned int animNo, bool forceRun = false);
};

/* TODO

anim用queueを用意
キーが押されたとき
  通常行動ならキューに追加
  割り込み行動ならキューを空にして現在実行しているのも中断

キューから一つ取り出す
実行状態にする
実行終了ならキューからまた取り出す

キューが空ならdefaultを実行(ANIM_STAND)
*/