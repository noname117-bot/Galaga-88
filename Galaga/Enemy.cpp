#include "Enemy.hpp"
#include "raylib.h"
#include <cmath>



Enemy::Enemy(int type, Vector2 position, int pathType, bool startFromLeft) : type(type), position(position), pathType(pathType), startFromLeft(startFromLeft)
{

	this->type = type;
	this->position = position;
    this->pathType = pathType;
    this->startFromLeft = startFromLeft;

	switch (type)
	{
	case 1:
		image = LoadTexture("resources/enemies/enemy2_9.png");
		break;
	case 2:
		image = LoadTexture("resources/enemies/enemy2_17.png");
		break;
    case 3:
        image = LoadTexture("resources/enemies/bee1.png");
	}


    currentPhase = 0;
    active = false;

    infinityProgress = 0.0f;
    moveSpeed = 1.0f;   //1.0f;
    movingRight = true; // Para el movimiento lateral en formación
    formationMoveTimer = 0; // Temporizador para cambiar de dirección

    formationPhase = 0;
    formationProgress = 0.0f;
    formationPosition = 0;

    float centerX = 512.0f;

    if (startFromLeft) {
        if (pathType == 0) { 
            finalPosition = { centerX - 100.0f, 200.0f };
        }
        else { // ORANGE_PATH
            finalPosition = { centerX - 50.0f, 200.0f + 55.0f };
        }
    }
    else {
        if (pathType == 0) { // RED_PATH
            finalPosition = { centerX + 50.0f, 200.0f };
        }
        else { // ORANGE_PATH
            finalPosition = { centerX + 100.0f, 200.0f + 55.0f };
        }
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
    return currentPhase == 3; 
}


void Enemy::CalculateFormationOffset()
{
    float horizontalSpacing = 100.0f;

    if (formationPosition < 4) {
        formationOffset.x = formationPosition * horizontalSpacing;
        formationOffset.y = 0.0f; 
    }
    else {
        formationOffset.x = (formationPosition - 4) * horizontalSpacing;
        formationOffset.y = 70.0f; // Bottom row offset
    }



}

void Enemy::Update()
{

    if (!active) return;
    switch (currentPhase)
    {
    case 0:
    {
        if (startFromLeft) {
            position.x += moveSpeed * 2;
            if (position.x >= 200) {
                currentPhase = 1;
            }
        }
        else {
            position.x -= moveSpeed * 2;
            if (position.x <= 800) {
                currentPhase = 1; 
            }
        }
        break;
    }

    case 1: 
    {
        infinityProgress += 0.01f * moveSpeed;

        if (infinityProgress >= 2.0f * PI) {
            currentPhase = 2; 
            break;
        }

        float centerX = 512.0f;
        float centerY = 400.0f;
        float widthFactor = 250.0f;
        float heightFactor = 150.0f;
        float denominator = 1.0f + pow(sin(infinityProgress), 2);
        float x = centerX + widthFactor * cos(infinityProgress) / denominator;
        float y = centerY + heightFactor * sin(infinityProgress) * cos(infinityProgress) / denominator;

        if (pathType == 1) {
            x = 2 * centerX - x;
        }

        position.x = x;
        position.y = y;
        break;
    }

    case 2: 
    {
        Vector2 direction = { finalPosition.x - position.x, finalPosition.y - position.y};

        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < 5.0f) {
            position = finalPosition;
            currentPhase = 3; 
            break;
        }

        direction.x /= distance;
        direction.y /= distance;
        position.x += direction.x * moveSpeed * 2;
        position.y += direction.y * moveSpeed * 2;
        break;
    }

    case 3:
    {
        formationMoveTimer += GetFrameTime();

        Vector2 basePos = {
            finalPosition.x + formationOffset.x,
            finalPosition.y + formationOffset.y
        };

        // Cambiar dirección cada segundo
        if (formationMoveTimer >= 1.0f) {
            movingRight = !movingRight;
            formationMoveTimer = 0;
        }

        // Movimiento lateral más amplio
        float lateralMovement = 1.2f; // antes era 0.5f
        if (movingRight) {
            position.x += lateralMovement;
        }
        else {
            position.x -= lateralMovement;
        }

        // Quitamos el efecto de hover
        position.y = basePos.y;

        break;
    }
    }
}

void Enemy::Draw()
{
    Color debugColor;

	DrawTextureEx(image, position, 0.0f, 4.0, WHITE);

    switch (currentPhase) {
    case 0: // ENTERING
        debugColor = RED;
        break;
    case 1: // INFINITI
        debugColor = YELLOW;
        break;
    case 2: // EXITING
        debugColor = GREEN;
        break;
    case 3: // FORMATION
        debugColor = BLUE;
        break;
    }

    DrawCircle(position.x + image.width * 2, position.y + image.height * 2, 5, debugColor);

}

int Enemy::GetType()
{
	return type;
}

Rectangle Enemy::getRect()
{
    return { position.x, position.y,float(image.width*4.0f), float(image.height*4.0f) };
}
