#include "space.hpp"
#include <iostream>



Spaceship::Spaceship()
{
	snd_bullet = LoadSound("resources/sound_effects/effect_nullet_starship.wav");
	lives = 3;
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
	
	game_over = LoadTexture("resources/UI/game_over.png");
	livesTexture = LoadTexture("resources/spaceship.png");
	isGameOver = false;
	isExploding = false;
	explosionFrame = 0;
	explosionFrameTime = 0.1f; 
	explosionTimer = 0.0f;
	explosionPosition = { 0.0f, 0.0f };

	isLocked = false;
	lockTimer = 0.0f;

	state = ALIVE;
	respawnTimer = 0.0f;
	respawnAnimationTimer = 0.0f;
	isRespawning = false;

	respawnTexture = LoadTexture("resources/respawn.png");
	respawnFrame = 0;
	respawnFrameCounter = 0;
	respawnFrameSpeed = 8;
	respawnFrameWidth = respawnTexture.width / 2; 
	respawnFrameHeight = respawnTexture.height;

	respawnStartPosition = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() + 100.0f }; 
	respawnTargetPosition = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() - 100.0f };
}
Spaceship::~Spaceship()
{
	UnloadSound(snd_bullet); 
	UnloadTexture(image);
	UnloadTexture(livesTexture);
	UnloadTexture(game_over);
	UnloadTexture(respawnTexture);
}

void Spaceship::Reset() {
	animation = false;
	UnloadTexture(livesTexture);
	livesTexture = LoadTexture("resources/spaceship.png");
}

void Spaceship::Draw()
{

	if (state == RESPAWNING) {
		DrawRespawn();
		return;
	}

	if (state != ALIVE) return;

	if (!animation)
	{
		int frameX = startX + currentFrame * (frameWidth + 4); 

		Rectangle sourceRect = { frameX, startY, frameWidth, frameHeight };
		Rectangle destRect = { position.x, position.y, frameWidth * 4, frameHeight * 4 }; 

		DrawTexturePro(spriteSheet, sourceRect, destRect, { 0, 0 }, 0.0f, WHITE);
	}
	else { 
		
		
		if (isExploding) {
			int frameWidth = explosion_spriteSheet.width / 8; 
			int frameHeight = explosion_spriteSheet.height;
			Rectangle sourceRect = { frameWidth * explosionFrame, 0, frameWidth, frameHeight };
			Rectangle destRect = { explosionPosition.x, explosionPosition.y, frameWidth * 4.0f, frameHeight * 4.0f };
			DrawTexturePro(explosion_spriteSheet, sourceRect, destRect, { frameWidth * 2.0f, frameHeight * 2.0f }, 0.0f, WHITE);
		}
		else
		{
			if (lives > 0) {
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
	position.x -= 4;
	if (position.x < 0)
	{
		position.x = 0;
	}
}
void Spaceship::MoveRight()
{
	position.x += 4;
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

void Spaceship::Update() 
{

	if (state == RESPAWNING) {
        UpdateRespawn();
        return; 
    }
    
    if (state != ALIVE) return;

	if (isLocked) {
		lockTimer -= GetFrameTime();
		if (lockTimer <= 0.0f) {
			isLocked = false;
			lockTimer = 0.0f;
		}
	}

	if (!isLocked) {
		if (IsKeyDown(KEY_LEFT)) {
			MoveLeft();
		}
		if (IsKeyDown(KEY_RIGHT)) {
			MoveRight();
		}
	}

	// Disparo (permitir disparo incluso si está bloqueada)
	if (isExploding == false && lives > -1) FireLaser();

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

	//  la animación de la explosión
	if (isExploding) {
		explosionTimer += GetFrameTime();
		if (explosionTimer >= explosionFrameTime) {
			explosionTimer = 0.0f;
			explosionFrame++;

			if (explosionFrame >= 8) {
				explosionFrame = 0;			
				isExploding = false;
				lives--;

			

				if (lives > 0)
				{
					animation = false;
					currentFrame = 0;
					frameCounter = 0.0f;

					position.x = (GetScreenWidth() - frameWidth * 4.0f) / 2; 
					position.y = GetScreenHeight() - frameHeight * 4.0f - 80;
				}
				
				
				

			}
		}
	}
}

void Spaceship::LockInCenter(float duration) {
	isLocked = true;
	lockTimer = duration;
	position.x = (GetScreenWidth() - frameWidth * 4.0f) / 2;
}

void Spaceship::StartRespawn() {
	if (lives > 0) {
		state = RESPAWNING;
		respawnTimer = 0.0f;
		respawnAnimationTimer = 0.0f;
		respawnFrame = 0;
		respawnFrameCounter = 0;
		position = respawnStartPosition;
		isRespawning = true;
	}
}

void Spaceship::UpdateRespawn() {
	if (state != RESPAWNING) return;

	respawnTimer += GetFrameTime();
	respawnFrameCounter++;
	if (respawnFrameCounter >= (60 / respawnFrameSpeed)) {
		respawnFrameCounter = 0;
		respawnFrame = (respawnFrame + 1) % 2;
	}

	float animationDuration = 2.0f;
	if (respawnTimer < animationDuration) {
		float progress = respawnTimer / animationDuration;
		position.y = respawnStartPosition.y + (respawnTargetPosition.y - respawnStartPosition.y) * progress;
		position.x = respawnTargetPosition.x; 
	}
	else {
		position = respawnTargetPosition;
		state = ALIVE;
		isRespawning = false;
	}
}

void Spaceship::DrawRespawn() {
	if (state != RESPAWNING) return;
	Rectangle sourceRec = {	respawnFrame * respawnFrameWidth,	0,	respawnFrameWidth,respawnFrameHeight};
	Rectangle destRec = {	position.x - (respawnFrameWidth * 4) / 2,position.y - (respawnFrameHeight * 4) / 2,respawnFrameWidth * 4,	respawnFrameHeight * 4};
	Vector2 origin = { 0, 0 };
	DrawTexturePro(respawnTexture, sourceRec, destRec, origin, 0.0f, WHITE);
}

Rectangle Spaceship::getRect()
{
	return { position.x, position.y,float(image.width * 4), float(image.height * 4) };
}
