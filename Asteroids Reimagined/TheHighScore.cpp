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
			DrawText("Left/Right",
				(GetScreenWidth() / 2) - 80, (GetScreenHeight() / 2) - 250, 40, color);
			DrawText("arrow keys",
				(GetScreenWidth() / 2) - 90, (GetScreenHeight() / 2) - 200, 40, color);
			DrawText("to change letter,",
				(GetScreenWidth() / 2) - 130, (GetScreenHeight() / 2) - 150, 40, color);
			DrawText("down arrow key",
				(GetScreenWidth() / 2) - 120, (GetScreenHeight() / 2) - 100, 40, color);
			DrawText("to chose letter.",
				(GetScreenWidth() / 2) - 120, (GetScreenHeight() / 2) - 50, 40, color);
			DrawText(const_cast<char*>(HighScoreEntryText.c_str()), (GetScreenWidth() / 2) - 30,
				(GetScreenHeight() / 2) + 130, 60, color);
		}
		else
		{
			if (ShowHighScores)
			{
				DisplayHighScoreList();
			}
			else
			{
				DrawText("GAME OVER", GetScreenWidth() / 2 - 160,
					GetScreenHeight() / 2 - 60, 60, color);
				DrawText("ONE COIN ONE PLAY", GetScreenWidth() / 2 - 290,
					GetScreenHeight() / 2 + 60, 60, color);
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
	int font = 20;
	int start = 150;
	int center = GetScreenWidth() / 2;

	DrawText("Press N or Start to start new game.",
		center - 350, (GetScreenHeight()) - 100, 40, color);


	DrawText("High Scores", (int)((center - space) * 2.25f), start - 20, font, color);

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

		DrawText(const_cast<char*>(name.c_str()), center - (space + 10),
			start + (space * i), font, color);
		DrawText(const_cast<char*>(score.c_str()), center + (space + 10),
			start + (space * i), font, color);
	}
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