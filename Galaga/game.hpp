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

	void CheckForCollisions();
private:
	void DeleteInactiveBullet();
	std::vector<Enemy> createEnemy();
	Spaceship spaceship;
	std::vector<Enemy> enemies;
};