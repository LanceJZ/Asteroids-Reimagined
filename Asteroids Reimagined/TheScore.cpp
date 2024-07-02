#include "TheScore.h"

TheScore::TheScore()
{

}

TheScore::~TheScore()
{
}

bool TheScore::Initialize(Utilities* utilities)
{
	OnScreenText::Initialize(TheUtilities);

	return false;
}

bool TheScore::BeginRun()
{

	return false;
}

void TheScore::Update()
{
	OnScreenText::Update();

}

void TheScore::Draw2D()
{
	OnScreenText::Draw2D();

	DrawText(const_cast<char*>(std::to_string(Score).c_str()), 200, 5, 45, WHITE);
	DrawText(const_cast<char*>(std::to_string(HighScore).c_str()),
		GetScreenWidth() / 2, 4, 20, WHITE);
	DrawText("(C) 1980 ATARI INC", (GetScreenWidth() / 2) - 35,
		GetScreenHeight() - 12, 8, WHITE);
}

void TheScore::UpdateScore(int addToScore)
{
	Score += addToScore;

	if (Score > HighScore)
	{
		HighScore = Score;
	}
}

int TheScore::GetScore()
{
	return Score;
}

void TheScore::Reset()
{
	Score = 0;
}

void TheScore::SetHighScore(int highScore)
{
	HighScore = highScore;
}
