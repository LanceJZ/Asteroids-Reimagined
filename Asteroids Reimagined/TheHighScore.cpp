#include "TheHighScore.h"

TheHighScore::TheHighScore()
{
	HighScoreListTimerID = Managers.EM.AddTimer(7.5f);
}

TheHighScore::~TheHighScore()
{
}
bool TheHighScore::Initialize(Utilities* utilities)
{
	OnScreenText::Initialize(TheUtilities);

	return false;
}

bool TheHighScore::BeginRun()
{
	TheFontSmall = LoadFontEx("font/asteroids-display.otf", 30, 0, 250);
	TheFontLarge = LoadFontEx("font/asteroids-display.otf", 60, 0, 250);

	GameOver = true;

	return false;
}

void TheHighScore::Input()
{
	Common::Input();

}

void TheHighScore::FixedUpdate()
{
	OnScreenText::FixedUpdate();

	if (NewHighScore && GameOver)
	{
		NewHighScoreEntry();
	}

	if (GameOver && !NewHighScore)
	{
		if (Managers.EM.TimerElapsed(HighScoreListTimerID))
		{
			ShowHighScores = !ShowHighScores;
			Managers.EM.ResetTimer(HighScoreListTimerID);
		}
	}
}

void TheHighScore::Draw2D()
{
	OnScreenText::Draw2D();

	if (GameOver)
	{
		if (NewHighScore)
		{
			DrawTextEx(TheFontSmall, "Left or Right arrow keys",
				Vector2((float)(GetScreenWidth() / 2) - 150,
				(float)(GetScreenHeight() / 2) - 280), 30, 0, color);
			DrawTextEx(TheFontSmall, "to change letter. Down arrow key",
				Vector2((float)(GetScreenWidth() / 2) - 230,
				(float)(GetScreenHeight() / 2) - 200), 30, 0, color);
			DrawTextEx(TheFontSmall, "to go to next letter.",
				Vector2((float)(GetScreenWidth() / 2) - 130,
				(float)(GetScreenHeight() / 2) - 120), 30, 0, color);
			DrawTextEx(TheFontSmall, "Enter to submit.",
				Vector2((float)(GetScreenWidth() / 2) - 100,
				(float)(GetScreenHeight() / 2) - 30), 30, 0, color);
			DrawTextEx(TheFontLarge, const_cast<char*>(HighScoreEntryText.c_str()),
				Vector2((float)(GetScreenWidth() / 2) - 70,
				(float)(GetScreenHeight() / 2) + 100), 60, 0, color);
		}
		else
		{
			if (ShowHighScores)
			{
				DisplayHighScoreList();
			}
			else
			{
				DrawTextEx(TheFontLarge, "GAME OVER",
					Vector2((float)(GetScreenWidth() / 2) - 170.0f,
					(float)(GetScreenHeight() / 2) - 100.0f), 60.0f, 0, color);
				DrawTextEx(TheFontLarge, "ONE COIN ONE PLAY",
					Vector2((float)(GetScreenWidth() / 2) - 290.0f,
					(float)(GetScreenHeight() / 2) + 60.0f), 60.0f, 0, color);
			}
		}
	}
}

void TheHighScore::TheGameIsOver(int playerScore)
{
	GameOver = true;
	CheckForNewHighScore(playerScore);
}

void TheHighScore::Reset()
{
	GameOver = false;
}

int TheHighScore::GetHighScore()
{
	return HighestScore;
}

void TheHighScore::DisplayHighScoreList()
{
	std::string name = "";
	std::string score = "";
	float space = 30.0f;
	float font = 30.0f;
	float start = 150.0f;
	float center = (float)GetScreenWidth() / 2.0f;

	DrawTextEx(TheFontSmall, "HIGH SCORE LIST",
		Vector2((center - 120), start - 50), font, 0, color);

	for (int i = 0; i < 20; i++)
	{
		name = HighScoreList[i].Name;
		score = std::to_string(HighScoreList[i].Score);

		DrawTextEx(TheFontSmall, const_cast<char*>(name.c_str()),
			Vector2(center - (space + 70.0f),
			start + (space * i)), font, 0, color);
		DrawTextEx(TheFontSmall, const_cast<char*>(score.c_str()),
			Vector2(center + space,
			start + (space * i)), font, 0, color);
	}

	DrawTextEx(TheFontSmall, "PRESS N OR START TO START NEW GAME.",
		Vector2((center - 280.0f), (float)(GetScreenHeight()) - 100.0f), font, 0, color);
}

