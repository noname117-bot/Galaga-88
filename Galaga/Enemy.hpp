#pragma once
#include "raylib.h"

class Enemy
{
private:
    Texture2D image;
    Texture2D image2; // Para animación del boss
    Vector2 position;
    int pathType;
    bool startFromLeft;
    bool active;

    // Variables de explosión
    Texture2D explosion_spriteSheet;
    int explosionFrame;
    int explosionFrameCount; // NUEVA VARIABLE: número de frames en el spritesheet
    float explosionFrameTime;
    float explosionTimer;

    // Variables de animación
    int currentFrame;
    int frameCounter;
    bool animation;

    // Variables de movimiento
    int currentPhase;
    float infinityProgress;
    float moveSpeed;
    bool movingRight;
    int formationMoveTimer;
    int formationPhase;
    float formationProgress;
    int formationPosition;
    Vector2 formationOffset;

    // Variables de disparo
    bool canShoot = true;
    float shootTimer = 0.0f;
    float shootCooldown = 2.0f; // 2 segundos entre disparos
    double lastShotTime = 0.0;

    bool isInSpecialMovement;           // Si está haciendo movimiento especial
    int specialMovementPhase;           // 0=patrón_inicial, 1=bajando, 2=subiendo, 3=regresando
    int specialMovementPattern;         // 0=circular, 1=forma_S, 2=zigzag (futuro)
    float specialMovementTimer;         // Timer para controlar el movimiento
    Vector2 originalFormationPosition;  // Posición original en formación
    Vector2 patternStartPosition;       // Posición donde inicia el patrón
    Vector2 circleCenter;              // Centro del círculo para el movimiento
    float circleRadius;                // Radio del círculo
    float circleAngle;                 // Ángulo actual en el círculo
    bool movingClockwise;              // Dirección del movimiento circular
    float specialMoveSpeed;
  

public:
    Enemy(int type, Vector2 position, int pathType = 0, bool startFromLeft = true);

    void Activate();
    bool IsActive();
    bool IsInFormation();
    void CalculateFormationOffset();
    void Update();
    void Draw();
    void MoveInFormation(float deltaX, float deltaY);

    void StartSpecialMovement();       // Iniciar movimiento especial
    bool IsInSpecialMovement() const;  // Verificar si está en movimiento especial
    void UpdateSpecialMovement();      // Actualizar lógica del movimiento especial
    Vector2 GetOriginalPosition() const; // Obtener posición original
    void SetOriginalPosition(Vector2 pos);


    bool CanShoot();
    void SetShot();
    Vector2 GetShootPosition();

    int GetType();
    int type;

    bool dead;
    int life;
    bool isExploding;

    float sPatternProgress;            // Progreso en el patrón S (0.0 a 1.0)
    float sPatternAmplitude;           // Amplitud de la S
    float sPatternFrequency;

    Rectangle getRect();
    Vector2 getPosition() { return position; }
    int GetLife() { return life; }
    bool IsDead() { return dead; }
    bool IsExploding() { return isExploding; }

    Vector2 finalPosition;
    Vector2 explosionPosition;

    void TakeDamage(int damage = 1) {
        life -= damage;
        if (life <= 0) {
            isExploding = true;
            explosionPosition = position;
            explosionFrame = 0;
            explosionTimer = 0.0f;
        }
    }
};