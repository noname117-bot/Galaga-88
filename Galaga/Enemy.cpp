#include "Enemy.hpp"

Enemy::Enemy(int type, Vector2 position)
{

	this->type = type;
	this->position = position;

	switch (type)
	{
	case 1:
		image = LoadTexture("resources/enemies/enemy2_9.png");
		break;
	case 2:
		image = LoadTexture("resources/enemies/enemy2_17.png");
		break;
	}

}

void Enemy::Update()
{
}

void Enemy::Draw()
{

	DrawTextureEx(image, position, 0.0f, 4.0, WHITE);


}

int Enemy::GetType()
{
	return type;
}