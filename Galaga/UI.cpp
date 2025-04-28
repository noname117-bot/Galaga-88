#include "UI.hpp"


GameUI::GameUI()
{

	score = LoadTexture("resources/UI/score_letters.png");

	life1 = LoadTexture("resources/spaceship.png");
	life2 = LoadTexture("resources/spaceship.png");

	level = LoadTexture("resources/UI/levels.png");

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
	
	position = { 65, 30 }; //score 

	position1 = { 70, 860 }; //vida 1 
	position2 = { 5, 860 };//vida 2

	position3 = { 860, 875 };//nivel


	DrawTextureEx(score, position, 0.0f, scale, WHITE);
	
	DrawTextureEx(life1, position1, 0.0f, scale, WHITE);
	DrawTextureEx(life2, position2, 0.0f, scale, WHITE);

	DrawTextureEx(level, position3, 0.0f, scale, WHITE);


}


Vector2 GameUI::GetScorePosition() const
{
	return position;
}
