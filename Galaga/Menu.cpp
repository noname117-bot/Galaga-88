#include "Menu.hpp"

Menu::Menu()
{
    spriteSheet = LoadTexture("resources/electionsingle-sheet.png");

    currentFrame = 0;
    frameTime = 0.15f; // Velocidad de animación (ajustable)
    frameCounter = 0;
    selected = false;

    animationTime = 0; // Tiempo para la animación
    maxTime = 10.0f;

    totalCycles = 5; // Número de ciclos de animación (por ejemplo, dos vueltas completas)
    cyclesCompleted = 0;

    // Configurar el primer frame
    sourceRect = { 0, 0, 256, 232 };
}

Menu::~Menu()
{
    UnloadTexture(spriteSheet);
}

void Menu::Update()
{

    if (!selected)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            animationTime = 0;
            selected = true;
        }
    }
    else
    {

        if (cyclesCompleted < totalCycles)
        {
            frameCounter += GetFrameTime();

            // Cambia de frame según el tiempo
            if (frameCounter >= frameTime)
            {
                frameCounter = 0;
                currentFrame++;

                // Si ya pasó todos los frames, reiniciamos y aumentamos el ciclo
                if (currentFrame > 3)
                {
                    currentFrame = 0;
                    cyclesCompleted++; // Se aumenta el ciclo completado
                }

                sourceRect.x = currentFrame * 256; // Actualiza el sprite dentro del sprite sheet
            }
        }
    }
}

void Menu::Draw()
{
    DrawTexturePro(spriteSheet, sourceRect, { 0, 0, 1024, 928 }, { 0, 0 }, 0, WHITE);
}

bool Menu::SingleModeSelected()
{
    return selected&& cyclesCompleted >= totalCycles;
}
