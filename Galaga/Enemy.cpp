#include "Enemy.hpp"
#include "raylib.h"
#include <cmath>

Enemy::Enemy(int type, Vector2 position, int pathType, bool startFromLeft) : type(type), position(position), pathType(pathType), startFromLeft(startFromLeft)
{
    this->type = type;
    this->position = position;
    this->pathType = pathType;
    this->startFromLeft = startFromLeft;

    active = false;

    isExploding = false;
    explosionFrame = 0;
    explosionFrameTime = 0.0f;
    explosionTimer = 0.0f;
    explosionPosition = { 0.0f, 0.0f };
    dead = false;
    explosionFrameCount = 6; 

    currentFrame = 0;
    frameCounter = 0;
    animation = false;

    isInSpecialMovement = false;
    specialMovementPhase = 0;
    specialMovementTimer = 0.0f;
    originalFormationPosition = { 0, 0 };
    circleCenter = { 0, 0 };
    circleRadius = 80.0f;  // Radio del c�rculo
    circleAngle = 0.0f;
    movingClockwise = true;
    specialMoveSpeed = 2.5f;

    switch (type)
    {
    case 1: 
        image = LoadTexture("resources/enemies/enemy2_9.png");
        explosion_spriteSheet = LoadTexture("resources/explosions/explosion_1.png");
        life = 1;
        explosionFrameTime = 0.08f;
        break;
    case 2: 
        image = LoadTexture("resources/enemies/enemy3_7.png");
        explosion_spriteSheet = LoadTexture("resources/explosions/explosion_2.png");
        life = 1;
        explosionFrameTime = 0.10f; 
        break;
    case 3: 
        image = LoadTexture("resources/enemies/bee1.png");
        explosion_spriteSheet = LoadTexture("resources/explosions/explosion_1.png");
        life = 1;
        explosionFrameTime = 0.12f; 
        break;
    case 5: 
        image = LoadTexture("resources/enemies/enemy5_20.png");
        explosion_spriteSheet = LoadTexture("resources/explosions/explosion_2.png");
        life = 1;
        explosionFrameTime = 0.09f; 
        break;
    case 4: 
        image = LoadTexture("resources/Boss/Boss1_1.png");
        image2 = LoadTexture("resources/Boss/Boss1_2.png");
        explosion_spriteSheet = LoadTexture("resources/explosion_starhip.png");
        active = true;
        life = 30;
        explosionFrameTime = 0.1f;
        explosionFrameCount = 8; 
        animation = true;
        break;
    }

    currentPhase = 0;
    infinityProgress = 0.0f;
    moveSpeed = 1.0f;
    movingRight = true;
    formationMoveTimer = 0;
    formationPhase = 0;
    formationProgress = 0.0f;
    formationPosition = 0;

    if (type != 4) {
        finalPosition = position;
    }
}

void Enemy::Activate()
{
    active = true;
}

bool Enemy::IsActive()
{
    return active;
}

bool Enemy::IsInFormation()
{
    return currentPhase == 1;
}

void Enemy::CalculateFormationOffset()
{
    formationOffset.x = 0.0f;
    formationOffset.y = 0.0f;
}

bool Enemy::CanShoot()
{
    if (!active || isExploding || life <= 0) return false;

    if (currentPhase != 1 && type != 4) return false;

    return canShoot;
}

void Enemy::SetShot()
{
    canShoot = false;
    shootTimer = 0.0f;
    lastShotTime = GetTime();
}

Vector2 Enemy::GetShootPosition()
{
    Vector2 shootPos;
    shootPos.x = position.x + (image.width * 4.0f * 0.5f); 
    shootPos.y = position.y + (image.height * 4.0f); 
    return shootPos;
}

void Enemy::StartSpecialMovement()
{
    if (type == 4 || !IsInFormation()) return; // El boss no hace movimientos especiales

    isInSpecialMovement = true;
    specialMovementPhase = 0; // Fase circular
    specialMovementTimer = 0.0f;

    // Guardar posici�n actual como posici�n original
    originalFormationPosition = position;

    // Configurar centro del c�rculo (un poco hacia adelante de la posici�n actual)
    circleCenter.x = position.x + (image.width * 4.0f * 0.5f);
    circleCenter.y = position.y + 60.0f;

    // �ngulo inicial basado en la posici�n actual
    circleAngle = 0.0f;

    // Direcci�n aleatoria del c�rculo
    movingClockwise = (GetRandomValue(0, 1) == 1);
}

bool Enemy::IsInSpecialMovement() const
{
    return isInSpecialMovement;
}

Vector2 Enemy::GetOriginalPosition() const
{
    return originalFormationPosition;
}

void Enemy::SetOriginalPosition(Vector2 pos)
{
    originalFormationPosition = pos;
}

