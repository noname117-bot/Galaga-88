#include "Menu.hpp"

Menu::Menu()
{
    spriteSheet = LoadTexture("resources/electionsingle-sheet.png");
    spriteSheet2 = LoadTexture("resources/selecion_letras-sheet.png");


    usingSecondAnimation = false;
    frameTimeSelected = 0.05;//para cambiar la velocidad de la animacion de spriteSheet2

    currentFrame = 0;
    frameTime = 0.15f; //para cambiar la velocidad de la animacion de spriteSheet
    frameCounter = 0;
    selected = false;

    animationTime = 0; 
    maxTime = 10.0f;

    totalCycles = 5; 
    cyclesCompleted = 0;

  
    sourceRect = { 0, 0, 256, 232 };

    snd_selection = LoadSound("resources/sound_effects/effect_selection.wav");
    //snd_animation = LoadSound("resources/sound_effects/Animation.wav");


}

Menu::~Menu()
{
    UnloadTexture(spriteSheet);
    UnloadTexture(spriteSheet2);
	//UnloadSound(snd_animation);
	UnloadSound(snd_selection); 
}

void Menu::Update()
{

    if (!selected)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            selected = true;
            usingSecondAnimation = true;   // Cambiar a la segunda animación
            currentFrame = 0;   
            cyclesCompleted = 0;
            frameCounter = 0;
            PlaySound(snd_selection); // Reproducir el sonido del disparo
			//PlaySound(snd_animation); // Reproducir el sonido de la animación
        }
        else
        {
            // Primera animación, antes de presionar ENTER
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
