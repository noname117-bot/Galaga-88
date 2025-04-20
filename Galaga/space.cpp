#include "space.hpp"
#include <iostream>



Spaceship::Spaceship()
{
	snd_bullet = LoadSound("resources/sound_effects/effect_nullet_starship.wav");
	lives = 2;
	image = LoadTexture("resources/spaceship.png"); 
	position.x = (GetScreenWidth() - image.width) / 2;
	position.y = GetScreenHeight() - image.height - 150;
	lastFiretime = 0.0;
	reloadTime = 0.05f;
	bulletCount = 0;

	spriteSheet = LoadTexture("resources/aparicion1-sheet.png");
	explosion_spriteSheet = LoadTexture("resources/explosion_starhip.png");
	frameWidth = 16;
	frameHeight = 32;
	currentFrame = 0;
	frameTime = 0.2f; 
	frameCounter = 0.0f;
	animation = false;
	startY = 2;
	startX = 2;

	game_over = LoadTexture("resources/game_over.png");
	livesTexture = LoadTexture("resources/spaceship.png");

	isExploding = false;
	explosionFrame = 0;
	explosionFrameTime = 0.1f; 
	explosionTimer = 0.0f;
	explosionPosition = { 0.0f, 0.0f };
}
Spaceship::~Spaceship()
{
	UnloadSound(snd_bullet); 
	UnloadTexture(image);
	UnloadTexture(livesTexture);	
}
void Spaceship::Draw()
{
	if (!animation)
	{
		int frameX = startX + currentFrame * (frameWidth + 4); 

		Rectangle sourceRect = { frameX, startY, frameWidth, frameHeight };
		Rectangle destRect = { position.x, position.y, frameWidth * 4, frameHeight * 4 }; 

		DrawTexturePro(spriteSheet, sourceRect, destRect, { 0, 0 }, 0.0f, WHITE);
	}
	else { 
		
		
		if (isExploding) {
			int frameWidth = explosion_spriteSheet.width / 8; // Ancho de un fotograma
			int frameHeight = explosion_spriteSheet.height;  // Altura de un fotograma
			Rectangle sourceRect = { frameWidth * explosionFrame, 0, frameWidth, frameHeight };
			Rectangle destRect = { explosionPosition.x, explosionPosition.y, frameWidth * 4.0f, frameHeight * 4.0f };
			DrawTexturePro(explosion_spriteSheet, sourceRect, destRect, { frameWidth * 2.0f, frameHeight * 2.0f }, 0.0f, WHITE);
		}
		else
		{
			if (lives > -1) {
				DrawTextureEx(image, position, 0.0f, 4.0f, WHITE);
			}
			else 
			{
				DrawTextureEx(game_over, { 396,400 } , 0.0f, 4.0f, WHITE);
			}
		}
	}

}	
void Spaceship::MoveLeft()	
{
	position.x -= 6;
	if (position.x < 0)
	{
		position.x = 0;
	}
}
void Spaceship::MoveRight()
{
	position.x += 6;
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
			PlaySound(snd_bullet); // Reproducir el sonido del disparo
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
	if (isExploding == false && lives > -1) FireLaser();

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

	// Actualizar la animación de la explosión
	if (isExploding) {
		explosionTimer += GetFrameTime();
		if (explosionTimer >= explosionFrameTime) {
			explosionTimer = 0.0f;
			explosionFrame++;

			// Si se completan los 8 fotogramas, desactiva la explosión
			if (explosionFrame >= 8) {
				explosionFrame = 0;			
				isExploding = false;
				lives--;
				//if (lives < 1) DrawTextureEx(life1, { 135, 860 }, 0.0f, 4.0f, BLACK);
				//if (lives < 2) DrawTextureEx(life2, { 70, 860 }, 0.0f, 4.0f, BLACK);

			}
		}
	}
}

Rectangle Spaceship::getRect()
{
	return { position.x, position.y,float(image.width * 4), float(image.height * 4) };
}
