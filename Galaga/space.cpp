#include "space.hpp"

Spaceship::Spaceship()
{
	image = LoadTexture("resources/spaceship.png");
	position.x = (GetScreenWidth() - image.width) / 2;
	position.y = GetScreenHeight() - image.height - 100;
}
Spaceship::~Spaceship()
{
	UnloadTexture(image);
}
void Spaceship::Draw()
{
	DrawTextureEx(image, position, 0.0f, 4.0f, WHITE); // draw the spaceship
}	
void Spaceship::MoveLeft()
{
	position.x -= 5;
}
void Spaceship::MoveRight()
{
	position.x += 5;
}