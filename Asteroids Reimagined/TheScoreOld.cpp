#include "TheScoreOld.h"

TheScoreOld::TheScoreOld()
{

}

TheScoreOld::~TheScoreOld()
{
}

bool TheScoreOld::Initialize()
{
	OnScreenText::Initialize();

	return false;
}

bool TheScoreOld::BeginRun()
{
	TheFontTiny = LoadFontEx("font/asteroids-display.otf", 16, 0, 250);
	TheFontSmall = LoadFontEx("font/asteroids-display.otf", 30, 0, 250);
	TheFontLarge = LoadFontEx("font/asteroids-display.otf", 40, 0, 250);

	return false;
}

void TheScoreOld::Update()
{
	OnScreenText::Update();

}

void TheScoreOld::Draw2D()
{
	OnScreenText::Draw2D();

	DrawTextEx(TheFontLarge, const_cast<char*>(std::to_string(Score).c_str()),
		Vector2(200.0f, 5.0f), 40.0f, 0, WHITE);

	DrawTextEx(TheFontSmall, const_cast<char*>(std::to_string(HighScore).c_str()),
		Vector2((float)(GetScreenWidth() / 2), 5.0f), 30.0f, 0, WHITE);

	DrawTextEx(TheFontTiny, "C  1 9 8 0  A T A R I  I N C",
		Vector2((float)((GetScreenWidth() / 2) - 70),
		(float)(GetScreenHeight() - 20)), 16.0f, 0, WHITE);
}

void TheScoreOld::UpdateScore(int addToScore)
{
	Score += addToScore;

	if (Score > HighScore)
	{
		HighScore = Score;
	}
}

int TheScoreOld::GetScore()
{
	return Score;
}

void TheScoreOld::Reset()
{
	Score = 0;
}

void TheScoreOld::SetHighScore(int highScore)
{
	HighScore = highScore;
}
