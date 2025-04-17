#include "game.hpp"
#include "enemy.hpp"
#include "bullet.hpp"
#include <iostream>
#include <vector>


Game::Game()
{
    snd_explosion_red = LoadSound("resources/sound_effects/explosion_red.wav");
    startSprite = LoadTexture("resources/UI/start.png");  
    stageSprite = LoadTexture("resources/UI/stage1.png");
    enemies = createEnemy(); 

    for (int i = 0; i < 10; i++) {
        char file[32];
        sprintf_s(file, sizeof(file),"resources/UI/score/n%d.png", i);
        scoreTextures[i] = LoadTexture(file);
     
    }

    score = 0;
}

Game::~Game()
{
	UnloadSound(snd_explosion_red); // Unload sound

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

        // Activar un nuevo enemigo cada 1 segundo
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
    DeleteInactiveBullet(); 
    CheckForCollisions();
}

void Game::Draw()
{
    spaceship.Draw();
    for (auto& bullet : spaceship.bullets) {
        bullet.Draw();
    }

    for (auto& enemy : enemies) {
        enemy.Draw();  
    }

    Vector2 scorePosition = { 70, 30 };
    float scoreWidth = 120; 


    float scoreCenterX = scorePosition.x + (scoreWidth / 2);
    float digitWidth = scoreTextures[0].width * 4.0f;

 
    float startX = scoreCenterX - (digitWidth * 2) -10;

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
        if (!bullet.active) continue;

        Rectangle bulletRect = bullet.getRect();

        for (auto it = enemies.begin(); it != enemies.end(); )
        {
            Rectangle enemyRect = it->getRect();

            if (CheckCollisionRecs(enemyRect, bulletRect))
            {
                if (it->type == 1 || it->type ==2)
                {
                    score += 40;
                    std::cout << "Enemigo destruido! Puntaje: " << score << std::endl;
                }
				PlaySound(snd_explosion_red); // Reproducir sonido de explosión
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

std::vector<Enemy> Game::createEnemy()
{
    std::vector<Enemy> enemies;

    
    enemies.push_back(Enemy(1, { 0.0f, 350.0f }, RED_PATH, true));
    enemies.push_back(Enemy(1, { -50.0f, 350.0f }, ORANGE_PATH, true));
    enemies.push_back(Enemy(1, { 0.0f, 450.0f }, RED_PATH, true));
    enemies.push_back(Enemy(1, { -50.0f, 450.0f }, ORANGE_PATH, true));


    enemies.push_back(Enemy(2, { 1024.0f, 350.0f }, RED_PATH, false));
    enemies.push_back(Enemy(2, { 1074.0f, 350.0f }, ORANGE_PATH, false));
    enemies.push_back(Enemy(2, { 1024.0f, 450.0f }, RED_PATH, false));
    enemies.push_back(Enemy(2, { 1074.0f, 450.0f }, ORANGE_PATH, false));

    return enemies;
}