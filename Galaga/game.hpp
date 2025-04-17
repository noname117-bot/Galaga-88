#pragma once
#include "space.hpp"
#include "enemy.hpp"

class Game {
public:
	Game();
	~Game();
	void Update();
	void Draw();
	void HandleInput();
	int score;

	Texture2D scoreTextures[10];
	Texture2D startSprite;  // Sprite para "START"
	Texture2D stageSprite;  // Sprite para "STAGE 1"
	float textTimer = 0.0f;

	void CheckForCollisions();
private:
	void DeleteInactiveBullet();
	std::vector<Enemy> createEnemy();
	Spaceship spaceship;
	std::vector<Enemy> enemies;
	Sound snd_explosion_red;
};