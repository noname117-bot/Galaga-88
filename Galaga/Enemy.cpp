#include "Enemy.hpp"
#include "raylib.h"
#include <cmath>

Enemy::Enemy(int type, Vector2 position)
{

	this->type = type;
	this->position = position;

	switch (type)
	{
	case 1:
		image = LoadTexture("resources/enemies/enemy2_9.png");
		break;
	case 2:
		image = LoadTexture("resources/enemies/enemy2_17.png");
		break;
	}

	entering = true;  // El enemigo empieza entrando desde los bordes
	angle = 0.0f;     // Ángulo inicial
	radius = 150.0f;  // Radio del círculo
	speed = 1.0f;
}

void Enemy::Update()
{
    if (entering) {
        // Movimiento de entrada desde los bordes
        if (position.x < 512) { // Si el enemigo aún no está en el centro
            position.x += speed * 2;  // Aumentamos la velocidad para la entrada
        }
        else {
            entering = false;  // Una vez que está en el centro, comienza el movimiento circular
            angle = 0;  // Comienza el ángulo para el movimiento circular
        }
    }

    if (!entering) {
        // Movimiento circular
        angle += speed * 0.1f; // Ajusta la velocidad del giro, lo lento o rápido que se mueve
        position.x = 512 + cos(angle) * radius; // Movimiento en el eje X
        position.y = 400 + sin(angle) * radius; // Movimiento en el eje Y

        // Si el ángulo supera 360 grados (2 PI), comienza a colocarse en la posición final
        if (angle > 6.28f) {  // 2 PI = 360 grados
            target = position; // Actualiza la posición final
            position = target; // Detiene al enemigo en esa posición final
        }
    }
}

void Enemy::Draw()
{

	DrawTextureEx(image, position, 0.0f, 4.0, WHITE);


}

int Enemy::GetType()
{
	return type;
}