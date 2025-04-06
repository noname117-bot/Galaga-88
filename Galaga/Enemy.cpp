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
	angle = 0.0f;     // �ngulo inicial
	radius = 150.0f;  // Radio del c�rculo
	speed = 1.0f;
}

void Enemy::Update()
{
    if (entering) {
        // Movimiento de entrada desde los bordes
        if (position.x < 512) { // Si el enemigo a�n no est� en el centro
            position.x += speed * 2;  // Aumentamos la velocidad para la entrada
        }
        else {
            entering = false;  // Una vez que est� en el centro, comienza el movimiento circular
            angle = 0;  // Comienza el �ngulo para el movimiento circular
        }
    }

    if (!entering) {
        // Movimiento circular
        angle += speed * 0.1f; // Ajusta la velocidad del giro, lo lento o r�pido que se mueve
        position.x = 512 + cos(angle) * radius; // Movimiento en el eje X
        position.y = 400 + sin(angle) * radius; // Movimiento en el eje Y

        // Si el �ngulo supera 360 grados (2 PI), comienza a colocarse en la posici�n final
        if (angle > 6.28f) {  // 2 PI = 360 grados
            target = position; // Actualiza la posici�n final
            position = target; // Detiene al enemigo en esa posici�n final
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