#pragma once
#include <raylib.h>


enum {
	ENTERING = 0,
	INFINITI = 1,
	EXITING = 2,
	FORMATION = 3
};

enum {
	RED_PATH = 0,
	ORANGE_PATH = 1
};

class Enemy {
public:
    Enemy(int type, Vector2 position, int pathType, bool startFromLeft);

    void Update();
    void Draw();
    int GetType();
    Rectangle getRect();

    // Variables públicas
    int type;
    void Activate(); // Activar el enemigo
    bool IsActive(); // Verificar si está activo
    bool IsInFormation();

private:
    Vector2 position;
    Vector2 finalPosition;
    int pathType;
    bool startFromLeft;
    int currentPhase;
    float infinityProgress;
    float moveSpeed;
    Texture2D image;
    bool active;
    // Para el movimiento en formación
    bool movingRight;
    float formationMoveTimer;
};
