#include "bosszilla.hpp"
#include <cmath>
#include <iostream>

Bosszilla::Bosszilla(float x, float y)
{
    // Cargar textura del boss
    sprite = LoadTexture("resources/bosszilla.png");  // Asegúrate de que exista esta textura

    // Inicializar posición
    position = { x, y };
    centerPoint = { GetScreenWidth() / 2.0f, 200.0f };  // Centro del patrón circular
    radius = 200.0f;                                    // Radio del movimiento circular
    angle = 0.0f;
    rotationSpeed = 0.5f;                               // Velocidad de rotación

    // Parámetros de balanceo vertical
    oscillationAmplitude = 50.0f;                       // Amplitud del balanceo
    oscillationSpeed = 2.0f;                            // Velocidad del balanceo
    oscillationTime = 0.0f;

    // Estado inicial
    health = 30;                                        // Vida del boss
    active = true;

    // Configuración de disparos
    fireTimer = 0.0f;
    fireRate = 1.0f;                                    // Disparos por segundo

    // Configuración de animación
    sourceRect = { 0, 0, static_cast<float>(sprite.width), static_cast<float>(sprite.height) };
    currentFrame = 0;
    frameCounter = 0.0f;
    frameTime = 0.1f;                                   // Tiempo entre frames
    frameCount = 2;                                     // Número de frames de animación

    // Efectos visuales
    isHit = false;
    hitEffectTimer = 0.0f;
}

Bosszilla::~Bosszilla()
{
    UnloadTexture(sprite);
}

void Bosszilla::Update(float deltaTime)
{
    if (!active) return;

    // Actualizar el ángulo para el movimiento circular
    angle += rotationSpeed * deltaTime;

    // Calcular la nueva posición basada en el movimiento circular
    float x = centerPoint.x + radius * cos(angle);
    float y = centerPoint.y + radius * sin(angle) * 0.5f;  // Aplana el círculo en el eje Y

    // Aplicar el balanceo vertical
    oscillationTime += deltaTime;
    float verticalOffset = oscillationAmplitude * sin(oscillationTime * oscillationSpeed);

    // Actualizar posición final
    position.x = x;
    position.y = y + verticalOffset;

    // Actualizar tiempo de disparo
    fireTimer += deltaTime;

    // Actualizar animación
    frameCounter += deltaTime;
    if (frameCounter >= frameTime)
    {
        frameCounter = 0.0f;
        currentFrame = (currentFrame + 1) % frameCount;
        sourceRect.x = currentFrame * (sprite.width / frameCount);
    }

    // Actualizar efecto de hit
    if (isHit)
    {
        hitEffectTimer += deltaTime;
        if (hitEffectTimer >= 0.1f)  // Duración del efecto de hit
        {
            isHit = false;
            hitEffectTimer = 0.0f;
        }
    }
}

void Bosszilla::Draw()
{
    if (!active) return;

    // Determinar el color basado en si está siendo golpeado
    Color drawColor = isHit ? RED : WHITE;

    // Dibujar el boss
    DrawTextureRec(sprite, sourceRect, position, drawColor);

    // Dibujar barra de salud (opcional)
    float healthBarWidth = 100.0f;
    float healthPercentage = static_cast<float>(health) / 30.0f;  // 30 es la salud inicial
    DrawRectangle(position.x, position.y - 20, healthBarWidth, 10, GRAY);
    DrawRectangle(position.x, position.y - 20, healthBarWidth * healthPercentage, 10, RED);
}

bool Bosszilla::IsActive() const
{
    return active;
}

Rectangle Bosszilla::GetCollisionRect() const
{
    return Rectangle{
        position.x + (sprite.width / frameCount) * 0.2f,
        position.y + sprite.height * 0.2f,
        (sprite.width / frameCount) * 0.6f,
        sprite.height * 0.6f
    };
}

void Bosszilla::Hit()
{
    if (!active) return;

    health--;
    isHit = true;
    hitEffectTimer = 0.0f;

    if (health <= 0)
    {
        active = false;
    }
}

void Bosszilla::Fire(std::vector<enemy_Bullet>& bullets)
{
    if (!active || fireTimer < 1.0f / fireRate) return;

    // Reiniciar el temporizador
    fireTimer = 0.0f;

    // Crear patrón de disparo
    // Disparo central
    Vector2 bulletPos = {
        position.x + (sprite.width / frameCount) * 0.5f,
        position.y + sprite.height
    };
    bullets.push_back(enemy_Bullet(bulletPos, -10)); // Velocidad negativa para que vaya hacia abajo

    // Disparos adicionales (en patrón)
    float spreadAngle = 20.0f;  // Ángulo entre disparos
    for (int i = -2; i <= 2; i++)
    {
        if (i == 0) continue;  // Saltamos el central que ya disparamos

        float angle = i * spreadAngle * (PI / 180.0f);  // Convertir a radianes
        Vector2 bulletPosSpread = {
            bulletPos.x + 20.0f * i,
            bulletPos.y + 10.0f * abs(i)
        };
        bullets.push_back(enemy_Bullet(bulletPosSpread, -10));
    }
}