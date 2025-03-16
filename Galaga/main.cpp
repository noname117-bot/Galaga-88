#include <iostream>
using namespace std;
#include <raylib.h>
#include "game.hpp"



int main() {

	 // color gris oscuro
	int initWidth = 1024; // ancho de la pantalla
	int initHeight = 928; // alto de la pantalla

	InitWindow(initWidth, initHeight, "C++ Space Invaders ");
	SetTargetFPS(60); // establece la cantidad de fotogramas por segundo
	
	Texture2D background_image= LoadTexture("resources/bg_stage1_2.png");
	float scale = 4.0f; //aumentamos las dimensiones de la imagen de fondo
	Vector2 position = { 0,0 }; //posicionamos el fondo en el centro

	Game game; // crea un objeto de la clase Game	

    while (WindowShouldClose()==false) {

		game.HandleInput(); 
		game.Update();
		BeginDrawing(); // dibuja un canva blanco( básicamente es la pantalla principal) 
		
		DrawTextureEx(background_image, position, 0.0f,scale,  WHITE); //dibujamos el fondo
		game.Draw(); // llama al metodo Draw de la clase Game
		EndDrawing();
    }

    
    CloseWindow();

    return 0;
}
