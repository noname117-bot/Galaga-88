#include "transicion.hpp"

Transition::Transition() {
    background1 = LoadTexture("resources/bg_startgame-sheet.png");
    spaceship = LoadTexture("resources/spaceship.png");
    shipSprite = LoadTexture("resources/aparicion1-sheet.png");

    score = LoadTexture("resources/score_letters.png");

    backgroundY = 0.0f;
    currentBackgroundFrame = 0;  // Comienza con el primer frame de fondo

    shipY = 800;
    transitionTime = 0;
    currentFrame = 3;  // Comienza en el último frame del spritesheet de la nave
    frameTime = 0.15f;  // Velocidad de la animación de la nave
    frameCounter = 0;

    backgroundFrameCounter = 0;
    backgroundFrameTime = 0.9f;
    finished = false;

    delayTime = 1.0f;
    waitBeforeMoving = true;

    currentPhase = 1;

    sourceRect = { currentFrame * (shipSprite.width / 4), 0.0f, static_cast<float>(shipSprite.width) / 4, static_cast<float>(shipSprite.height) };  // 4 frames en el spritesheet de la nave
}

Transition::~Transition()
{
    UnloadTexture(background1);
    UnloadTexture(spaceship);
    UnloadTexture(shipSprite);
}

void Transition::Update() {
    if (finished) return;

    transitionTime += GetFrameTime();


    backgroundFrameCounter += GetFrameTime();
    if (backgroundFrameCounter >= backgroundFrameTime) {
        backgroundFrameCounter -= backgroundFrameTime;
        currentBackgroundFrame = (currentBackgroundFrame + 1) % 2;
    }
    if (currentPhase == 1) {
        if (transitionTime >= 2.0f) {
            currentPhase = 2; 
        }
    }
    else if (currentPhase == 2) 
    {
        shipY -= 250 * GetFrameTime();
        if (shipY <= 400) {
            shipY = 400; 
            currentPhase = 3; 
        }
    }
    else if (currentPhase == 3) {
      
        backgroundY += 200 * GetFrameTime();

      
        if (backgroundY >= 300) {
            currentPhase = 4;
        }
    }
    else if (currentPhase == 4) {
    
        frameCounter += GetFrameTime();
        if (frameCounter >= frameTime) {
            frameCounter = 0;
            currentFrame--; 

            
            sourceRect.x = currentFrame * (shipSprite.width / 4);

          
            if (currentFrame < 0) {
                finished = true;
            }
        }


        backgroundY += 200 * GetFrameTime();
    }
}

void Transition::Draw() {

    int widthPerFrame = background1.width / 2; 
    Rectangle backgroundSourceRect = { currentBackgroundFrame * widthPerFrame, 0,  widthPerFrame,  background1.height  };

    float backgroundScale = 4.0f;
    float scaledWidth = widthPerFrame * backgroundScale;
    float scaledHeight = background1.height * backgroundScale;
    float backgroundX = (GetScreenWidth() - scaledWidth) / 2;


    DrawTexturePro(background1, backgroundSourceRect, { 0, backgroundY, (float)GetScreenWidth(), (float)GetScreenHeight() },{ 0, 0 },0.0f,WHITE);

    if (currentPhase <= 3) 
    {
        float scale = 4.0;
        DrawTextureEx(spaceship,{ 512 - spaceship.width * scale / 2, shipY - spaceship.height * scale / 2 },0.0f,scale,WHITE);
    }
    else if (currentPhase == 4 && !finished) 
    {
        DrawTexturePro(shipSprite, sourceRect,{ 512, shipY, 128, 128 }, { 64, 64 },0, WHITE);
    }

    DrawTextureEx(score, { 70, 30 }, 0.0f, 4.0f, WHITE);
}

bool Transition::IsFinished() const {
    return finished;
}
