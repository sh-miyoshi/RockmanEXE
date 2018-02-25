#pragma once

#include <memory>

class AnimProcess{
public:
	virtual void Begin(){}
	virtual void End(){}
	virtual void Draw(){}
	virtual void Process(){}

	static std::shared_ptr<AnimProcess> Create(){
		return std::shared_ptr<AnimProcess>(new AnimProcess());
	}
};
