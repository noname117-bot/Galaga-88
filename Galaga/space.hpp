#pragma once
#include <raylib.h>

class Spaceship {

public:
	Spaceship();
	~Spaceship();
	void Draw();	
	void MoveLeft(); // basic movements of the spaceship
	void MoveRight();
	void FireLaser();
private:
	Texture2D image;
	Vector2 position;// position of the spaceship

};