#include "raylib.h"
#include <vector>
#include <string>

class GameUI {
public:

	GameUI();
	~GameUI();

	Vector2 GetScorePosition() const;
	void Draw();

private:
	Texture2D score, life1, life2, level;

	Vector2 position, position1, position2, position3;
	
	std::string currentScore;

	void DrawScore();
};