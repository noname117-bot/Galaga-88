#pragma once
#include "space.hpp"

class Game {
	public:
	Game();
	~Game();
	void Update();
	void Draw();
	void HandleInput();
private:
	void DeleteInactiveBullet();
	Spaceship spaceship;

};