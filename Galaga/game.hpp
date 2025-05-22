
#pragma once
#include "space.hpp"
#include <vector>
#include "enemy.hpp"
#include "enemy_bullet.hpp"

class Game {
public:
    Game();
    ~Game();
    void Update();
    void Draw();
    void Reset();
    void HandleInput();
    void enemies_shot();
    void CheckForEnemyBulletCollisions();
    void CheckForCollisions();
    void DeleteInactiveBullet();
    void DeleteInactiveEnemyBullet();
    std::vector<Enemy> createEnemy();
    bool areEnemiesDefeated();
    void nextLevel();
    int getCurrentLevel();
    bool isLevelCompleted();

    Spaceship& getSpaceship() { return spaceship; }

private:
    Spaceship spaceship;
    std::vector<Enemy> enemies;
    std::vector<enemy_Bullet> enemy_bullets;
    Sound snd_explosion_red;
    Sound snd_shipExplosion;
    Texture2D startSprite;
    Texture2D stageSprite;
    Texture2D scoreTextures[10];
    int score;
    float textTimer = 0.0f;
    int currentLevel;
    bool levelCompleted;
};
