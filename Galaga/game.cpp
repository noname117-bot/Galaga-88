#include "game.hpp"
#include "enemy.hpp"
#include "bullet.hpp"
#include "enemy_bullet.hpp"
#include <iostream>
#include <vector>


Game::Game()
{
    snd_explosion_red = LoadSound("resources/sound_effects/explosion_red.wav");
    snd_shipExplosion = LoadSound("resources/sound_effects/explosion_starship.wav");
    startSprite = LoadTexture("resources/UI/start.png");
    stageSprite = LoadTexture("resources/UI/stage1.png");
    enemies = createEnemy();

    for (int i = 0; i < 10; i++) {
        char file[32];
        sprintf_s(file, sizeof(file), "resources/UI/score/n%d.png", i);
        scoreTextures[i] = LoadTexture(file);
    }

    currentLevel = 1;
    score = 0;
    cheat = false;
    levelCompleted = false; 
}

Game::~Game()
{
    UnloadSound(snd_explosion_red);

    for (int i = 0; i < 10; i++)
    {
        UnloadTexture(scoreTextures[i]);
    }
}

void Game::Update()
{
    static float enemyActivationTimer = 0;
    static int nextEnemyToActivate = 0;

    if (nextEnemyToActivate < enemies.size()) {
        enemyActivationTimer += GetFrameTime();

        if (enemyActivationTimer >= 1.0f) {
            enemies[nextEnemyToActivate].Activate();
            nextEnemyToActivate++;
            enemyActivationTimer = 0;
        }
    }
    textTimer += GetFrameTime();

    spaceship.Update();
    for (auto& bullet : spaceship.bullets) {
        bullet.Update();
    }

    for (auto& enemy : enemies) {
        if (enemy.IsActive()) {
            enemy.Update();
        }
    }

    if (IsKeyPressed(KEY_ONE)) {
        cheat = true;
    }

    if (IsKeyPressed(KEY_TWO)) {
        cheat = false;
    }

    if (IsKeyPressed(KEY_THREE)) {
        //levelCompleted = true;
        for (auto it = enemies.begin(); it != enemies.end(); ) {

                it = enemies.erase(it);

        }

    }

    enemies_shot();
    if (cheat == false) CheckForEnemyBulletCollisions();

    for (auto& enemy_bullet : enemy_bullets) {
        enemy_bullet.Update();
    }

    DeleteInactiveEnemyBullet();
    DeleteInactiveBullet();
    CheckForCollisions();

    if (areEnemiesDefeated() && !levelCompleted) {
        levelCompleted = true;
    }
}

void Game::Draw()
{
    spaceship.Draw();
    for (auto& bullet : spaceship.bullets) {
        bullet.Draw();
    }

    if (currentLevel == 1) {
        for (auto& enemy_bullet : enemy_bullets) {
            enemy_bullet.Draw();
        }

        for (auto& enemy : enemies) {
            enemy.Draw();
        }
    }

    Vector2 scorePosition = { 70, 30 };
    float scoreWidth = 120;

    float scoreCenterX = scorePosition.x + (scoreWidth / 2);
    float digitWidth = scoreTextures[0].width * 4.0f;

    float startX = scoreCenterX - (digitWidth * 2) - 10;

    int tempScore = score;

    float spacing = 5.0f; // espacio entre dígitos
    float totalWidth = digitWidth * 4 + spacing * 3;

    if (tempScore == 0) {
        DrawTextureEx(scoreTextures[0], { startX + 3 * (digitWidth + spacing), scorePosition.y + 40 }, 0.0f, 4.0f, WHITE);
    }
    else {
        int digits[4] = { 0, 0, 0, 0 };

        for (int i = 3; i >= 0 && tempScore > 0; i--) {
            digits[i] = tempScore % 10;
            tempScore /= 10;
        }

        bool leadingZero = true;
        for (int i = 0; i < 4; i++) {
            if (leadingZero && digits[i] == 0 && i != 3) {
                continue;
            }
            else {
                leadingZero = false;
                DrawTextureEx(scoreTextures[digits[i]], { startX + i * (digitWidth + spacing), scorePosition.y + 40 }, 0.0f, 4.0f, WHITE);
            }
        }
    }
}

