#include "transicion.hpp"

Transition::Transition()
{
	background = LoadTexture("resources/bg_startgame.png");
	spaceship = LoadTexture("resources/spaceship.png");
	spriteSheet = LoadTexture("resources/aparicion1-sheet.png");

	shipY = 900;
	transitionTime = 0;

	currentFrame = 3;
	frameCounter = 0;
	frameTime = 0.15f;

	finished = false;
	moving = true;
	delayTime = 1.0f;

	sourceRect = { currentFrame * (spriteSheet.width / 4), 0.0f, static_cast<float>(spriteSheet.width) / 4, static_cast<float>(spriteSheet.height) };
}

Transition::~Transition()
{
	UnloadTexture(background);
	UnloadTexture(spaceship);
	UnloadTexture(spriteSheet);
}


void Transition::Update()
{
	if (finished) return;

	transitionTime += GetFrameTime();



	if (moving && transitionTime >= delayTime)
	{
		moving = false;
	}

	if (moving && transitionTime >= delayTime) {
		moving = false; 
	}

	if (!moving) {
		if (shipY > 400) {
			shipY -= 100 * GetFrameTime();  
		}
	}

	if (transitionTime >= 3.0f &&transitionTime < 5.0f)
	{
		frameCounter += GetFrameTime();
		if (frameCounter >= frameTime) {
			frameCounter = 0;
			currentFrame--;
			if (currentFrame < 0)
			{
				finished = true; 
			}
			sourceRect.x = currentFrame * (spriteSheet.width / 4);
		}
	}

}



void Transition::Draw()
{
	if (transitionTime < 2.5f) {
		DrawTextureEx(background, { 0,0 }, 0.0f, 4.0f, WHITE);
	}

	if(transitionTime<3.0f)
	{
		float scale = 4.0;
		DrawTextureEx(spaceship, { 512 - spaceship.width * scale / 2, shipY - spaceship.height * scale / 2 }, 0.0f, scale, WHITE);

	}

	if (!finished && transitionTime >= 5.0f) {
		DrawTexturePro(spriteSheet, sourceRect, { 512, shipY, 128, 128 }, { 64, 64 }, 0, WHITE);
	}


}


bool Transition::IsFinished() const
{
	return finished;
}
