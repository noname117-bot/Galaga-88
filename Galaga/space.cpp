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

	blinkingSpriteSheet = LoadTexture("resources/respawn.png"); // Spritesheet con frames de parpadeo
	blinkingFrame = 0;
	blinkingFrameCounter = 0;
	blinkingFrameSpeed = 10;
	blinkingFrameWidth = blinkingSpriteSheet.width / 2; 
	blinkingFrameHeight = blinkingSpriteSheet.height;

	respawnStartY = GetScreenHeight() + 50; // Comienza debajo de la pantalla
	respawnTargetY = GetScreenHeight() - frameHeight * 4.0f - 80; // Posici�n final
	respawnSpeed = 200.0f; 
}

Spaceship::~Spaceship()
{
	UnloadSound(snd_bullet);
	UnloadTexture(image);
	UnloadTexture(livesTexture);
	UnloadTexture(blinkingSpriteSheet); 
}

void Spaceship::Reset() {
	animation = false;
	currentFrame = 0;
	frameCounter = 0.0f;

	UnloadTexture(livesTexture);
	livesTexture = LoadTexture("resources/spaceship.png");

	UnloadTexture(blinkingSpriteSheet);
	blinkingSpriteSheet = LoadTexture("resources/respawn.png");
	state = ALIVE;
	isRespawning = false;
	blinkingFrame = 0;
	blinkingFrameCounter = 0;
	respawnTimer = 0.0f;
	respawnAnimationTimer = 0.0f;

	isExploding = false;
	explosionFrame = 0;
	explosionTimer = 0.0f;

	position.x = (GetScreenWidth() - image.width * 4.0f) / 2;
	position.y = GetScreenHeight() - image.height * 4.0f - 150;

	isLocked = false;
	lockTimer = 0.0f;
	lives = 3; 
}

void Spaceship::Draw()
{
	if (state == RESPAWNING) {
		DrawBlinkingRespawn();
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
	if (bulletCount < 2 && GetTime() - lastFiretime >= 0.20)
	{
		if (IsKeyDown(KEY_X))
		{
			PlaySound(snd_bullet);
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
		UpdateBlinkingRespawn();
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

	// Disparo (permitir disparo incluso si est� bloqueada)
	if (isExploding == false && lives > -1) FireLaser();

	if (!animation)
	{
		frameCounter += GetFrameTime();

		if (frameCounter >= frameTime)
		{
			frameCounter = 0;
			currentFrame++;

			if (currentFrame >= 4)
			{
				animation = true;
				currentFrame = 3;
			}
		}
	}

	// Animaci�n de la explosi�n
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
					StartBlinkingRespawn(); 
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

void Spaceship::StartBlinkingRespawn() {
	if (lives > 0) {
		state = RESPAWNING;
		respawnTimer = 0.0f;
		blinkingFrame = 0;
		blinkingFrameCounter = 0;

		position.x = (GetScreenWidth() - frameWidth * 4.0f) / 2;
		position.y = respawnStartY;

		isRespawning = true;
	}
}

void Spaceship::UpdateBlinkingRespawn() {
	if (state != RESPAWNING) return;

	respawnTimer += GetFrameTime();

	blinkingFrameCounter++;
	if (blinkingFrameCounter >= (60 / blinkingFrameSpeed)) {
		blinkingFrameCounter = 0;
		blinkingFrame = (blinkingFrame + 1) % 2; // Alternar entre frame 0 y 1
	}

	position.y -= respawnSpeed * GetFrameTime();

	if (position.y <= respawnTargetY) {
		position.y = respawnTargetY;
		state = ALIVE;
		isRespawning = false;

		animation = true;      
		currentFrame = 3;      
		frameCounter = 0.0f; 
	}
}

void Spaceship::DrawBlinkingRespawn() {
	if (state != RESPAWNING) return;


	Rectangle sourceRec = {
		blinkingFrame * blinkingFrameWidth,	0,	blinkingFrameWidth,blinkingFrameHeight	};

	Rectangle destRec = {position.x,	position.y,	blinkingFrameWidth * 4,	blinkingFrameHeight * 4};

	Vector2 origin = { 0, 0 };
	DrawTexturePro(blinkingSpriteSheet, sourceRec, destRec, origin, 0.0f, WHITE);
}

Rectangle Spaceship::getRect()
{
	return { position.x, position.y, float(image.width * 4), float(image.height * 4) };
}