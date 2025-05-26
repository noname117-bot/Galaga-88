	#pragma once
#include <raylib.h>
#include "bullet.hpp"
#include <vector>

#include "enemy_bullet.hpp"
#include "Enemy.hpp"
//#include "UI.hpp"


enum SpaceshipState {
    ALIVE,
    DYING,
    DEAD,
    RESPAWNING
};
class Spaceship {

public:
	Spaceship();
	~Spaceship();
	void Draw();	
	void MoveLeft(); // basic movements of the spaceship
	void MoveRight();
	void FireLaser();
	void Update();
	void LockInCenter(float duration);

    bool IsRespawning() const { return state == RESPAWNING; }
    bool IsDead() const { return state == DEAD; }
    bool IsAlive() const { return state == ALIVE; }
    void SetState(SpaceshipState newState) { state = newState; }
    SpaceshipState GetState() const { return state; }
	Rectangle getRect();

	
	std::vector<Bullet> bullets;
	std::vector<enemy_Bullet> enemy_bullets;
	std::vector<Enemy> enemies;

	bool isExploding;          // Indica si la explosi�n est� activa
	int explosionFrame;        // Fotograma actual de la explosi�n
	float explosionFrameTime;  // Tiempo entre fotogramas
	float explosionTimer;      // Temporizador para cambiar de fotograma
	Vector2 explosionPosition; // Posici�n de la explosi�n
	int lives;

	bool isGameOver;
	
	Vector2 position;// position of the spaceship

	void Reset();
	bool isLocked;
	float lockTimer;

	Texture2D blinkingSpriteSheet;
	int blinkingFrame;
	int blinkingFrameCounter;
	int blinkingFrameSpeed;
	int blinkingFrameWidth;
	int blinkingFrameHeight;
	float respawnStartY;
	float respawnTargetY;
	float respawnSpeed;

	SpaceshipState state;
	float respawnTimer;
	float respawnAnimationTimer;
	bool isRespawning;


	void StartBlinkingRespawn();
	void UpdateBlinkingRespawn();
	void DrawBlinkingRespawn();

private:
	Texture2D image;
	double lastFiretime;
	double reloadTime; // tiempo en el que se vuelven a recargar las balas 
	int bulletCount; // contador de balas

	Texture2D spriteSheet;
	Texture2D explosion_spriteSheet;
	Texture2D life1;
	Texture2D life2;

	int frameWidth, frameHeight;
	int currentFrame;
	float frameTime, frameCounter;
	bool animation;
	int startX, startY;




	
	Texture2D livesTexture;
	

	// Sound
	Sound snd_bullet;
	//Sound snd_shipExplosion;

};	