void Game::Reset() {
    for (int i = 0; i < 10; i++)
    {
        UnloadTexture(scoreTextures[i]);
    }
    currentLevel = 1;
    enemies = createEnemy();
    score = 0;
    spaceship.Reset();
    levelCompleted = false; 
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

void Game::enemies_shot()
{
    // Solo permitir que los enemigos disparen en el nivel 1
    if (currentLevel != 1) {
        return;
    }

    for (auto bull = enemies.begin(); bull != enemies.end(); ++bull)
    {
        if (GetRandomValue(0, 100) < 0.01) { // Probabilidad de disparar de cada enemigo
            enemy_bullets.push_back(enemy_Bullet({ bull->getPosition().x , bull->getPosition().y + 20 }, -6));
        }
    }
}

void Game::CheckForEnemyBulletCollisions()
{
    if (spaceship.lives > -1) {
        for (auto& bullet : enemy_bullets) {
            if (!bullet.active) continue;

            Rectangle bulletRect = bullet.getRect();
            Rectangle spaceshipRect = spaceship.getRect();

            if (CheckCollisionRecs(spaceshipRect, bulletRect)) {

                for (auto& bullet : enemy_bullets) {
                    bullet.active = false;
                }

                spaceship.isExploding = true;
                spaceship.explosionPosition = { spaceshipRect.x + 20 + spaceshipRect.width / 2, spaceshipRect.y + 20 + spaceshipRect.height / 2 };

                PlaySound(snd_shipExplosion); // Reproducir sonido de explosión
                break;
            }
        }
    }
}

void Game::CheckForCollisions()
{
    for (auto& bullet : spaceship.bullets)
    {
        if (!bullet.active) continue;

        Rectangle bulletRect = bullet.getRect();

        for (auto it = enemies.begin(); it != enemies.end(); )
        {
            Rectangle enemyRect = it->getRect();

            if (CheckCollisionRecs(enemyRect, bulletRect))
            {
                if (it->type == 1 || it->type == 2)
                {
                    score += 40;
                }
                PlaySound(snd_explosion_red); // Reproducir sonido de explosión

                if (it->type == 3)
                {
                    score += 50;
                }
                PlaySound(snd_explosion_red);

                it = enemies.erase(it);
                bullet.active = false;

                break;  // Una bala solo puede impactar un enemigo
            }
            else
            {
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

void Game::DeleteInactiveEnemyBullet()
{
    for (auto it = enemy_bullets.begin(); it != enemy_bullets.end();)
    {
        if (!it->active)
        {
            it = enemy_bullets.erase(it);
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
    int spacing = 100;
    int formationGap = 400;

    float y1 = 200;
    float y2 = 255;

    float leftGroupCenter = centerX - (formationGap / 2);

    enemies.push_back(Enemy(1, { leftGroupCenter - spacing / 2, y1 }, 0, true));
    enemies.push_back(Enemy(1, { leftGroupCenter + spacing / 2, y1 }, 0, true));
    enemies.push_back(Enemy(1, { leftGroupCenter - spacing, y2 }, 1, true));
    enemies.push_back(Enemy(1, { leftGroupCenter, y2 }, 1, true));

    float rightGroupCenter = centerX + (formationGap / 2);

    // pathType=0 para los enemigos de la derecha y startFromLeft=false
    enemies.push_back(Enemy(1, { rightGroupCenter - spacing / 2, y1 }, 0, false));
    enemies.push_back(Enemy(1, { rightGroupCenter + spacing / 2, y1 }, 0, false));
    enemies.push_back(Enemy(1, { rightGroupCenter, y2 }, 1, false));
    enemies.push_back(Enemy(1, { rightGroupCenter + spacing, y2 }, 1, false));

    return enemies;
}

bool Game::areEnemiesDefeated() {
    return enemies.empty();
}

void Game::nextLevel() {
    currentLevel++;
    enemies = createEnemy(); 
    levelCompleted = false; 

    static float enemyActivationTimer = 0;
    static int nextEnemyToActivate = 0;
    enemyActivationTimer = 0;
    nextEnemyToActivate = 0;
}

int Game::getCurrentLevel() {
    return currentLevel;
}

bool Game::isLevelCompleted() {
    return levelCompleted;
}