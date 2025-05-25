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
    STATE_GAMEOVER_ANNOUNCEMENT,
    STATE_GAMEOVER
};

int main() {
    int initWidth = 896;
    int initHeight = 928;
    InitWindow(initWidth, initHeight, "Galaga");
    SetTargetFPS(60);
    InitAudioDevice();

    Texture2D intro = LoadTexture("resources/screens/intro-sheet.png");

    Texture2D background_image = LoadTexture("resources/screens/bg_stage1_2.png");
    Texture2D background_level2 = LoadTexture("resources/screens/bg_stage1_2.png");

    Texture2D gameOverScreen = LoadTexture("resources/screens/gameover1.png");
    Texture2D gameOverScreen2 = LoadTexture("resources/screens/gameover2.png");

    Texture2D gameOverAnnouncement = LoadTexture("resources/UI/game_over.png");

    Texture2D stage2Announcement = LoadTexture("resources/UI/stage2.png"); 
    Texture2D stage1Announcement = LoadTexture("resources/UI/stage1.png");

    Texture2D introAnimation = LoadTexture("resources/screens/intro-sheet.png");

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
                game.nextLevel(); 
                ui.UpdateLevel(game.getCurrentLevel());
                stageAnnouncementTimer = stageAnnouncementDuration; 
                game.getSpaceship().LockInCenter(1.0f);
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
                        game.getSpaceship().StartRespawn();
                        deathTimer = 0.0f;
                    }
                }
            }
            else if (currentLives != lastLives) {
                ui.UpdateLives(currentLives);
                lastLives = currentLives;
            }

          
            if (game.getCurrentLevel() == 1) {
                DrawTextureEx(background_image, position, 0.0f, scale, WHITE);
            }
            else {
                DrawTextureEx(background_level2, position, 0.0f, scale, WHITE);
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

            if (stageAnnouncementTimer > 0.0f) {
                Vector2 stagePos = { (GetScreenWidth() - stage2Announcement.width * scale) / 2,  (GetScreenHeight() - stage2Announcement.height * scale) / 2  };
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
        case STATE_GAMEOVER_ANNOUNCEMENT: 
        {
            game.Update();

            if (game.getCurrentLevel() == 1) {
                DrawTextureEx(background_image, position, 0.0f, scale, WHITE);
            }
            else {
                DrawTextureEx(background_level2, position, 0.0f, scale, WHITE);
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
            }
            break;
        }
        case STATE_GAMEOVER:
        {

            Texture2D currentGameOverScreen;
            if (game.getCurrentLevel() == 1) {
                currentGameOverScreen = gameOverScreen;
            }
            else {
                currentGameOverScreen = gameOverScreen2;
            }

            Vector2 gameOverPos = { (GetScreenWidth() - gameOverScreen.width * scale) / 2, (GetScreenHeight() - gameOverScreen.height * scale) / 2 };
            DrawTextureEx(currentGameOverScreen, gameOverPos, 0.0f, 4.0f, WHITE);
            gameOverTimer += GetFrameTime();
            if (gameOverTimer >= gameOverDelay || IsKeyPressed(KEY_SPACE)) {
                currentState = STATE_MENU;
                menu = Menu();
                menu.SetShowIntro(true);
                menu.Reset();
                game = Game();
                game.getSpaceship().Reset();
                ui.UpdateLives(3); 
                ui.UpdateLevel(1);
                lastLives = 3;
                FadingIn = false;
                fadeAlpha = 1.0f;
                showStage1Announcement = true;
            }
            break;
        }
        }

        EndDrawing();
    }

    UnloadTexture(background_image);
    UnloadTexture(background_level2);

    UnloadTexture(intro);
    UnloadTexture(introAnimation);
    
    UnloadTexture(stage1Announcement);
    UnloadTexture(stage2Announcement); 

    UnloadTexture(gameOverScreen);
      UnloadTexture(gameOverScreen2);
    UnloadTexture(gameOverAnnouncement);

    CloseWindow();
    return 0;
}