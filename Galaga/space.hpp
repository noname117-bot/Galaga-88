#pragma once
#include <raylib.h>
#include "bullet.hpp"
#include <vector>

class Spaceship {

public:
	Spaceship();
	~Spaceship();
	void Draw();
	void MoveLeft(); // basic movements of the spaceship
	void MoveRight();
	void FireLaser();
	std::vector<Bullet> bullets;
private:
	Texture2D image;
	Vector2 position;// position of the spaceship

};