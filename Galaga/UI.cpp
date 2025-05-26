#include "UI.hpp"

GameUI::GameUI()
{
    score = LoadTexture("resources/UI/score_letters.png");
    life1 = LoadTexture("resources/spaceship.png");
    life2 = LoadTexture("resources/spaceship.png");
    level = LoadTexture("resources/UI/levels.png");
    level2= LoadTexture("resources/UI/levels.png");

    currentLives = 3; 
    currentLevel = 1;
}

GameUI::~GameUI()
{
    UnloadTexture(score);
    UnloadTexture(life1);
    UnloadTexture(life2);
    UnloadTexture(level);
    UnloadTexture(level2);
}

void GameUI::Draw()
{
    DrawScore();
}

void GameUI::DrawScore() {
    float scale = 4.0f;
    position = { 65, 30 }; 
    position1 = { 70, 860 }; 
    position2 = { 5, 860 }; 
    position3 = { 860, 875 }; 
    position4 = { 825, 875 };

    DrawTextureEx(score, position, 0.0f, scale, WHITE);

    if (currentLives >= 3) {
        DrawTextureEx(life2, position1, 0.0f, scale, WHITE);
    }
    if (currentLives >= 2) {
        DrawTextureEx(life1, position2, 0.0f, scale, WHITE);
    }

    if (currentLevel < 3) {
        DrawTextureEx(level, position3, 0.0f, scale, WHITE);
        if (currentLevel == 2) {
            DrawTextureEx(level2, position4, 0.0f, scale, WHITE);
        }
    }
}

void GameUI::UpdateLives(int lives) {
    currentLives = lives;
}

void GameUI::UpdateLevel(int level) {
    currentLevel = level;
}

Vector2 GameUI::GetScorePosition() const
{
    return position;
}