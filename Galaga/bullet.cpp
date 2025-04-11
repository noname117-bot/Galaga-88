#include "Bullet.hpp"
#include <iostream>
using namespace std;

Bullet::Bullet(Vector2 position, int speed)
{
	imagen = LoadTexture("resources/bala.png");

	this->position = position;
	this->speed = speed;
	active = true;
	

}

void Bullet::Draw() {

	if (active)
		DrawTextureEx(imagen, position, 0.0f, 4.0, WHITE);// imagen de la bala de la nave
}

Rectangle Bullet::getRect()
{
	
	Rectangle rect;
	rect.x = position.x;
	rect.y = position.y;
	rect.width = 4;
	rect.height = 15;
	return rect;


}

void Bullet::Update()
{
	position.y += speed;
	if (active)
	{
		if (position.y > GetScreenHeight() || position.y < 0)
		{
			active = false;
			cout << "Bala inactivada" << endl;
		}
	}
}



