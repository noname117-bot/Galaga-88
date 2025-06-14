#pragma once
#include "raylib.h"
#include "space.hpp"

class GameUI {
public:
    GameUI();
    ~GameUI();
    void Draw();
    void DrawScore();
    Vector2 GetScorePosition() const;
    void UpdateLives(int lives);  
    void UpdateLevel(int level);

private:
    Texture2D score;
    Texture2D life1;
    Texture2D life2;
    Texture2D level;
    Texture2D level2;
    Vector2 position;
    Vector2 position1;
    Vector2 position2;
    Vector2 position3;
    Vector2 position4;
    int currentLives; 
    int currentLevel;
};
