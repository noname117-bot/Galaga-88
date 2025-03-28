#pragma once;
#include "raylib.h"

class Menu {
private:
	Texture2D spriteSheet;  // Spritesheet con los 4 frames
	Rectangle sourceRect;   // Secci�n actual del spritesheet
	
	int currentFrame;       // Frame actual de la animaci�n
	float frameTime;        // Tiempo por frame
	float frameCounter;     // Contador para cambiar de frame
	bool selected;
	
	float animationTime;
	float maxTime;

	int totalCycles;
	int cyclesCompleted;
public:
	Menu();
	~Menu();

	void Update();
	void Draw();
	bool SingleModeSelected();

};
