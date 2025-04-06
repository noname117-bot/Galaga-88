#pragma once
#include <raylib.h>

class Enemy {
public:
	Enemy(int type, Vector2 position);
	void Update();
	void Draw();
	int GetType();
	Texture2D image;
	int type;
	Vector2 position;

	bool entering;  // Indica si el enemigo está entrando desde los bordes
	float angle;  // Ángulo para el movimiento circular
	float radius;  // Radio del círculo para movimiento
	float speed;   // Velocidad de movimiento
	Vector2 target;  // Posición final después del círculo
};