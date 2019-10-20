#pragma once

class Fade{
public:
	enum State{
		eSTATE_NULL,
		eSTATE_FADE_IN,
		eSTATE_FADE_IN_END,
		eSTATE_FADE_OUT,
		eSTATE_FADE_OUT_END,
	};
private:
	int fadeFrame;
	int alphaParam;
	int color;
	State state;
public:
	Fade();
	~Fade();

	State GetState()const{return state;}

	void SetFadeIn(int fadeFrame);
	void SetFadeOut(int fadeFrame);
	void SetColor(int color){this->color=color;}

	void Draw();
	void Process();
};