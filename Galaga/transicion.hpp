#pragma once
#include "raylib.h"

class Transition{
private:
	Texture2D background1;
	Texture2D shipSprite;
	Texture2D spaceship;

	Texture2D score;

	Rectangle sourceRect;

	float shipY;
	float transitionTime;
	float currentFrame;
	float frameCounter;
	float frameTime;

	float backgroundFrameCounter;
	float backgroundFrameTime;
	float currentPhase;

	bool finished;
	bool moving;
	float delayTime;

	bool  waitBeforeMoving;
	float backgroundY;
	int currentBackgroundFrame;

public:
	Transition();
	~Transition();

	void Update();
	void Draw();
	bool IsFinished()const;




};