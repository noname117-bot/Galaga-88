#include "space.hpp"

Spaceship::Spaceship()
{
	image = LoadTexture("resources/spaceship.png"); //nave original
	position.x = (GetScreenWidth() - image.width) / 2;
	position.y = GetScreenHeight() - image.height - 150;
	lastFiretime = 0.0;
	reloadTime = 0.05f;
	bulletCount = 0;

	//nave animacion entrada
	spriteSheet = LoadTexture("resources/aparicion1-sheet.png");
	frameWidth = 16;
	frameHeight = 32;
	currentFrame = 0;
	frameTime = 0.2f; // Tiempo entre cada cambio de frame
	frameCounter = 0.0f;
	animation = false;
	startY = 2;
	startX = 2;
}
Spaceship::~Spaceship()
{
	UnloadTexture(image);
}
void Spaceship::Draw()
{
	if (!animation)
	{
		int frameX = startX + currentFrame * (frameWidth + 4); // 4 es la separación entre sprites

		Rectangle sourceRect = { frameX, startY, frameWidth, frameHeight };
		Rectangle destRect = { position.x, position.y, frameWidth * 4, frameHeight * 4 }; // Escalamos 4x

		DrawTexturePro(spriteSheet, sourceRect, destRect, { 0, 0 }, 0.0f, WHITE);
	}
	else { // cuando la animacion finalice solo quedara la nave "original"
		DrawTextureEx(image, position, 0.0f, 4.0f, WHITE); // draw the spaceship

	}

}	
void Spaceship::MoveLeft()
{
	position.x -= 3;
	if (position.x < 0)
	{
		position.x = 0;
	}
}
void Spaceship::MoveRight()
{
	position.x += 3;
	float scaleWidth = image.width * 4.0f;
	if (position.x + scaleWidth > GetScreenWidth()) {
		position.x = GetScreenWidth() - scaleWidth;
	}

}
void Spaceship::FireLaser()
{
	if (bulletCount <2 && GetTime() - lastFiretime >= 0.20)// el 0.20 es la diferencia de segundos entre la primera y la segunda bala
	{
		if(IsKeyDown(KEY_X))
		{
			bullets.push_back(Bullet({ position.x + (image.width * 4.0f / 2) - 2, position.y }, -6));
			bulletCount++;
			lastFiretime = GetTime();

		}
	
	}

	if (bulletCount >= 2 && GetTime() - lastFiretime >= reloadTime)
	{
		bulletCount = 0;
	}
}

void Spaceship::Update() // funcion para cuando la nave se mueva dispare al mismo tiempo
{
	if (IsKeyDown(KEY_LEFT)) {
		MoveLeft();
	}
	if (IsKeyDown(KEY_RIGHT)) {
		MoveRight();
	}

	// Disparo
	FireLaser();

	if (!animation)
	{
		frameCounter += GetFrameTime();

		if (frameCounter >= frameTime)
		{
			frameCounter = 0;
			currentFrame++;

			if (currentFrame >= 4) // Hay 4 frames después de la nave original
			{
				animation = true;
				currentFrame = 3; // Se queda en el último frame
			}
		}
	}
}
