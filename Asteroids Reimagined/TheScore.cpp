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
	TheFontTiny = LoadFontEx("font/asteroids-display.otf", 16, 0, 250);
	TheFontSmall = LoadFontEx("font/asteroids-display.otf", 30, 0, 250);
	TheFontLarge = LoadFontEx("font/asteroids-display.otf", 40, 0, 250);

	return false;
}

void TheScore::Update()
{
	OnScreenText::Update();

}

void TheScore::Draw2D()
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
