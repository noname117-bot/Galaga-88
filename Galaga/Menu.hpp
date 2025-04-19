#pragma once;
#include "raylib.h"

class Menu {
private:
	Texture2D spriteSheet;  // Spritesheet con los 4 frames
	Texture2D spriteSheet2;
	Rectangle sourceRect;   // Sección actual del spritesheet
	

	Texture2D introImage;
	Texture2D bee;
	Rectangle introSourceRect;

	int currentFrame;       // Frame actual de la animación
	float frameTime;        // Tiempo por frame
	float frameCounter;     // Contador para cambiar de frame

	bool selected;
	bool usingSecondAnimation;

	float frameTimeSelected;

	float animationTime;
	float maxTime;

	int totalCycles;
	int cyclesCompleted;
         

	bool showIntro;             
	bool introAnimationFinished; 
	int introCurrentFrame;       
	float introFrameCounter;    
	float introFrameTime;        

	bool beeActive;              
	bool beeFinished;           
	Vector2 beePosition;         
	float beeSpeed;

	bool waitingForBee;          
	float beeDelayTimer;         
	float beeDelayDuration;     

public:
	Menu();
	~Menu();

	void Update();
	void Draw();
	bool SingleModeSelected();
	Sound snd_selection;
};
