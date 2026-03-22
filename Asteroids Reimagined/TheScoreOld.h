#pragma once
#include "Globals.h"
#include "OnScreenText.h"

class TheScoreOld : public OnScreenText
{
public:
	TheScoreOld();
	virtual ~TheScoreOld();

	bool Initialize();
	bool BeginRun();

	void Update();
	void Draw2D();

	void UpdateScore(int addToScore);

	int GetScore();
	void Reset();
	void SetHighScore(int highScore);

private:
	int Score = 0;
	int HighScore = 0;

	Font TheFontLarge = {};
	Font TheFontSmall = {};
	Font TheFontTiny = {};
};

