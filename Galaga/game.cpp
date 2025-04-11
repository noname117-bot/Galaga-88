#include "game.hpp"
#include "enemy.hpp"
#include "bullet.hpp"
#include <iostream>
#include <vector>

Game::Game()
{
    enemies = createEnemy();  // Crea los enemigos
}

Game::~Game()
{
}

void Game::Update()
{
    spaceship.Update();
    for (auto& bullet : spaceship.bullets) {
        bullet.Update();
    }

    // Actualiza todos los enemigos
    for (auto& enemy : enemies) {
        enemy.Update();  // Actualiza la posición de cada enemigo
    }

    DeleteInactiveBullet();  // Elimina las balas inactivas

    CheckForCollisions();
}

void Game::Draw()
{
    spaceship.Draw();
    for (auto& bullet : spaceship.bullets) {
        bullet.Draw();
    }

    // Dibuja a los enemigos
    for (auto& enemy : enemies) {
        enemy.Draw();  // Dibuja cada enemigo en su nueva posición
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

void Game::CheckForCollisions()
{

    for (auto& bullet : spaceship.bullets)
    {
        auto it = enemies.begin();
        while (it != enemies.end())
        {
            if (CheckCollisionRecs(it->getRect(), bullet.getRect()))
            {
                it = enemies.erase(it);
                bullet.active = false;
            }
            else {
                ++it;
            }
        }
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
    std::vector<Enemy> enemies;

    int screenWidth = 1024;
    int centerX = screenWidth / 2;
    float offsetX = centerX - 375;

    // Creamos enemigos en las posiciones iniciales
    for (int row = 0; row < 2; row++) {
        float y = 200 + row * 55;

        float x_left = offsetX + 600 + row * 50;
        enemies.push_back(Enemy(1, { x_left, y }));

        float x_left2 = offsetX + 650 + row * 50;
        enemies.push_back(Enemy(1, { x_left2, y }));

        float x_right = offsetX + 100 - row * 50;
        enemies.push_back(Enemy(1, { x_right, y }));

        float x_right2 = offsetX + 150 - row * 50;
        enemies.push_back(Enemy(1, { x_right2, y }));
    }

    return enemies;
}