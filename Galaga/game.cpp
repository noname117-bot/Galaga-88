#include "game.hpp"
#include<iostream>
using  namespace std;
#include "bullet.hpp"
#include <vector>



Game::Game()
{
}

Game::~Game()
{

}

void Game::Update()
{
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
