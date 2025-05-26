
#pragma once
#include "space.hpp"
#include <vector>
#include "Enemy.hpp"
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
    std::vector<Enemy> createBoss();
    bool areEnemiesDefeated();
    void nextLevel();
    int getCurrentLevel();
    bool isLevelCompleted();

    Spaceship& getSpaceship() { return spaceship; }
    float formationMoveTimer;
    bool formationMovingRight;

    // Y este método:
    void UpdateFormation();

    float globalShootTimer;     // Temporizador global
    float timeBetweenShots;     // Tiempo entre cada disparo de cualquier enemigo
    int currentShooterIndex;

    void UpdateEnemyShooting();
  
    // Variables para movimientos especiales
    float specialMovementTimer;
    float minTimeBetweenSpecialMoves;
    float maxTimeBetweenSpecialMoves;
    float nextSpecialMoveTime;

    // Métodos para movimientos especiales
    void UpdateSpecialMovements();
    void TriggerRandomSpecialMovement();
    std::vector<int> GetEnemiesInFormation();

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
    bool cheat;

    bool IsShowingLevelAnnouncement() const;
    void SetShowingLevelAnnouncement(bool showing);
    float levelAnnouncementTimer;
    bool showingLevelAnnouncement;
    bool enemiesCanAppear;
};
