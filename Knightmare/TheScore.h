#pragma once
#include <string>
#include "raylib.h"

class TheScore
{
public:
	TheScore();
	virtual ~TheScore();

	bool Enabled = true;

	bool Initialize();
	bool BeginRun();

	void Update();
	void Draw2D();

	void AddToScore(int amount);
	void SetHighScore(int highScore);
	void ClearScore();
	void SetColor(Color color);
	void SetPosition(int x, int y);
	void SetPosition(int x);
	void SetFontSize(int fontSize);

	unsigned int GetScore();

private:
	unsigned int Score = 0;
	unsigned int HighScore = 10000;

	int X = 100 - 45;
	int Y = 75;
	int FontSize = 45;

	Color CurrentColor = RAYWHITE;
};

