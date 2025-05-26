#include <iostream>
#include "transicion.hpp"

Transition::Transition() {
    background1 = LoadTexture("resources/screens/bg_startgame-sheet.png");
    spaceship = LoadTexture("resources/spaceship.png");
    shipSprite = LoadTexture("resources/aparicion1-sheet.png");

    score = LoadTexture("resources/UI/score_letters.png");
    for (int i = 0; i < 10; i++) {
        char file[32];
        sprintf_s(file, sizeof(file), "resources/UI/score/n%d.png", i);
        scoreTextures[i] = LoadTexture(file);
    }

    snd_animation = LoadSound("resources/sound_effects/Animation.wav");
    startAnnouncement = LoadTexture("resources/UI/start.png");

    backgroundY = 0.0f;
    currentBackgroundFrame = 0;

    shipY = 800;
    transitionTime = 0;
    currentFrame = 3;
    frameTime = 0.15f;
    frameCounter = 0;

    backgroundFrameCounter = 0;
    backgroundFrameTime = 0.9f;
    finished = false;

    delayTime = 1.0f;
    waitBeforeMoving = true;

    currentPhase = 1;
    startAnnouncementTimer = 0.0f;
    showStartAnnouncement = false;
    announcementStarted = false;

    // Inicializar puntaje actual (puedes pasarlo como parámetro si lo necesitas)
    currentScore = 0;

    sourceRect = { currentFrame * (shipSprite.width / 4), 0.0f, static_cast<float>(shipSprite.width) / 4, static_cast<float>(shipSprite.height) };
}

Transition::~Transition()
{
    UnloadTexture(background1);
    UnloadTexture(spaceship);
    UnloadTexture(shipSprite);
    UnloadTexture(score);

    // Descargar texturas de números
    for (int i = 0; i < 10; i++) {
        UnloadTexture(scoreTextures[i]);
    }

    UnloadSound(snd_animation);
    UnloadTexture(startAnnouncement);
}

void Transition::Update() {
    if (finished) return;

    if (IsKeyPressed(KEY_THREE)) {
        finished = true;
    }

    transitionTime += GetFrameTime();

    backgroundFrameCounter += GetFrameTime();
    if (backgroundFrameCounter >= backgroundFrameTime) {
        backgroundFrameCounter -= backgroundFrameTime;
        currentBackgroundFrame = (currentBackgroundFrame + 1) % 2;
    }

    if (currentPhase == 1) {
        if (transitionTime <= 0.05) {
            PlaySound(snd_animation);
        }

        if (transitionTime >= 1.0f && !announcementStarted) {
            showStartAnnouncement = true;
            startAnnouncementTimer = startAnnouncementDuration; 
            announcementStarted = true;
        }

        if (showStartAnnouncement) {
            startAnnouncementTimer -= GetFrameTime();
            if (startAnnouncementTimer <= 0.0f) {
                showStartAnnouncement = false;
            }
        }

        if (announcementStarted && !showStartAnnouncement) {
            currentPhase = 2;
        }
    }
    else if (currentPhase == 2) {
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
    Rectangle backgroundSourceRect = { currentBackgroundFrame * widthPerFrame, 0, widthPerFrame, background1.height };

    DrawTexturePro(background1, backgroundSourceRect, { 0, backgroundY, (float)GetScreenWidth(), (float)GetScreenHeight() }, { 0, 0 }, 0.0f, WHITE);

    if (currentPhase <= 3) {
        float scale = 4.0;
        DrawTextureEx(spaceship, { GetScreenWidth() / 2 - spaceship.width * scale / 2, shipY - spaceship.height * scale / 2 }, 0.0f, scale, WHITE);
    }
    else if (currentPhase == 4 && !finished) {
        DrawTexturePro(shipSprite, sourceRect, { static_cast<float>(GetScreenWidth()) / 2, shipY, 128.0f, 128.0f }, { 64.0f, 64.0f }, 0.0f, WHITE);
    }

    DrawScoreUI();

    if (showStartAnnouncement) {
        float scale = 4.0f;
        Vector2 startPos = { (GetScreenWidth() - startAnnouncement.width * scale) / 2, (GetScreenHeight() - startAnnouncement.height * scale) / 2 };
        DrawTextureEx(startAnnouncement, startPos, 0.0f, scale, WHITE);
    }
}

void Transition::DrawScoreUI() {
    DrawTextureEx(score, { 70, 30 }, 0.0f, 4.0f, WHITE);

    Vector2 scorePosition = { 70, 30 };
    float scoreWidth = 120;

    float scoreCenterX = scorePosition.x + (scoreWidth / 2);
    float digitWidth = scoreTextures[0].width * 4.0f;

    float startX = scoreCenterX - (digitWidth * 2) - 10;

    int tempScore = currentScore;

    float spacing = 5.0f; // espacio entre dígitos
    float totalWidth = digitWidth * 4 + spacing * 3;

    if (tempScore == 0) {
        DrawTextureEx(scoreTextures[0], { startX + 3 * (digitWidth + spacing), scorePosition.y + 40 }, 0.0f, 4.0f, WHITE);
    }
    else {
        int digits[4] = { 0, 0, 0, 0 };

        for (int i = 3; i >= 0 && tempScore > 0; i--) {
            digits[i] = tempScore % 10;
            tempScore /= 10;
        }

        bool leadingZero = true;
        for (int i = 0; i < 4; i++) {
            if (leadingZero && digits[i] == 0 && i != 3) {
                continue;
            }
            else {
                leadingZero = false;
                DrawTextureEx(scoreTextures[digits[i]], { startX + i * (digitWidth + spacing), scorePosition.y + 40 }, 0.0f, 4.0f, WHITE);
            }
        }
    }
}

void Transition::Reset() {
    backgroundY = 0.0f;
    currentBackgroundFrame = 0;

    shipY = 800;
    transitionTime = 0;
    currentFrame = 3;
    frameTime = 0.15f;
    frameCounter = 0;

    backgroundFrameCounter = 0;
    backgroundFrameTime = 0.9f;
    finished = false; 

    delayTime = 1.0f;
    waitBeforeMoving = true;

    currentPhase = 1;
    startAnnouncementTimer = 0.0f;
    showStartAnnouncement = false;
    announcementStarted = false;

    sourceRect = { currentFrame * (shipSprite.width / 4), 0.0f, static_cast<float>(shipSprite.width) / 4, static_cast<float>(shipSprite.height) };

   
}

void Transition::SetScore(int score) {
    currentScore = score;
}

bool Transition::IsFinished() const {
    return finished;
}