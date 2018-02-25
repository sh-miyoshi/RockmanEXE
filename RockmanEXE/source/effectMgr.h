#pragma once

#include <vector>
#include "point.h"

class EffectMgr{
public:
	enum EffectID{
		eID_EXPLODE,

		eID_MAX
	};
private:
	typedef struct{
		int id;
		int count;
		int imgDelay;
		CPoint<int> pos;
		CPoint<int> ofs;
	}EffectData_t;

	std::vector<EffectData_t> actives;
	std::vector<int> images[eID_MAX];
	int EFFECT_IMAGE_DELAY[eID_MAX];// ŽŸ‚Ì‰æ‘œ‚ÉˆÚ‚é‚Ü‚Å‚ÌƒJƒEƒ“ƒg
	CPoint<int> EFFECT_IMAGE_OFFSET[eID_MAX];

	EffectMgr(){}
	~EffectMgr(){}
public:
	static EffectMgr *GetInst(){
		static EffectMgr inst;
		return &inst;
	}

	void Init();
	void End();

	void Process();
	void Draw();

	bool Register(int id, CPoint<int> pos);
};