#include "enemy_bullet.hpp"
#include <iostream>
using namespace std;

enemy_Bullet::enemy_Bullet(Vector2 position, int speed)
{
	imagen = LoadTexture("resources/enemy_bullet_red.png");

	this->position = position;
	this->speed = speed;
	active = true;


}

void enemy_Bullet::Draw() {

	if (active)
		DrawTextureEx(imagen, position, 0.0f, 4.0, WHITE);// imagen de la bala enemiga
}

Rectangle enemy_Bullet::getRect()
{

	Rectangle rect;
	rect.x = position.x + (imagen.width * 4.0f * 0.25f);
	rect.y = position.y;
	rect.width = imagen.width * 4.0 * 0.5f;
	rect.height = imagen.height * 4.0 * 0.8f;
	return rect;


}

void enemy_Bullet::Update()
{
	position.y -= speed;
	if (active)
	{
		if (position.y > GetScreenHeight() || position.y > 928)
		{
			active = false;
			cout << "Bala enemiga inactivada" << endl;
		}
	}
}



