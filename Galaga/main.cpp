#include <iostream>
using namespace std;
#include <raylib.h>
//#include "space.hpp"
#include "game.hpp"
int main() {

	Color grey = { 29,29,27,255 }; // color gris oscuro
	int initWidth = 1045; // ancho de la pantalla
	int initHeight = 940; // alto de la pantalla

	InitWindow(initWidth, initHeight, "C++ Space Invaders ");
	SetTargetFPS(60); // establece la cantidad de fotogramas por segundo

	/*Spaceship spaceship;*/ // crea un objeto de la clase Spaceship
	Game game; // crea un objeto de la clase Game	


    while (WindowShouldClose()==false) {

		game.HandleInput(); 

		BeginDrawing(); // dibuja un canva blanco( básicamente es la pantalla principal) 
		ClearBackground(grey);  
		/*spaceship.Draw()*/; // llama al metodo Draw de la clase Spaceship
		game.Draw(); // llama al metodo Draw de la clase Game
        EndDrawing();
    }

    
    CloseWindow();

    return 0;
}
