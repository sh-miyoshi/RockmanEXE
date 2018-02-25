#pragma once

class Fps{
	float ave;
	unsigned int count, baseCnt;
	int *frame;
public:
	static const int FPS = 60;

	Fps();
	~Fps();

	void Process();
	void Draw(int x, int y);
};