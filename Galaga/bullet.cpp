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
		DrawTextureEx(imagen, position, 0.0f, 3.5, WHITE);// imagen de la bala de la nave
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



