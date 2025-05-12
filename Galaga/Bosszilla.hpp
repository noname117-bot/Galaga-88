#pragma once
#include <raylib.h>
#include <vector>
#include <cmath>
#include "enemy_bullet.hpp"

class Bosszilla {
public:
    Bosszilla(float x, float y);
    ~Bosszilla();

    void Update(float deltaTime);
    void Draw();
    bool IsActive() const;
    Rectangle GetCollisionRect() const;
    void Hit();
    void Fire(std::vector<enemy_Bullet>& bullets);

private:
    // Texturas y sprites
    Texture2D sprite;

    // Posición y movimiento
    Vector2 position;
    float radius;          // Radio del movimiento circular
    float angle;           // Ángulo actual en el movimiento circular
    Vector2 centerPoint;   // Punto central del movimiento circular
    float rotationSpeed;   // Velocidad de rotación

    // Balanceo vertical
    float oscillationAmplitude; // Amplitud del balanceo
    float oscillationSpeed;     // Velocidad del balanceo
    float oscillationTime;      // Tiempo acumulado para el balanceo

    // Estado y propiedades del jefe
    int health;
    bool active;

    // Disparos
    float fireTimer;
    float fireRate;

    // Animación
    Rectangle sourceRect;
    int currentFrame;
    float frameCounter;
    float frameTime;
    int frameCount;

    // Efectos visuales
    bool isHit;
    float hitEffectTimer;
};