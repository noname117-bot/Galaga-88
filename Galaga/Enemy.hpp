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

	bool entering;  // Indica si el enemigo est� entrando desde los bordes
	float angle;  // �ngulo para el movimiento circular
	float radius;  // Radio del c�rculo para movimiento
	float speed;   // Velocidad de movimiento
	Vector2 target;  // Posici�n final despu�s del c�rculo
};