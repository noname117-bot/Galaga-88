#include "UI.hpp"

GameUI::GameUI()
{

	score = LoadTexture("resources/score_letters.png");


	currentScore = "0000";

}

GameUI::~GameUI()
{
	DrawScore();
}

void GameUI::Draw()
{
	DrawScore();
}

void GameUI::DrawScore() {

	float scale = 4.0f;
	Vector2 position = { 750, 30 };

	DrawTextureEx(score, position, 0.0f, scale, WHITE);

}
