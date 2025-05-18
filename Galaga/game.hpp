#pragma once
#include "space.hpp"
#include "enemy.hpp"
#include "enemy_bullet.hpp"
#include "bosszilla.hpp"  

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
	void CheckForBossCollisions();
	Spaceship& getSpaceship() { return spaceship; }

	int getCurrentLevel() const { return currentLevel; }

	bool areEnemiesDefeated()const { return enemies.empty(); }

	bool areEnemiesDefeated();
	void nextLevel();
	int getCurrentLevel();
	bool isLevelCompleted();

private:
	void DeleteInactiveBullet();
	void DeleteInactiveEnemyBullet();
	std::vector<Enemy> createEnemy();
	void enemies_shot();
	void Reset();
	void SpawnBoss();

	
	bool levelCompleted;
	Spaceship spaceship;
	std::vector<Enemy> enemies;
	Bosszilla* boss;
	bool bossActive;
	int enemiesDefeated;
	int currentLevel = 1;

	Sound snd_explosion_red;
	Sound snd_shipExplosion;
	Sound snd_bossAppear;
};