#pragma once
#include "raylib.h"

class Transition{
private:
	Texture2D background1;
	Texture2D shipSprite;
	Texture2D spaceship;
	Texture2D startAnnouncement;
	Texture2D scoreTextures[10];

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

	float startAnnouncementTimer;
	const float startAnnouncementDuration = 1.5f; // Solo esta línea
	bool showStartAnnouncement;
	bool announcementStarted;

	int currentScore;
	void DrawScoreUI();

public:
	Transition();
	~Transition();

	void Update();
	void Draw();
	bool IsFinished()const;
	void SetScore(int score);
	void Reset();

	Sound snd_animation;
};