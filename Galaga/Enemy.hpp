#pragma once
#include "raylib.h"

class Enemy
{
private:
    Texture2D image;
    Texture2D image2; // Para animaci�n del boss
    Vector2 position;
    int pathType;
    bool startFromLeft;
    bool active;

    // Variables de explosi�n
    Texture2D explosion_spriteSheet;
    int explosionFrame;
    int explosionFrameCount; // NUEVA VARIABLE: n�mero de frames en el spritesheet
    float explosionFrameTime;
    float explosionTimer;

    // Variables de animaci�n
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

    bool isInSpecialMovement;           // Si est� haciendo movimiento especial
    int specialMovementPhase;           // 0=patr�n_inicial, 1=bajando, 2=subiendo, 3=regresando
    int specialMovementPattern;         // 0=circular, 1=forma_S, 2=zigzag (futuro)
    float specialMovementTimer;         // Timer para controlar el movimiento
    Vector2 originalFormationPosition;  // Posici�n original en formaci�n
    Vector2 patternStartPosition;       // Posici�n donde inicia el patr�n
    Vector2 circleCenter;              // Centro del c�rculo para el movimiento
    float circleRadius;                // Radio del c�rculo
    float circleAngle;                 // �ngulo actual en el c�rculo
    bool movingClockwise;              // Direcci�n del movimiento circular
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
    bool IsInSpecialMovement() const;  // Verificar si est� en movimiento especial
    void UpdateSpecialMovement();      // Actualizar l�gica del movimiento especial
    Vector2 GetOriginalPosition() const; // Obtener posici�n original
    void SetOriginalPosition(Vector2 pos);


    bool CanShoot();
    void SetShot();
    Vector2 GetShootPosition();

    int GetType();
    int type;

    bool dead;
    int life;
    bool isExploding;

    float sPatternProgress;            // Progreso en el patr�n S (0.0 a 1.0)
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