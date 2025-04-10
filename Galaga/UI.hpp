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
	Texture2D numberSprites[10];
	std::string currentScore;

	void DrawScore();
};