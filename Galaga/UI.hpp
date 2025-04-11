#include "raylib.h"
#include <vector>
#include <string>

class GameUI {
public:

	GameUI();
	~GameUI();

	void Draw();

private:
	Texture2D score;
	Texture2D life1;
	Texture2D life2;
	Texture2D level;

	std::string currentScore;

	void DrawScore();
};