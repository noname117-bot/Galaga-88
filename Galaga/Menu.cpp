#include "Menu.hpp"

Menu::Menu()
{
    spriteSheet = LoadTexture("resources/electionsingle-sheet.png");
    spriteSheet2 = LoadTexture("resources/selecion_letras-sheet.png");

    introImage = LoadTexture("resources/intro-sheet.png");
    bee = LoadTexture("resources/bee.png");

    usingSecondAnimation = false;
    frameTimeSelected = 0.05;

    currentFrame = 0;
    frameTime = 0.15f; 
    frameCounter = 0;
    selected = false;

    animationTime = 0; 
    maxTime = 10.0f;

    totalCycles = 5; 
    cyclesCompleted = 0;

    sourceRect = { 0, 0, 256, 232 };

    snd_selection = LoadSound("resources/sound_effects/effect_selection.wav");

    introAnimationFinished = false;
    introCurrentFrame = 0;
    introFrameCounter = 0;
    introFrameTime = 0.4f; 
    introSourceRect = { 0, 0, 256, 232 };


    showIntro = true;
    beeFinished = false;
    beePosition = { -100.0f,220.0f };
    beeSpeed = 350.0f;

    beeDelayTimer = 0.0f;
    beeDelayDuration = 5.0f;
    waitingForBee = false;
}

Menu::~Menu()
{

    UnloadTexture(spriteSheet);
    UnloadTexture(spriteSheet2);
	UnloadSound(snd_selection); 

    UnloadTexture(introImage);
    UnloadTexture(bee);
}

void Menu::Update()
{
    if (showIntro)
    {
        if (!introAnimationFinished)
        {
            introFrameCounter += GetFrameTime();

            if (introFrameCounter >= introFrameTime)
            {
                introFrameCounter = 0;
                introCurrentFrame++;

                if (introCurrentFrame > 5)
                {
                    introCurrentFrame = 5; 
                    introAnimationFinished = true;

                    waitingForBee = true; 
                }

                introSourceRect.x = introCurrentFrame * 256;
            }
        }

        if (waitingForBee)
        {
            beeDelayTimer += GetFrameTime();

            if (beeDelayTimer >= beeDelayDuration)
            {
                beeActive = true;
                waitingForBee = false;
            }
        }
        if (beeActive)
        {
            beePosition.x += beeSpeed * GetFrameTime();

            if (beePosition.x > GetScreenWidth() + 100)
            {
                beeFinished = true;
            }
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            showIntro = false;
        }

        return; 
    }

    
    if (!selected)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            selected = true;
            usingSecondAnimation = true;  
            currentFrame = 0;   
            cyclesCompleted = 0;
            frameCounter = 0;
            PlaySound(snd_selection); 
        }
        else
        {

            frameCounter += GetFrameTime();

            float currentFrameTime = selected ? frameTimeSelected : frameTime;

            if (frameCounter >= currentFrameTime)
            {
                frameCounter = 0;
                currentFrame = (currentFrame + 1) % 4;
                sourceRect.x = currentFrame * 256;
            }
        }
    }
    else
    {
        if (cyclesCompleted < totalCycles)
        {
            frameCounter += GetFrameTime();

            float currentFrameTime = selected ? frameTimeSelected : frameTime;

            if (frameCounter >= currentFrameTime)
            {
                frameCounter = 0;
                currentFrame++;

                if (currentFrame > 3)
                {
                    currentFrame = 0;
                    cyclesCompleted++;
                }

                sourceRect.x = currentFrame * 256;
            }
        }
    }
}

void Menu::Draw()
{

    if (showIntro)
    {
        DrawTexturePro(introImage, introSourceRect, { 0, 0, 1024, 928 }, { 0, 0 }, 0, WHITE);

        if (beeActive)
        {
            DrawTextureEx(bee, beePosition,0.0f ,4.0f, WHITE);
        }

        return;
    }

    if (!usingSecondAnimation)
    {
        DrawTexturePro(spriteSheet, sourceRect, { 0, 0, 1024, 928 }, { 0, 0 }, 0, WHITE);
    }
    else
    {
        DrawTexturePro(spriteSheet2, sourceRect, { 0, 0, 1024, 928 }, { 0, 0 }, 0, WHITE);

    }
}

bool Menu::SingleModeSelected()
{
    return selected&& cyclesCompleted >= totalCycles;
}
