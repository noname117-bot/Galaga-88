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
	int initWidth = 1024; // ancho de la pantalla
	int initHeight = 928; // alto de la pantalla

	InitWindow(initWidth, initHeight, "Galaga ");
	SetTargetFPS(60); 
	InitAudioDevice();
	Texture2D background_image= LoadTexture("resources/bg_stage1_2.png");

	
	float scale = 4.0f; //aumentamos las dimensiones de la imagen de fondo
	Vector2 position = { 0,0 }; //posicionamos el fondo en el centro

	float fadeAlpha = 1.0f;
	bool FadingIn = false;


	 //
	Game game; // crea un objeto de la clase Game	
	Menu menu;
	GameUI ui;
	
	bool inTransition = false;
	Transition transition;

    while (WindowShouldClose()==false) {

		if (!menu.SingleModeSelected())
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

			if (!FadingIn)
			{
				FadingIn = true;
			}

			if (fadeAlpha > 0.0f)
			{
				fadeAlpha -= 0.7f * GetFrameTime();
				if (fadeAlpha < 0.0f)fadeAlpha = 0.0f;
			}
			game.Update();
		}
		
		BeginDrawing(); // dibuja un canva blanco( básicamente es la pantalla principal) 
		ClearBackground(BLACK);

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
			//
			DrawTextureEx(background_image, position, 0.0f, scale, WHITE);
			game.Draw();
			ui.Draw();

			if (fadeAlpha > 0.0f)
			{
				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, fadeAlpha));
			}
		}

	
		EndDrawing();
    }

    
    CloseWindow();

    return 0;
}
