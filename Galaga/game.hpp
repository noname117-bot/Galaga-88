#pragma once
#include "space.hpp"
#include "enemy.hpp"
#include "enemy_bullet.hpp"

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
	std::vector<enemy_Bullet> enemy_bullets;

	void CheckForCollisions();
	void CheckForEnemyBulletCollisions();
	Spaceship& getSpaceship() { return spaceship; }
private:
	void DeleteInactiveBullet();
	void DeleteInactiveEnemyBullet();
	std::vector<Enemy> createEnemy();
	void enemies_shot();
	void Reset();


	Spaceship spaceship;
	std::vector<Enemy> enemies;
	
	Sound snd_explosion_red;
	Sound snd_shipExplosion;
};