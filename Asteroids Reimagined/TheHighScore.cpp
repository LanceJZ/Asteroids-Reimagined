#include "TheHighScore.h"

TheHighScore::TheHighScore()
{
	HighScoreListTimerID = TheManagers.EM.AddTimer(7.5f);
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

	Load();
	GameOver = true;

	return false;
}

void TheHighScore::Input()
{
	Common::Input();

}

void TheHighScore::Update()
{
	OnScreenText::Update();

	if (NewHighScore && GameOver)
	{
		NewHighScoreEntry();
	}

	if (GameOver && !NewHighScore)
	{
		if (TheManagers.EM.TimerElapsed(HighScoreListTimerID))
		{
			ShowHighScores = !ShowHighScores;
			TheManagers.EM.ResetTimer(HighScoreListTimerID);
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
			DrawTextEx(TheFontSmall, "Left/Right arrow keys",
				Vector2((GetScreenWidth() / 2) - 150,
				(GetScreenHeight() / 2) - 280), 30, 0, color);
			DrawTextEx(TheFontSmall, "to change letter, down arrow key",
				Vector2((GetScreenWidth() / 2) - 230,
				(GetScreenHeight() / 2) - 200), 30, 0, color);
			DrawTextEx(TheFontSmall, "to go to next letter.",
				Vector2((GetScreenWidth() / 2) - 130,
				(GetScreenHeight() / 2) - 120), 30, 0, color);
			DrawTextEx(TheFontSmall, "Enter to submit.",
				Vector2((GetScreenWidth() / 2) - 100,
				(GetScreenHeight() / 2) - 30), 30, 0, color);
			DrawTextEx(TheFontLarge, const_cast<char*>(HighScoreEntryText.c_str()),
				Vector2((GetScreenWidth() / 2) - 70,
				(GetScreenHeight() / 2) + 100), 60, 0, color);
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
					Vector2((GetScreenWidth() / 2) - 170,
					(GetScreenHeight() / 2) - 100), 60, 0, color);
				DrawTextEx(TheFontLarge, "ONE COIN ONE PLAY",
					Vector2((GetScreenWidth() / 2) - 290,
					(GetScreenHeight() / 2) + 60), 60, 0, color);
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
	int space = 30;
	int font = 30;
	int start = 150;
	int center = GetScreenWidth() / 2;

	DrawTextEx(TheFontSmall, "HIGH SCORE LIST",
		Vector2((center - 120), start - 50), font, 0, color);

	for (int i = 0; i < 20; i++)
	{
		if (i < 10)
		{
			name = HighScoreList[i].Name;
		}
		else
		{
			name = " ";
		}

		score = std::to_string(HighScoreList[i].Score);

		DrawTextEx(TheFontSmall, const_cast<char*>(name.c_str()),
			Vector2(center - (space + 70),
			start + (space * i)), font, 0, color);
		DrawTextEx(TheFontSmall, const_cast<char*>(score.c_str()),
			Vector2(center + space,
			start + (space * i)), font, 0, color);
	}

	DrawTextEx(TheFontSmall, "PRESS N OR START TO START NEW GAME.",
		Vector2((center - 280), (GetScreenHeight()) - 100), font, 0, color);
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

	for (auto character : HighScoreListRaw)
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
				number.append(1, character);
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
	for (int rank = 0; rank < 10; rank++)
	{
		if (score > HighScoreList[rank].Score)
		{
			if (rank < 49)
			{
				ScoreList oldScores[50];

				for (int oldRank = rank; oldRank < 50; oldRank++)
				{
					oldScores[oldRank] = HighScoreList[oldRank];
				}

				for (int newRank = rank; newRank < 49; newRank++)
				{
					HighScoreList[newRank + 1] = oldScores[newRank];
				}
			}

			HighScoreList[rank].Name = "XXXXX";
			HighScoreList[rank].Score = score;
			HighScoreEntryText = "_____";
			NewHighScoreRank = rank;
			NewHighScore = true;

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