#include "DxLib.h"
#include "define.h"
#include "fade.h"

Fade::Fade():fadeFrame(0),alphaParam(0),color(GetColor(0,0,0)),state(eSTATE_NULL){
}

Fade::~Fade(){
}

void Fade::SetFadeIn(int fadeFrame){
	this->fadeFrame=fadeFrame;
	alphaParam=255;
	state=eSTATE_FADE_IN;
}

void Fade::SetFadeOut(int fadeFrame){
	this->fadeFrame=fadeFrame;
	alphaParam=0;
	state=eSTATE_FADE_OUT;
}

void Fade::Draw(){
	if(state==eSTATE_FADE_IN||state==eSTATE_FADE_OUT){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,alphaParam);// アルファブレンド
		DrawBox(0,0,def::FMX,def::FMY,color,TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);// ノーマルブレンド
	}
}

void Fade::Process(){
	switch(state){
	case eSTATE_FADE_IN:
		alphaParam-=(255/fadeFrame);
		if(alphaParam<0)
			state=eSTATE_FADE_IN_END;
		break;
	case eSTATE_FADE_OUT:
		alphaParam+=(255/fadeFrame);
		if(alphaParam>=255)
			state=eSTATE_FADE_OUT_END;
		break;
	}
}