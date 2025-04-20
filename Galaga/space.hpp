#pragma once
#include <raylib.h>
#include "bullet.hpp"
#include <vector>

#include "enemy_bullet.hpp"
#include "Enemy.hpp"
//#include "UI.hpp"

class Spaceship {

public:
	Spaceship();
	~Spaceship();
	void Draw();	
	void MoveLeft(); // basic movements of the spaceship
	void MoveRight();
	void FireLaser();
	void Update();

	Rectangle getRect();

	
	std::vector<Bullet> bullets;
	std::vector<enemy_Bullet> enemy_bullets;
	std::vector<Enemy> enemies;

	bool isExploding;          // Indica si la explosión está activa
	int explosionFrame;        // Fotograma actual de la explosión
	float explosionFrameTime;  // Tiempo entre fotogramas
	float explosionTimer;      // Temporizador para cambiar de fotograma
	Vector2 explosionPosition; // Posición de la explosión
	int lives;
private:
	Texture2D image;
	Vector2 position;// position of the spaceship
	double lastFiretime;
	double reloadTime; // tiempo en el que se vuelven a recargar las balas 
	int bulletCount; // contador de balas

	Texture2D spriteSheet;
	Texture2D explosion_spriteSheet;
	Texture2D life1;
	Texture2D life2;
	Texture2D game_over;
	int frameWidth, frameHeight;
	int currentFrame;
	float frameTime, frameCounter;
	bool animation;
	int startX, startY;

	// life
	
	Texture2D livesTexture;
	

	// Sound
	Sound snd_bullet;
	//Sound snd_shipExplosion;

};	