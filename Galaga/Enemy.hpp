#pragma once
#include <raylib.h>
//#include "enemy_bullet.hpp"
#include <vector>



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
    Vector2 getPosition() const {  return position; }
	int life; // Vida del enemigo
    void CalculateFormationOffset();
    Texture2D explosion_spriteSheet;
    bool isExploding;          // Indica si la explosión está activa
    int explosionFrame;        // Fotograma actual de la explosión
    float explosionFrameTime;  // Tiempo entre fotogramas
    float explosionTimer;      // Temporizador para cambiar de fotograma
    Vector2 explosionPosition; // Posición de la explosión
    bool dead; 

private:
    Vector2 position;
    Vector2 finalPosition;
    int pathType;
    bool startFromLeft;
    int currentPhase;
    float infinityProgress;
    float moveSpeed;
    Texture2D image;
    Texture2D image2;
    bool active;

    float formationPhase;
    float formationProgress;
    float formationPosition;
    Vector2 formationOffset;

    bool movingRight;
    float formationMoveTimer;


    int currentFrame;
    int frameCounter;
    bool animation;

};
