#include <iostream>
using namespace std;
#include <raylib.h>
#include "game.hpp"
#include "space.hpp"
#include "enemy.hpp"
#include "Menu.hpp"
#include "UI.hpp"
#include "Transicion.hpp"



int main() {

	// color gris oscuro
	int initWidth = 896; // ancho de la pantalla
	int initHeight = 928; // alto de la pantalla



	InitWindow(initWidth, initHeight, "Galaga ");
	SetTargetFPS(60); 
	InitAudioDevice();
	Texture2D background_image= LoadTexture("resources/screens/bg_stage1_2.png");
	Texture2D gameOverScreen = LoadTexture("resources/screens/gameOverScreen.png");
	Texture2D splashScreen = LoadTexture("resources/screens/splash.png");
	
	float scale = 4.0f; //aumentamos las dimensiones de la imagen de fondo
	Vector2 position = { (initWidth - background_image.width * scale) / 2,  // Centrado horizontal
	(initHeight - background_image.height * scale) / 2  // Centrado vertical
	}; //posicionamos el fondo en el centro

	float fadeAlpha = 1.0f;
	bool FadingIn = false;


	 //
	Game game; // crea un objeto de la clase Game	
	Menu menu;
	GameUI ui;
	Spaceship space;
	
	bool inTransition = false;
	Transition transition;

	float gameOverTimer = 0.0f;
    const float gameOverDelay = 15.0f; // 15 segundos
    bool gameOver = false;



    while (WindowShouldClose()==false) {



		if (game.getSpaceship().lives <= 0 && !gameOver) {
			gameOver = true;
			gameOverTimer = 0.0f;
		}

		if (gameOver) {
			// Gestionar el estado de Game Over
			gameOverTimer += GetFrameTime();
			// Comprobar si han pasado 15 segundos o se ha pulsado espacio
			if (gameOverTimer >= gameOverDelay || IsKeyPressed(KEY_SPACE)) {
				// Reiniciar el juego
				gameOver = false;
				gameOverTimer = 0.0f;
				// Crear una nueva instancia del menú y asegurarnos que muestre la intro
				menu = Menu();
				menu.SetShowIntro(true);
				menu.Reset();
				// Reiniciar la transición y el fade
				inTransition = false;
				FadingIn = false;
				fadeAlpha = 1.0f;
				// Reiniciar el juego si es necesario
				game = Game();
				// Make sure to reset the spaceship inside the game object
				game.getSpaceship().Reset();
			}
		}
		else if (!menu.SingleModeSelected())
		{
			menu.Update();
		}
		else if (!inTransition)
		{
			inTransition = true;

		}
		else if (!transition.IsFinished())
		{
			transition.Update();
			
		}
		else
		{
			if(game.getSpaceship().lives > 0)
			{
				if (!FadingIn)
				{
					FadingIn = true;
				}

				if (fadeAlpha > 0.0f)
				{
					fadeAlpha -= 0.7f * GetFrameTime();
					if (fadeAlpha < 0.0f)fadeAlpha = 0.0f;
				}
			}
			game.Update();
		}
		
		BeginDrawing(); // dibuja un canva blanco( básicamente es la pantalla principal) 
		ClearBackground(BLACK);

		if (game.getSpaceship().lives <= 0) {
			Vector2 gameOverPos = { (GetScreenWidth() - gameOverScreen.width * scale) / 2, (GetScreenHeight() - gameOverScreen.height * scale) / 2 };
			DrawTextureEx(gameOverScreen, gameOverPos, 0.0f, 4.0f, WHITE);

		}
		else
		{
			if (!menu.SingleModeSelected())
			{
				menu.Draw();
			}
			else if (!transition.IsFinished())
			{
				transition.Draw();
			}
			else
			{

				DrawTextureEx(background_image, position, 0.0f, scale, WHITE);
				game.Draw();
				ui.Draw();

				if (fadeAlpha > 0.0f)
				{
					DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, fadeAlpha));
				}
			}
		}

	
		EndDrawing();
    }

    
    CloseWindow();

    return 0;
}
