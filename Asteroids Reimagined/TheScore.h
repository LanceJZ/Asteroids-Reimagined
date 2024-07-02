#pragma once
#include "Globals.h"
#include "OnScreenText.h"

class TheScore : public OnScreenText
{
public:
	TheScore();
	virtual ~TheScore();

	bool Initialize(Utilities* utilities);
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
};

