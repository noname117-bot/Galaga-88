#pragma once
#include <raylib.h>

class Bullet {
public:
	Bullet(Vector2 position, int speed);
	void Update();
	void Draw();
	bool active;

	Rectangle getRect();
private:
	Texture2D imagen;
	Vector2 position;
	int speed;
	 
};