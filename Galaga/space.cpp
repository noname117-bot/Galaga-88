#include "space.hpp"

Spaceship::Spaceship()
{
	image = LoadTexture("resources/spaceship.png");
	position.x = (GetScreenWidth() - image.width) / 2;
	position.y = GetScreenHeight() - image.height - 100;
	lastFiretime = 0.0;
	reloadTime = 1.0f;
	bulletCount = 0;
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
	if (position.x < 0)
	{
		position.x = 0;
	}
}
void Spaceship::MoveRight()
{
	position.x += 5;
	float scaleWidth = image.width * 4.0f;
	if (position.x + scaleWidth > GetScreenWidth()) {
		position.x = GetScreenWidth() - scaleWidth;
	}

}
void Spaceship::FireLaser()
{
	if (bulletCount <2 && GetTime() - lastFiretime >= 0.20)// el 0.20 es la diferencia de segundos entre la primera y la segunda bala
	{
		if(IsKeyDown(KEY_X))
		{
			bullets.push_back(Bullet({ position.x + (image.width * 4.0f / 2) - 2, position.y }, -6));
			bulletCount++;
			lastFiretime = GetTime();

		}
	
	}

	if (bulletCount >= 2 && GetTime() - lastFiretime >= reloadTime)
	{
		bulletCount = 0;
	}
}
