#pragma once

#include <list>
#include <vector>
#include <memory>
#include "point.h"

typedef struct EffectArg {
	CPoint<int> pos;
	unsigned int rndSize;
}EffectArg;

class EffectData {
public:
	EffectData() {}
	~EffectData() {}

	virtual void Draw() {}
	virtual bool Process() = 0;
};

class EffectMgr {
public:
	enum EffectID {
		eID_ロックバスター_HIT,

		eID_MAX
	};
private:
	std::vector<int> images[eID_MAX];
	std::list<std::shared_ptr<EffectData>> effectList;

	EffectMgr() {}
	~EffectMgr() {}
public:

	static EffectMgr* GetInst() {
		static EffectMgr inst;
		return &inst;
	}

	void LoadImages();
	void DeleteImages();

	void Draw();
	void Process();
	void Register(EffectID id, EffectArg args);
};