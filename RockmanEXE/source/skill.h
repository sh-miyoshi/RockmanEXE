#pragma once

#include <list>
#include <memory>
#include "point.h"
#include "chip.h"
#include "include.h"

class SkillArg{
public:
	CPoint<int> atkPos;
//	CPoint<int> charPos;
	int damage;
	int recover;
};

class DamageData{
public:
	def::charType myCharType;
	int damage;
	CPoint<int> pos;
	bool makeMuteki;
	int delCount;
	int skillID;
	bool forceDelFlag;
	bool isPenetrate;// 無敵状態を貫通するか

	DamageData(CPoint<int> pos, int damage, def::charType myCharType, bool makeMuteki, int delCount, int skillID, bool isPenetrate)
		:damage(damage), pos(pos), myCharType(myCharType), makeMuteki(makeMuteki)
		, delCount(delCount), skillID(skillID), forceDelFlag(false), isPenetrate(isPenetrate){}
};

class SkillData{
public:
	int id;
	bool delFlag;
	bool dependChar;// スキルがキャラクタの状態に依存するか(のけぞり時ストップするか)
	bool isPenetrate;// 攻撃ヒット時も貫通して処理を続行するか
	int damagePower;// ダメージを与える量
	def::charType myCharType;
	std::list<DamageData> damageList;

	SkillData(bool dependChar, bool isPenetrate, def::charType myCharType, int damage)
		:delFlag(false), dependChar(dependChar), myCharType(myCharType)
		, isPenetrate(isPenetrate), id(IDManager::CreateUniqueID()), damagePower(damage){}
	~SkillData(){}

	virtual void Process() = 0;
	virtual void Draw(){}
};

class SkillMgr{
public:
	enum SkillID{
		eID_バスター,
		eID_ショックウェーブ,
		eID_キャノン,
		eID_ハイキャノン,
		eID_ブーメラン_周回,
		eID_ブーメラン_直線,
		eID_サンダーボール,
		eID_ストーンキューブ,
		eID_ヘルズローリング_UP,
		eID_ヘルズローリング_DOWN,
		eID_フレイムライン_固定,
		eID_ソード,
		eID_リカバリー,
		eID_ダークアームブレード_SHORT,
		eID_ダークアームブレード_LONG,
		eID_ミニボム,
		eID_ワイドソード,
		eID_シューティングバスター,

		eID_MAX
	};

	static std::shared_ptr<SkillData> GetData(int id, SkillArg args, def::charType myCharType);
	static std::shared_ptr<SkillData> GetData(ChipData c, SkillArg args, def::charType myCharType);
};
