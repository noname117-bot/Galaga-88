#include "game.hpp"
#include "Enemy.hpp"
#include "bullet.hpp"
#include "enemy_bullet.hpp"
#include <iostream>
#include <vector>

Game::Game()
{
    snd_explosion_red = LoadSound("resources/sound_effects/explosion_red.wav");
    snd_shipExplosion = LoadSound("resources/sound_effects/explosion_starship.wav");
    startSprite = LoadTexture("resources/UI/start.png");

    levelAnnouncementTimer = 0.0f;
    showingLevelAnnouncement = true;
    enemiesCanAppear = false;

    globalShootTimer = 0.0f;
    timeBetweenShots = 1.0f;  // 1.2 segundos entre cada disparo (ajustable)
    currentShooterIndex = 0;

    specialMovementTimer = 0.0f;
    minTimeBetweenSpecialMoves = 5.0f;  // Mínimo 5 segundos
    maxTimeBetweenSpecialMoves = 10.0f; // Máximo 10 segundos
    nextSpecialMoveTime = GetRandomValue((int)minTimeBetweenSpecialMoves, (int)maxTimeBetweenSpecialMoves);

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

    // Variables para el movimiento sincronizado de la formación
    formationMoveTimer = 0.0f;
    formationMovingRight = true;
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
    // CONTROLAR EL ANUNCIO DEL NIVEL Y APARICIÓN DE ENEMIGOS
    if (showingLevelAnnouncement) {
        levelAnnouncementTimer += GetFrameTime();

        // Después de 2 segundos, permitir que aparezcan los enemigos
        if (levelAnnouncementTimer >= 2.0f) {
            showingLevelAnnouncement = false;
            enemiesCanAppear = true;
        }

        // Durante el anuncio, no actualizar enemigos
        spaceship.Update();
        for (auto& bullet : spaceship.bullets) {
            bullet.Update();
        }
        return; // Salir temprano, no procesar enemigos todavía
    }

    // ACTIVAR TODOS LOS ENEMIGOS A LA VEZ - SOLO SI YA PUEDEN APARECER
    if (!enemies.empty() && enemiesCanAppear) {
        bool allActivated = true;
        for (auto& enemy : enemies) {
            if (enemy.GetType() != 4 && !enemy.IsActive()) {
                allActivated = false;
                break;
            }
        }
        if (!allActivated) {
            for (auto& enemy : enemies) {
                if (enemy.GetType() != 4) {
                    enemy.Activate();
                }
            }
        }
    }

    textTimer += GetFrameTime();

    spaceship.Update();
    for (auto& bullet : spaceship.bullets) {
        bullet.Update();
    }

    // Actualizar enemigos individualmente - SOLO SI PUEDEN APARECER
    if (enemiesCanAppear) {
        for (auto& enemy : enemies) {
            if (enemy.IsActive() || enemy.isExploding == true) {
                enemy.Update();
            }
        }

        // CONTROLAR MOVIMIENTO SINCRONIZADO DE LA FORMACIÓN
        UpdateFormation();

        // ? NUEVO: CONTROLAR MOVIMIENTOS ESPECIALES
        UpdateSpecialMovements();

        // CONTROLAR DISPAROS ALTERNADOS
        UpdateEnemyShooting();
    }

    if (IsKeyPressed(KEY_ONE)) {
        cheat = true;
    }

    if (IsKeyPressed(KEY_TWO)) {
        cheat = false;
    }

    if (IsKeyPressed(KEY_THREE)) {
        for (auto it = enemies.begin(); it != enemies.end(); ) {
            it = enemies.erase(it);
        }
    }

    // ? COMENTAR O ELIMINAR LA FUNCIÓN ANTIGUA DE DISPAROS
    // Solo procesar disparos enemigos si pueden aparecer
    // if (enemiesCanAppear) {
    //     enemies_shot(); // ? COMENTAR ESTA LÍNEA - YA NO LA USAMOS
    //     if (cheat == false) CheckForEnemyBulletCollisions();
    // }

    // ? MANTENER SOLO LA VERIFICACIÓN DE COLISIONES
    if (enemiesCanAppear && cheat == false) {
        CheckForEnemyBulletCollisions();
    }

    for (auto& enemy_bullet : enemy_bullets) {
        enemy_bullet.Update();
    }

    DeleteInactiveEnemyBullet();
    DeleteInactiveBullet();

    // Solo verificar colisiones si los enemigos pueden aparecer
    if (enemiesCanAppear) {
        CheckForCollisions();
    }

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (it->dead == true)
        {
            it->dead = false;
            it = enemies.erase(it);
        }
        else ++it;
    }

    if (areEnemiesDefeated() && !levelCompleted) {
        levelCompleted = true;
    }
}

