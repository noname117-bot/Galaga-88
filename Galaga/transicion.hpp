#pragma once
#include "raylib.h"

class Transition{
private:
	Texture2D background;
	Texture2D spriteSheet;
	Texture2D spaceship;

	Rectangle sourceRect;

	float shipY;
	float transitionTime;
	float currentFrame;
	float frameCounter;
	float frameTime;

	bool finished;
	bool moving;
	float delayTime;

public:
	Transition();
	~Transition();

	void Update();
	void Draw();
	bool IsFinished()const;




};