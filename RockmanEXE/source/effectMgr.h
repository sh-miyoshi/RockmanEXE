#pragma once

#include <list>
#include <vector>
#include <memory>
#include "point.h"

typedef struct EffectArg {
	CPoint<int> pos;
	unsigned int rndSize;
	unsigned int num;

	EffectArg():rndSize(0), num(1) {}
	~EffectArg() {}
}EffectArg;

class EffectMgr {
public:
	enum EffectID {
		eID_ロックバスター_HIT,
		eID_EnemyDeleted,

		eID_MAX
	};
private:
	class EffectData {
		unsigned int count;
		std::vector<CPoint<int>> drawPositions;
		std::vector<int> images;
	public:
		EffectData(EffectArg args, std::vector<int> images);
		~EffectData();

		void Draw();
		bool Process();
	};

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