// ? NUEVA FUNCIÓN: Controlar el sistema de disparos alternados
void Game::UpdateEnemyShooting()
{
    globalShootTimer += GetFrameTime();

    // Solo permitir un disparo cada timeBetweenShots segundos
    if (globalShootTimer >= timeBetweenShots) {

        // Buscar el siguiente enemigo que pueda disparar
        bool shotFired = false;
        int attempts = 0;
        int maxAttempts = enemies.size(); // Evitar bucle infinito

        while (!shotFired && attempts < maxAttempts) {
            // Verificar si hay enemigos
            if (enemies.empty()) break;

            // Asegurar que el índice esté en rango
            if (currentShooterIndex >= enemies.size()) {
                currentShooterIndex = 0;
            }

            // Verificar si este enemigo puede disparar
            if (enemies[currentShooterIndex].CanShoot()) {
                // Crear la bala
                Vector2 shootPos = enemies[currentShooterIndex].GetShootPosition();

                // Para el boss (tipo 4), disparar múltiples balas
                if (enemies[currentShooterIndex].GetType() == 4) {
                    enemy_bullets.push_back(enemy_Bullet(shootPos, -6));
                    enemy_bullets.push_back(enemy_Bullet({ shootPos.x, shootPos.y - 60 }, -6));
                    enemy_bullets.push_back(enemy_Bullet({ shootPos.x, shootPos.y - 120 }, -6));
                    enemy_bullets.push_back(enemy_Bullet({ shootPos.x, shootPos.y - 180 }, -6));
                }
                else {
                    // Enemigos normales: una sola bala
                    enemy_bullets.push_back(enemy_Bullet(shootPos, -6));
                }

                // Marcar que el enemigo disparó
                enemies[currentShooterIndex].SetShot();

                shotFired = true;
                globalShootTimer = 0.0f; // Reiniciar temporizador global
            }

            // Pasar al siguiente enemigo
            currentShooterIndex++;
            if (currentShooterIndex >= enemies.size()) {
                currentShooterIndex = 0;
            }

            attempts++;
        }
    }
}

