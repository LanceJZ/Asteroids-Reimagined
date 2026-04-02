#include "TheScore.h"

TheScore::TheScore()
{

}

TheScore::~TheScore()
{
}

bool TheScore::Initialize()
{

	return false;
}

bool TheScore::BeginRun()
{

	return false;
}

void TheScore::Update()
{

}

void TheScore::Draw2D()
{
	if (Enabled)
	{
		std::string scoreString = std::to_string(Score);

		DrawText(scoreString.c_str(),
			X - (int)(scoreString.length() * (int)(FontSize * 0.5f)), Y,
			FontSize, CurrentColor);
	}
}

void TheScore::AddToScore(int amount)
{
	Score += amount;

	if (Score > HighScore)
	{
		HighScore = Score;
	}
}

void TheScore::SetHighScore(int highScore)
{
	HighScore = highScore;
}

void TheScore::ClearScore()
{
	Score = 0;
}

void TheScore::SetColor(Color color)
{
	CurrentColor = color;
}

void TheScore::SetPosition(int x, int y)
{
	X = x;
	Y = y;
}

void TheScore::SetPosition(int x)
{
	X = x;
}

void TheScore::SetFontSize(int fontSize)
{
	FontSize = fontSize;
}

unsigned int TheScore::GetScore()
{
	return Score;
}