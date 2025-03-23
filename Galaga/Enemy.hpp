#pragma once
#include <raylib.h>

class Enemy {
public:
	Enemy(int type, Vector2 position);
	void Update();
	void Draw();
	int GetType();
	Texture2D image;
	int type;
	Vector2 position;
private:


};