// NUEVA FUNCIÓN PARA CONTROLAR EL MOVIMIENTO SINCRONIZADO
void Game::UpdateFormation()
{
    // Contar cuántos enemigos están en formación (excluyendo los en movimiento especial)
    int enemiesInFormation = 0;
    for (auto& enemy : enemies) {
        if (enemy.IsInFormation() && !enemy.isExploding &&
            enemy.GetType() != 4 && !enemy.IsInSpecialMovement()) {
            enemiesInFormation++;
        }
    }

    if (enemiesInFormation > 0) {
        formationMoveTimer += GetFrameTime();

        if (formationMoveTimer >= 2.5f) {
            formationMovingRight = !formationMovingRight;
            formationMoveTimer = 0.0f;
        }

        float lateralSpeed = 1.2f;
        float deltaX = 0.0f;

        float leftMost = 896.0f;
        float rightMost = 0.0f;
        float enemyWidth = 32.0f * 4.0f;
        float rightMargin = 40.0f;
        float leftMargin = 20.0f;

        // ? MODIFICAR PARA EXCLUIR ENEMIGOS EN MOVIMIENTO ESPECIAL
        for (auto& enemy : enemies) {
            if (enemy.IsInFormation() && !enemy.isExploding &&
                enemy.GetType() != 4 && !enemy.IsInSpecialMovement()) {
                Vector2 pos = enemy.getPosition();
                if (pos.x < leftMost) leftMost = pos.x;
                if (pos.x + enemyWidth > rightMost) rightMost = pos.x + enemyWidth;
            }
        }

        if (formationMovingRight) {
            if (rightMost < (896 - rightMargin)) {
                deltaX = lateralSpeed;
            }
        }
        else {
            if (leftMost > leftMargin) {
                deltaX = -lateralSpeed;
            }
        }

        // ? APLICAR MOVIMIENTO SOLO A ENEMIGOS NO EN MOVIMIENTO ESPECIAL
        for (auto& enemy : enemies) {
            if (enemy.IsInFormation() && !enemy.isExploding &&
                enemy.GetType() != 4 && !enemy.IsInSpecialMovement()) {
                enemy.MoveInFormation(deltaX, 0.0f);
            }
        }
    }
}
void Game::Draw()
{
    spaceship.Draw();
    for (auto& bullet : spaceship.bullets) {
        bullet.Draw();
    }

    for (auto& enemy_bullet : enemy_bullets) {
        enemy_bullet.Draw();
    }

    if (enemiesCanAppear) {
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

    float spacing = 5.0f;
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

void Game::UpdateSpecialMovements()
{
    specialMovementTimer += GetFrameTime();

    // Verificar si es hora de activar un movimiento especial
    if (specialMovementTimer >= nextSpecialMoveTime) {
        TriggerRandomSpecialMovement();

        // Reiniciar timer y establecer próximo tiempo aleatorio
        specialMovementTimer = 0.0f;
        nextSpecialMoveTime = minTimeBetweenSpecialMoves +
            (float)GetRandomValue(0, (int)((maxTimeBetweenSpecialMoves - minTimeBetweenSpecialMoves) * 10)) / 10.0f;
    }
}

void Game::TriggerRandomSpecialMovement()
{
    // Obtener lista de enemigos que pueden hacer movimiento especial
    std::vector<int> availableEnemies = GetEnemiesInFormation();

    if (availableEnemies.empty()) return;

    // Seleccionar un enemigo aleatorio
    int randomIndex = GetRandomValue(0, availableEnemies.size() - 1);
    int selectedEnemyIndex = availableEnemies[randomIndex];

    // Activar movimiento especial
    enemies[selectedEnemyIndex].StartSpecialMovement();
}

std::vector<int> Game::GetEnemiesInFormation()
{
    std::vector<int> formationEnemies;

    for (int i = 0; i < enemies.size(); i++) {
        // Solo enemigos que están en formación, no el boss, no en movimiento especial, no explotando
        if (enemies[i].IsInFormation() &&
            enemies[i].GetType() != 4 &&
            !enemies[i].IsInSpecialMovement() &&
            !enemies[i].isExploding &&
            enemies[i].life > 0) {

            formationEnemies.push_back(i);
        }
    }

    return formationEnemies;
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

    levelAnnouncementTimer = 0.0f;
    showingLevelAnnouncement = true;
    enemiesCanAppear = false;

    formationMoveTimer = 0.0f;
    formationMovingRight = true;

    globalShootTimer = 0.0f;
    currentShooterIndex = 0;

    specialMovementTimer = 0.0f;
    nextSpecialMoveTime = GetRandomValue((int)minTimeBetweenSpecialMoves, (int)maxTimeBetweenSpecialMoves);

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

bool Game::IsShowingLevelAnnouncement() const {
    return showingLevelAnnouncement;
}

void Game::SetShowingLevelAnnouncement(bool showing) {
    showingLevelAnnouncement = showing;
    if (showing) {
        levelAnnouncementTimer = 0.0f;
        enemiesCanAppear = false;
    }
    else {
        enemiesCanAppear = true;
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

                PlaySound(snd_shipExplosion);
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

            if (it->life > 0 && CheckCollisionRecs(enemyRect, bulletRect))
            {
                it->life--;
                if (it->life < 1)
                {
                    switch (it->type) {
                    case 1:
                        score += 40;
                        break;
                    case 2:
                        score += 20;
                        break;
                    case 3:
                        score += 50;
                        break;
                    case 5:
                        score += 30;
                        break;
                    case 4:
                        score += 1000;
                        break;
                    }

                    if (it->type != 4) {
                        PlaySound(snd_explosion_red);
                        it->isExploding = true;
                        it->explosionPosition = {
                            enemyRect.x + 20 + enemyRect.width / 2,
                            enemyRect.y + 20 + enemyRect.height / 2
                        };
                    }
                    else {
                        PlaySound(snd_shipExplosion);
                        it->isExploding = true;
                        it->explosionPosition = {
                            enemyRect.x + 20 + enemyRect.width / 2,
                            enemyRect.y + 20 + enemyRect.height / 2
                        };
                    }
                }
                bullet.active = false;
                break;
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

    int screenWidth = 896;

    float spacingX = 75.0f; 
    float spacingY = 58.0f; 

    int columns = 10;
    int rows = 5;

    float totalFormationWidth = (columns - 1) * spacingX;

    float startX = (screenWidth - totalFormationWidth) / 2;
    float startY = 120.0f;

    for (int i = 3; i < 7; i++) { 
        float x = startX + i * spacingX;
        Vector2 startPos = { (i < 5) ? -100.0f : (float)screenWidth + 100.0f, startY };

        Enemy enemy(3, startPos, 0, i < 5);
        enemy.finalPosition = { x, startY };
        enemies.push_back(enemy);
    }

    for (int i = 2; i < 8; i++) { 
        float x = startX + i * spacingX;
        Vector2 startPos = { (i < 5) ? -100.0f : (float)screenWidth + 100.0f, startY + spacingY };

        Enemy enemy(5, startPos, 0, i < 5);
        enemy.finalPosition = { x, startY + spacingY };
        enemies.push_back(enemy);
    }

    for (int i = 1; i < 9; i++) {
        float x = startX + i * spacingX;
        Vector2 startPos = { (i < 5) ? -100.0f : (float)screenWidth + 100.0f, startY + 2 * spacingY };

        Enemy enemy(2, startPos, 0, i < 5);
        enemy.finalPosition = { x, startY + 2 * spacingY };
        enemies.push_back(enemy);
    }

    for (int i = 1; i < 9; i++) {
        float x = startX + i * spacingX;
        Vector2 startPos = { (i < 5) ? -100.0f : (float)screenWidth + 100.0f, startY + 3 * spacingY };

        Enemy enemy(2, startPos, 0, i < 5);
        enemy.finalPosition = { x, startY + 3 * spacingY };
        enemies.push_back(enemy);
    }

    for (int i = 0; i < 10; i++) {
        float x = startX + i * spacingX;
        Vector2 startPos = { (i < 5) ? -100.0f : (float)screenWidth + 100.0f, startY + 4 * spacingY };

        Enemy enemy(1, startPos, 0, i < 5);
        enemy.finalPosition = { x, startY + 4 * spacingY };
        enemies.push_back(enemy);
    }

    for (int i = 0; i < 10; i++) {
        float x = startX + i * spacingX;
        Vector2 startPos = { (i < 5) ? -100.0f : (float)screenWidth + 100.0f, startY + 5 * spacingY };

        Enemy enemy(1, startPos, 0, i < 5);
        enemy.finalPosition = { x, startY + 5 * spacingY };
        enemies.push_back(enemy);
    }

    return enemies;
}

std::vector<Enemy> Game::createBoss()
{
    std::vector<Enemy> enemies;
    int screenWidth = 1024;
    int centerX = screenWidth / 2;
    float y1 = 200;
    float leftGroupCenter = centerX - 128;

    enemies.push_back(Enemy(4, { leftGroupCenter , y1 }, 1, true));

    return enemies;
}

bool Game::areEnemiesDefeated() {
    return enemies.empty();
}

void Game::nextLevel() {
    currentLevel++;

    levelAnnouncementTimer = 0.0f;
    showingLevelAnnouncement = true;
    enemiesCanAppear = false;

    formationMoveTimer = 0.0f;
    formationMovingRight = true;

    globalShootTimer = 0.0f;
    currentShooterIndex = 0;

    specialMovementTimer = 0.0f;
    nextSpecialMoveTime = GetRandomValue((int)minTimeBetweenSpecialMoves, (int)maxTimeBetweenSpecialMoves);

    if (currentLevel == 1) {
        enemies = createEnemy();
    }
    else if (currentLevel == 2) {
        enemies = createEnemy();
    }
    else if (currentLevel == 3) {
        enemies = createBoss();
        levelAnnouncementTimer = 1.5f; 
    }
    else {
        enemies.clear();
    }

    levelCompleted = false;
    for (auto& bullet : enemy_bullets) {
        bullet.active = false;
    }
    for (auto& bullet : spaceship.bullets) {
        bullet.active = false;
    }
}

int Game::getCurrentLevel() {
    return currentLevel;
}

bool Game::isLevelCompleted() {
    return levelCompleted;
}