void Enemy::UpdateSpecialMovement()
{
    if (!isInSpecialMovement) return;

    specialMovementTimer += GetFrameTime();

    switch (specialMovementPhase)
    {
    case 0: // Fase circular
    {
        // Incrementar �ngulo
        float angleSpeed = 3.0f; // Velocidad angular
        if (movingClockwise) {
            circleAngle += angleSpeed * GetFrameTime();
        }
        else {
            circleAngle -= angleSpeed * GetFrameTime();
        }

        // Calcular nueva posici�n en el c�rculo
        position.x = circleCenter.x + cos(circleAngle) * circleRadius;
        position.y = circleCenter.y + sin(circleAngle) * circleRadius;

        // Despu�s de medio c�rculo, cambiar a fase de bajada
        if (specialMovementTimer >= 1.5f) {
            specialMovementPhase = 1;
            specialMovementTimer = 0.0f;
        }
        break;
    }

    case 1: // Bajando por la pantalla
    {
        position.y += specialMoveSpeed * 60.0f * GetFrameTime(); // Bajar r�pido

        // Si sale de la pantalla por abajo
        if (position.y > 800.0f) {
            specialMovementPhase = 2;
            specialMovementTimer = 0.0f;
            // Reaparecer arriba
            position.y = -100.0f;
        }
        break;
    }

    case 2: // Subiendo/regresando
    {
        // Moverse hacia la posici�n original
        Vector2 direction = {
            originalFormationPosition.x - position.x,
            originalFormationPosition.y - position.y
        };

        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < 10.0f) {
            // Ha llegado a su posici�n original
            position = originalFormationPosition;
            isInSpecialMovement = false;
            specialMovementPhase = 0;
            currentPhase = 1; // Volver a formaci�n
        }
        else {
            // Normalizar direcci�n y mover
            direction.x /= distance;
            direction.y /= distance;

            position.x += direction.x * specialMoveSpeed * 60.0f * GetFrameTime();
            position.y += direction.y * specialMoveSpeed * 60.0f * GetFrameTime();
        }
        break;
    }
    }
}

void Enemy::Update()
{

    shootTimer += GetFrameTime();
    if (shootTimer >= shootCooldown) {
        canShoot = true;
    }

    if (isExploding) {
        explosionTimer += GetFrameTime();
        if (explosionTimer >= explosionFrameTime) {
            explosionTimer = 0.0f;
            explosionFrame++;

            if (explosionFrame >= explosionFrameCount) {
                explosionFrame = 0;
                isExploding = false;
                dead = true;
            }
        }
    }
    else
    {
        if (isInSpecialMovement) {
            UpdateSpecialMovement();
            return; // No procesar movimiento normal
        }

        if (type == 4) { 
            active = true;
            currentPhase = 1;
            infinityProgress += 0.01f * moveSpeed;

            if (infinityProgress >= 2.0f * PI) {
                infinityProgress = 0.0f;
            }

            float centerX = 512.0f;
            float centerY = 200.0f;
            float widthFactor = 250.0f;
            float heightFactor = 150.0f;
            float denominator = 1.0f + pow(sin(infinityProgress), 2);
            float x = centerX + widthFactor * cos(infinityProgress) / denominator;
            float y = centerY + heightFactor * sin(infinityProgress) * cos(infinityProgress) / denominator;

            position.x = x - 256;
            position.y = y;
        }
        else { 
            if (!active) return;

            switch (currentPhase)
            {
            case 0:
            {
                Vector2 direction = { finalPosition.x - position.x, finalPosition.y - position.y };
                float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

                if (distance < 5.0f) {
                    position = finalPosition;
                    currentPhase = 1; 

                    originalFormationPosition = position;

                    break;
                }

                direction.x /= distance;
                direction.y /= distance;
                position.x += direction.x * moveSpeed * 3;
                position.y += direction.y * moveSpeed * 3;
                break;
            }

            case 1:
            {
                break;
            }
            }
        }
    }
}

void Enemy::MoveInFormation(float deltaX, float deltaY)
{
    if (currentPhase == 1 && !isExploding && !isInSpecialMovement) {
        position.x += deltaX;
        position.y += deltaY;
        originalFormationPosition.x += deltaX;
        originalFormationPosition.y += deltaY;
    }
}
void Enemy::Draw()
{
    if (isExploding) {
        int frameWidth = explosion_spriteSheet.width / explosionFrameCount;
        int frameHeight = explosion_spriteSheet.height;

        Rectangle sourceRect = { frameWidth * explosionFrame, 0, frameWidth, frameHeight };
        Rectangle destRect = { explosionPosition.x, explosionPosition.y, frameWidth * 4.0f, frameHeight * 4.0f };

        Vector2 origin = { frameWidth * 2.0f, frameHeight * 2.0f };

        DrawTexturePro(explosion_spriteSheet, sourceRect, destRect, origin, 0.0f, WHITE);
    }
    else
    {
        if (life > 0) {
            if (animation == false) {
                DrawTextureEx(image, position, 0.0f, 4.0, WHITE);
            }
            else
            {
                frameCounter++;
                if (frameCounter > 9) {
                    frameCounter = 0;
                    currentFrame++;
                    if (currentFrame > 1) {
                        currentFrame = 0;
                    }
                }
                if (currentFrame == 0)
                    DrawTextureEx(image2, position, 0.0f, 4.0, WHITE);
                else
                    DrawTextureEx(image, position, 0.0f, 4.0, WHITE);
            }
        }
    }
}

int Enemy::GetType()
{
    return type;
}

Rectangle Enemy::getRect()
{
    return { position.x, position.y, float(image.width * 4.0f), float(image.height * 4.0f) };
}