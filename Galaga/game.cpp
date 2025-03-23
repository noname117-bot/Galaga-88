#include "game.hpp"
#include<iostream>
using  namespace std;
#include "bullet.hpp"
#include <vector>



Game::Game()
{
	enemies = createEnemy();
}

Game::~Game()
{

}

void Game::Update()
{

	spaceship.Update();
	for (auto& bullet : spaceship.bullets)
	{
		bullet.Update();
	}
	DeleteInactiveBullet();
}

void Game::Draw()
{
	spaceship.Draw();
	for (auto& bullet : spaceship.bullets) {
		bullet.Draw();
	}

	for(auto&enemies:enemies)
	{
		enemies.Draw();
	}

}

void Game::HandleInput()
{
	if (IsKeyDown(KEY_LEFT)) {
		spaceship.MoveLeft();
	}
	else if (IsKeyDown(KEY_RIGHT)) {
		spaceship.MoveRight();
	}
	else if (IsKeyDown(KEY_X)) {
		spaceship.FireLaser();
	} 

}

void Game::DeleteInactiveBullet()
{

	for (auto it = spaceship.bullets.begin(); it != spaceship.bullets.end();)
	{
		if (!it->active)
		{
			it = spaceship.bullets.erase(it);
		}
		else {
			++it;
		}
	}

}

std::vector<Enemy> Game::createEnemy()
{
	std::vector<Enemy>enemies;
	for (int row = 0; row < 2; row++) // 2 filas de enemigos
	{
		
		float y = 200 + row * 55; // Espaciado entre las filas. 
		
		float x_left = 600 + row * 50; // Posición de los enemigos en el extremo izquierdo
		enemies.push_back(Enemy(1, { x_left, y }));

		float x_left2 = 650 + row * 50;  
		enemies.push_back(Enemy(1, { x_left2, y }));

		float x_right = 100 - row * 50; // Posición de los enemigos en el extremo derecho
		enemies.push_back(Enemy(1, { x_right, y }));

		float x_right2 = 150 - row * 50;  // Posición del segundo enemigo a la derecha
		enemies.push_back(Enemy(1, { x_right2, y }));
	}


	return enemies;
}