void TheHighScore::NewHighScoreEntry()//TODO: Make so it can use controller as well.
{
	if (IsKeyPressed(KEY_DOWN))
	{
		HighScoreSelectedLetter++;

		if (HighScoreSelectedLetter > 4)
		{
			HighScoreList[NewHighScoreRank].Name = HighScoreEntryText;
			NewHighScore = false;
			ConvertScoreListToString();
			Save();
			return;
		}
		else
		{
			HighScoreEntryText[HighScoreSelectedLetter] = (char)65;
		}
	}
	else if (IsKeyPressed(KEY_LEFT))
	{
		HighScoreEntryText[HighScoreSelectedLetter]--;

		if (HighScoreEntryText[HighScoreSelectedLetter] < (char)32)
		{
			HighScoreEntryText[HighScoreSelectedLetter] = (char)90;
		}
	}
	else if (IsKeyPressed(KEY_RIGHT))
	{
		HighScoreEntryText[HighScoreSelectedLetter]++;

		if (HighScoreEntryText[HighScoreSelectedLetter] > (char)90)
		{
			HighScoreEntryText[HighScoreSelectedLetter] = (char)32;
		}
	}
	else if (IsKeyPressed(KEY_ENTER))
	{
		HighScoreList[NewHighScoreRank].Name = HighScoreEntryText;
		NewHighScore = false;
		ConvertScoreListToString();
		Save();
		return;
	}
}

void TheHighScore::MakeNewScoreList()
{
	for (int i = 0; i < MaxScoreListLength; i++)
	{
		HighScoreList[i].Name = "AAAAA";
		HighScoreList[i].Score = 1000;
	}

	ConvertScoreListToString();
	Save();
}

void TheHighScore::ConvertScoreListToString()
{
	HighScoreListRaw = "";

	for (const auto& score : HighScoreList)
	{
		HighScoreListRaw += score.Name;
		HighScoreListRaw += ":";
		HighScoreListRaw += std::to_string(score.Score);
		HighScoreListRaw += ",";
	}
}

void TheHighScore::ConvertRawScoreListToArray()
{
	int listNumber = 0;
	bool isLetter = true;
	std::string number = "";

	for (const auto& character : HighScoreListRaw)
	{
		if (isLetter)
		{
			if (character !=  58) //58 for colon.
			{
				HighScoreList[listNumber].Name.append(1, character);
			}
			else
			{
				isLetter = false;
			}
		}
		else
		{
			if (character != 44) //44 for comma.
			{
				if (character > 47 && character < 58)
				{
					number.append(1, character);
				}
				else
				{
					MakeNewScoreList();
					return;
				}
			}
			else
			{
				HighScoreList[listNumber].Score = stoi(number);
				isLetter = true;
				listNumber++;
				number = "";
			}
		}
	}
}

void TheHighScore::CheckForNewHighScore(int score)
{
	for (int rank = 0; rank < MaxScoreListLength; rank++)
	{
		if (score > HighScoreList[rank].Score)
		{
			if (rank < MaxScoreListLength - 1)
			{
				ScoreList oldScores[MaxScoreListLength];

				for (int oldRank = rank; oldRank < MaxScoreListLength; oldRank++)
				{
					oldScores[oldRank] = HighScoreList[oldRank];
				}

				for (int newRank = rank; newRank < MaxScoreListLength - 1; newRank++)
				{
					HighScoreList[newRank + 1] = oldScores[newRank];
				}
			}

			HighScoreList[rank].Name = "XXXXX";
			HighScoreList[rank].Score = score;
			HighScoreEntryText = "_____";
			NewHighScoreRank = rank;
			NewHighScore = true;
			HighScoreEntryText[HighScoreSelectedLetter] = (char)65;

			break;
		}
	}
}

void TheHighScore::Load()
{
	if (FileExists("HighScoreList"))
	{
		HighScoreListRaw = LoadFileText("HighScoreList");
		ConvertRawScoreListToArray();
		HighestScore = HighScoreList[0].Score;
	}
	else
	{
		MakeNewScoreList();
	}
}

void TheHighScore::Save()
{
	SaveFileText("HighScoreList", const_cast<char*>(HighScoreListRaw.c_str()));
}