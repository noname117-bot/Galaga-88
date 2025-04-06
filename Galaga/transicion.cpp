#include "transicion.hpp"

Transition::Transition() {
    background1 = LoadTexture("resources/bg_startgame.png");
    spaceship = LoadTexture("resources/spaceship.png");
    shipSprite = LoadTexture("resources/aparicion1-sheet.png");

    shipY = 800;  // La nave comienza fuera de la pantalla
    transitionTime = 0;
    currentFrame = 3; // Empezamos desde el último frame (última imagen del spritesheet)
    frameTime = 0.15f;  // Velocidad de animación
    frameCounter = 0;
    finished = false;

    delayTime = 1.0f; // Tiempo de espera antes de mover la nave hacia arriba
    waitBeforeMoving = true;

    // Configurar el primer frame del sprite de la nave
    sourceRect = { currentFrame * (shipSprite.width / 4), 0.0f, static_cast<float>(shipSprite.width) / 4, static_cast<float>(shipSprite.height) };  // 4 frames en el spritesheet
}

Transition::~Transition() {
    UnloadTexture(background1);

    UnloadTexture(spaceship);
    UnloadTexture(shipSprite);
}

void Transition::Update() {
    if (finished) return;

    transitionTime += GetFrameTime();

    // Mueve la nave hacia arriba hasta el centro (usamos el sprite suelto para este movimiento)
    if (waitBeforeMoving && transitionTime >= delayTime) {
        waitBeforeMoving = false;  // Ya no esperamos más
    }

    if (!waitBeforeMoving) {
        // Mueve la nave hacia arriba hasta el centro (usamos el sprite suelto para este movimiento)
        if (shipY > 400) {
            shipY -= 100 * GetFrameTime();  // Ajusta la velocidad si es necesario
        }
    }

    // Después de 3 segundos, comienza la animación de la nave
    if (transitionTime >= 3.0f && transitionTime < 5.0f) {
        // Empieza la animación de desaparición después de un breve retraso
        frameCounter += GetFrameTime();
        if (frameCounter >= frameTime) {
            frameCounter = 0;
            currentFrame--;

            // Si la animación de desaparición termina, marca como finalizado
            if (currentFrame < 0) {
                finished = true;
            }
            sourceRect.x = currentFrame * (shipSprite.width / 4);  // Cambiar de frame en el sprite sheet
        }
    }
}

void Transition::Draw() {
    if (transitionTime < 2.5f) {
        DrawTextureEx(background1, { 0, 0 }, 0.0f, 4.0, WHITE);  // Fondo de la estación espacial
    }

    // Dibuja el sprite de la nave separada durante los primeros 3 segundos
    if (transitionTime < 3.0f) {
        float scale = 4.0;
        DrawTextureEx(spaceship, { 512 - spaceship.width * scale / 2, shipY - spaceship.height * scale / 2 }, 0.0f, scale, WHITE);
    }

    // Después de 3 segundos, muestra la animación del sprite sheet
    if (!finished && transitionTime >= 3.0f) {
        DrawTexturePro(shipSprite, sourceRect, { 512, shipY, 128, 128 }, { 64, 64 }, 0, WHITE);
    }
}

bool Transition::IsFinished() const {
    return finished;
}
