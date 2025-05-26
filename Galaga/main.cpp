#include <iostream>
#include <raylib.h>
#include "game.hpp"
#include "space.hpp"
#include "enemy.hpp"
#include "Menu.hpp"
#include "UI.hpp"
#include "Transicion.hpp"
using namespace std;

enum GameState {
    STATE_INTRO,
    STATE_INTRO_ANIMATION,
    STATE_MENU,
    STATE_TRANSITION,
    STATE_GAMEPLAY,
    STATE_BOSS_DEATH_ANIMATION,
    STATE_WIN,
    STATE_GAMEOVER_ANNOUNCEMENT,
    STATE_GAMEOVER
};

int main() {
    int initWidth = 896;
    int initHeight = 928;
    InitWindow(initWidth, initHeight, "Galaga");
    SetTargetFPS(60);
    InitAudioDevice();


    Music gameOverMusic = LoadMusicStream("resources/sound_effects/GameOverMusic.wav"); // Música para game over
    Music creditsMusic = LoadMusicStream("resources/sound_effects/Credits.wav");   // Música para créditos

    bool gameOverMusicPlaying = false;
    bool creditsMusicPlaying = false;

    SetMusicVolume(gameOverMusic,70.0f);
    SetMusicVolume(creditsMusic, 70.0f);

    Texture2D intro = LoadTexture("resources/screens/intro-sheet.png");

    Texture2D background_image = LoadTexture("resources/screens/bg_stage1_2.png");
    Texture2D background_level3 = LoadTexture("resources/screens/Boss1_background.png");

    Texture2D gameOverScreen = LoadTexture("resources/screens/gameover1.png");
    Texture2D gameOverScreen2 = LoadTexture("resources/screens/gameover2.png");
    Texture2D gameOverScreen3 = LoadTexture("resources/screens/gameover3.png");

    Texture2D gameOverAnnouncement = LoadTexture("resources/UI/game_over.png");

    Texture2D stage2Announcement = LoadTexture("resources/UI/stage2.png"); 
    Texture2D stage1Announcement = LoadTexture("resources/UI/stage1.png");

    Texture2D introAnimation = LoadTexture("resources/screens/intro-sheet.png");
    Texture2D winScreen = LoadTexture("resources/screens/credits.png");

    int frameWidth = introAnimation.width / 2;
    int frameHeight = introAnimation.height;
    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 8;

    float scale = 4.0f;
    Vector2 position = { (initWidth - background_image.width * scale) / 2, (initHeight - background_image.height * scale) / 2 };
    Vector2 animationPosition;

    float fadeAlpha = 1.0f;
    bool FadingIn = false;

    float stageAnnouncementTimer = 0.0f;
    const float stageAnnouncementDuration = 2.0f;

    float gameOverAnnouncementTimer = 0.0f;
    const float gameOverAnnouncementDuration = 3.0f;
    float gameOverAnimationProgress = 0.0f;
    const float gameOverAnimationDuration = 1.0f; 
    Vector2 gameOverStartPos = { 0, 0 }; 
    Vector2 gameOverEndPos = { 0, 0 };


    float winTimer = 0.0f;
    const float winDuration = 30.0f;

    float bossDeathAnimationTimer = 0.0f;
    const float bossDeathAnimationDuration = 5.5f;
    Vector2 shipCenterPosition;
    Vector2 shipStartPosition;
    Vector2 shipEndPosition;
    float backgroundOffset = 0.0f;
    bool shipMovingToCenter = true;
    bool shipMovingUp = false;
    bool shipDisappeared = false;

    Game game;
    Menu menu;
    GameUI ui;
    Spaceship space;
    Transition transition;

    int lastLives = 3; 
    ui.UpdateLives(3);
    ui.UpdateLevel(1);
    float gameOverTimer = 0.0f;
    const float gameOverDelay = 15.0f;

    bool gameOver = false;
    bool showStage1Announcement = true;

    GameState currentState = STATE_INTRO;

    while (!WindowShouldClose()) {

        if (gameOverMusicPlaying) {
            UpdateMusicStream(gameOverMusic);
        }
        if (creditsMusicPlaying) {
            UpdateMusicStream(creditsMusic);
        }
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
        case STATE_INTRO:
            DrawTextureEx(intro, position, 0.0f, scale, WHITE);

            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentState = STATE_INTRO_ANIMATION;
                framesCounter = 0;
                currentFrame = 0;
            }
            break;

        case STATE_INTRO_ANIMATION:
            DrawTextureEx(intro, position, 0.0f, scale, WHITE);

            animationPosition.x = (initWidth - frameWidth * scale) / 2;
            animationPosition.y = (initHeight - frameHeight * scale) / 2;

            framesCounter++;
            if (framesCounter >= (60 / framesSpeed)) {
                framesCounter = 0;
                currentFrame++;

                if (currentFrame > 1) {
                    currentState = STATE_MENU;
                }
            }

            {
                Rectangle sourceRec = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
                Rectangle destRec = { animationPosition.x, animationPosition.y, frameWidth * scale, frameHeight * scale };
                Vector2 origin = { 0, 0 };
                DrawTexturePro(introAnimation, sourceRec, destRec, origin, 0.0f, WHITE);
            }
            break;

        case STATE_MENU:

            if (gameOverMusicPlaying) {
                StopMusicStream(gameOverMusic);
                gameOverMusicPlaying = false;
            }
            if (creditsMusicPlaying) {
                StopMusicStream(creditsMusic);
                creditsMusicPlaying = false;
            }

            if (!menu.SingleModeSelected()) {
                menu.Update();
                menu.Draw();
            }
            else {
                currentState = STATE_TRANSITION;
            }
            break;

        case STATE_TRANSITION:
            transition.Update();
            transition.Draw();
            if (transition.IsFinished()) {
                currentState = STATE_GAMEPLAY;
                FadingIn = false;
                fadeAlpha = 1.0f;
            }
            break;

        case STATE_GAMEPLAY:
        {
           
            if (game.getSpaceship().lives <= 0) {
                currentState = STATE_GAMEOVER_ANNOUNCEMENT;;
                gameOverAnnouncementTimer = gameOverAnnouncementDuration;
                gameOverAnimationProgress = 0.0f;
                gameOverEndPos.x = (GetScreenWidth() - gameOverAnnouncement.width * scale) / 2;
                gameOverEndPos.y = (GetScreenHeight() - gameOverAnnouncement.height * scale) / 2;
                gameOverStartPos.x = gameOverEndPos.x;
                gameOverStartPos.y = -gameOverAnnouncement.height * scale;
            }
            else if (game.areEnemiesDefeated() && stageAnnouncementTimer <= 0.0f) {
                int currentLevel = game.getCurrentLevel();

                if (currentLevel >= 3) {
                    currentState = STATE_BOSS_DEATH_ANIMATION;
                    bossDeathAnimationTimer = 0.0f;

                    shipStartPosition = game.getSpaceship().position;
                    shipCenterPosition = { (float)GetScreenWidth() / 2.0f - 32.0f, shipStartPosition.y };
                    shipEndPosition = { shipCenterPosition.x, -100.0f };

                    backgroundOffset = 0.0f;
                    shipMovingToCenter = true;
                    shipMovingUp = false;
                    shipDisappeared = false;
                }
                else {
                    game.nextLevel();
                    ui.UpdateLevel(game.getCurrentLevel());

                    if (game.getCurrentLevel() == 2) {
                        stageAnnouncementTimer = stageAnnouncementDuration;
                    }

                    game.getSpaceship().LockInCenter(1.0f);
                }
            }


            game.Update();

            int currentLives = game.getSpaceship().lives;
            if (currentLives != lastLives && currentLives > 0) {
                ui.UpdateLives(currentLives);
                lastLives = currentLives;

                static float deathTimer = 0.0f;
                if (game.getSpaceship().IsDead()) {
                    deathTimer += GetFrameTime();
                    if (deathTimer >= 1.0f) { 
                        game.getSpaceship().DrawBlinkingRespawn();
                        deathTimer = 0.0f;
                    }
                }
            }
            else if (currentLives != lastLives) {
                ui.UpdateLives(currentLives);
                lastLives = currentLives;
            }

          
            int currentLevel = game.getCurrentLevel();
            if (currentLevel == 1) {
                DrawTextureEx(background_image, position, 0.0f, scale, WHITE);
            }
            else if (currentLevel == 2) {
                DrawTextureEx(background_image, position, 0.0f, scale, WHITE);
            }
            else { 
                DrawTextureEx(background_level3, position, 0.0f, scale, WHITE);
            }

            game.Draw();
            ui.Draw();

            if (showStage1Announcement && game.getCurrentLevel() == 1) {
                Vector2 stage1Pos = { (GetScreenWidth() - stage1Announcement.width * scale) / 2,   (GetScreenHeight() - stage1Announcement.height * scale) / 2 };
                DrawTextureEx(stage1Announcement, stage1Pos, 0.0f, scale, WHITE);

                static float stage1Timer = stageAnnouncementDuration;
                stage1Timer -= GetFrameTime();
                if (stage1Timer <= 0.0f) {
                    showStage1Announcement = false;
                    stage1Timer = stageAnnouncementDuration;
                }
            }

            if (stageAnnouncementTimer > 0.0f && game.getCurrentLevel() == 2) {
                Vector2 stagePos = { (GetScreenWidth() - stage2Announcement.width * scale) / 2,  (GetScreenHeight() - stage2Announcement.height * scale) / 2 };
                DrawTextureEx(stage2Announcement, stagePos, 0.0f, scale, WHITE);

                stageAnnouncementTimer -= GetFrameTime();
                if (stageAnnouncementTimer <= 0.0f) {
                    stageAnnouncementTimer = 0.0f;
                }
            }

            if (!FadingIn && fadeAlpha > 0.0f) {
                fadeAlpha -= 0.7f * GetFrameTime();
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, fadeAlpha));
                if (fadeAlpha <= 0.0f) FadingIn = true;
            }
            break;
        }
        case STATE_BOSS_DEATH_ANIMATION:
        {
            bossDeathAnimationTimer += GetFrameTime();
            float progress = bossDeathAnimationTimer / bossDeathAnimationDuration;

            if (bossDeathAnimationTimer < 1.5f && shipMovingToCenter) {
                float centerProgress = bossDeathAnimationTimer / 1.5f;
                float easedProgress = centerProgress * centerProgress * (3.0f - 2.0f * centerProgress);

                game.getSpaceship().position.x = shipStartPosition.x + (shipCenterPosition.x - shipStartPosition.x) * easedProgress;
                game.getSpaceship().position.y = shipStartPosition.y + (shipCenterPosition.y - shipStartPosition.y) * easedProgress;
            }
            else if (bossDeathAnimationTimer >= 1.5f && bossDeathAnimationTimer < 5.0f) {
                if (shipMovingToCenter) {
                    shipMovingToCenter = false;
                    shipMovingUp = true;
                }
                float upProgress = (bossDeathAnimationTimer - 1.5f) / 3.5f;
                float easedUpProgress = upProgress * upProgress * (3.0f - 2.0f * upProgress);

                game.getSpaceship().position.y = shipCenterPosition.y + (shipEndPosition.y - shipCenterPosition.y) * easedUpProgress;

                backgroundOffset = 928.0f * easedUpProgress; 
            }
            else if (bossDeathAnimationTimer >= 5.0f) {
                shipDisappeared = true;
            }
            Vector2 backgroundPos = { position.x, position.y + backgroundOffset };
            DrawTextureEx(background_level3, backgroundPos, 0.0f, scale, WHITE);

            if (!shipDisappeared) {
                game.getSpaceship().Draw();
            }
            ui.Draw();

            if (bossDeathAnimationTimer >= bossDeathAnimationDuration) {
                currentState = STATE_WIN;
                winTimer = 0.0f;

                if (!creditsMusicPlaying) {
                    PlayMusicStream(creditsMusic);
                    creditsMusicPlaying = true;
                }
            }

            break;
        }
        case STATE_WIN:
        {
            Vector2 winPos = { (GetScreenWidth() - winScreen.width * scale) / 2, (GetScreenHeight() - winScreen.height * scale) / 2 };
            DrawTextureEx(winScreen, winPos, 0.0f, scale, WHITE);
            winTimer += GetFrameTime();

            if (winTimer >= winDuration || IsKeyPressed(KEY_SPACE)) {
                currentState = STATE_GAMEOVER;
                gameOverTimer = 0.0f;

                if (creditsMusicPlaying) {
                    StopMusicStream(creditsMusic);
                    creditsMusicPlaying = false;
                }
                if (!gameOverMusicPlaying) {
                    PlayMusicStream(gameOverMusic);
                    gameOverMusicPlaying = true;
                }
            }
            break;
        }
        case STATE_GAMEOVER_ANNOUNCEMENT: 
        {
            game.Update();

            int currentLevel = game.getCurrentLevel();
            if (currentLevel == 1) {
                DrawTextureEx(background_image, position, 0.0f, scale, WHITE);
            }
            else if (currentLevel == 2) {
                DrawTextureEx(background_image, position, 0.0f, scale, WHITE);
            }
            else {
                DrawTextureEx(background_level3, position, 0.0f, scale, WHITE);
            }

            game.Draw();
            ui.Draw();

            if (gameOverAnimationProgress < 1.0f) {
                gameOverAnimationProgress += GetFrameTime() / gameOverAnimationDuration;
                if (gameOverAnimationProgress > 1.0f) {
                    gameOverAnimationProgress = 1.0f;
                }
            }

            float easedProgress = 1.0f - (1.0f - gameOverAnimationProgress) * (1.0f - gameOverAnimationProgress);
            Vector2 currentGameOverPos = { gameOverStartPos.x + (gameOverEndPos.x - gameOverStartPos.x) * easedProgress,  gameOverStartPos.y + (gameOverEndPos.y - gameOverStartPos.y) * easedProgress  };
            DrawTextureEx(gameOverAnnouncement, currentGameOverPos, 0.0f, scale, WHITE);

            gameOverAnnouncementTimer -= GetFrameTime();

            if (gameOverAnnouncementTimer <= 0.0f) {
                currentState = STATE_GAMEOVER;
                gameOverTimer = 0.0f;

                if (!gameOverMusicPlaying) {
                    PlayMusicStream(gameOverMusic);
                    gameOverMusicPlaying = true;
                }
            }
            break;
        }
        case STATE_GAMEOVER:
        {

            Texture2D currentGameOverScreen;
            if (game.getCurrentLevel() == 1) {
                currentGameOverScreen = gameOverScreen;
            }
            else if (game.getCurrentLevel()==2){
                currentGameOverScreen = gameOverScreen2;
            }
            else {
                currentGameOverScreen = gameOverScreen3;

            }

            Vector2 gameOverPos = { (GetScreenWidth() - gameOverScreen.width * scale) / 2, (GetScreenHeight() - gameOverScreen.height * scale) / 2 };
            DrawTextureEx(currentGameOverScreen, gameOverPos, 0.0f, 4.0f, WHITE);
            gameOverTimer += GetFrameTime();
            if (gameOverTimer >= gameOverDelay || IsKeyPressed(KEY_SPACE)) {
                currentState = STATE_MENU;
                menu = Menu();
                menu.SetShowIntro(true);
                menu.Reset();
                
                transition.Reset();

                game = Game();
                game.getSpaceship().Reset();
                game.Reset();

                ui.UpdateLives(3); 
                ui.UpdateLevel(1);
                lastLives = 3;
                
                FadingIn = false;
                fadeAlpha = 1.0f;
                showStage1Announcement = true;

                if (gameOverMusicPlaying) {
                    StopMusicStream(gameOverMusic);
                    gameOverMusicPlaying = false;
                }
            }
            break;
        }
        }

        EndDrawing();
    }

    UnloadMusicStream(gameOverMusic);
    UnloadMusicStream(creditsMusic);

    UnloadTexture(background_image);
    UnloadTexture(background_level3);

    UnloadTexture(intro);
    UnloadTexture(introAnimation);
    
    UnloadTexture(stage1Announcement);
    UnloadTexture(stage2Announcement); 

    UnloadTexture(gameOverScreen);
    UnloadTexture(gameOverScreen2);
    UnloadTexture(gameOverScreen3);
    UnloadTexture(gameOverAnnouncement);

    UnloadTexture(winScreen);

    CloseWindow();
    return 0;
}