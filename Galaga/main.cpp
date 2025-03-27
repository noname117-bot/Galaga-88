#include <iostream>
using namespace std;
#include <raylib.h>
#include "game.hpp"
#include "space.hpp"
#include "enemy.hpp"
#include "Menu.hpp"


int main() {

	 // color gris oscuro
	int initWidth = 1024; // ancho de la pantalla
	int initHeight = 928; // alto de la pantalla

	InitWindow(initWidth, initHeight, "Galaga ");
	SetTargetFPS(60); 

	
	Texture2D background_image= LoadTexture("resources/bg_stage1_2.png");


	
	float scale = 4.0f; //aumentamos las dimensiones de la imagen de fondo
	Vector2 position = { 0,0 }; //posicionamos el fondo en el centro




	Game game; // crea un objeto de la clase Game	
	Menu menu;
	
    while (WindowShouldClose()==false) {

		if (!menu.SingleModeSelected())
		{
			menu.Update();
		}
		else
		{
			game.Update();
		}
		
		BeginDrawing(); // dibuja un canva blanco( básicamente es la pantalla principal) 
		ClearBackground(BLACK);

		if (!menu.SingleModeSelected())
		{
			menu.Draw();
		}
		else
		{
			DrawTextureEx(background_image, position, 0.0f, scale, WHITE);
			game.Draw();
		}

	
		EndDrawing();
    }

    
    CloseWindow();

    return 0;
}
