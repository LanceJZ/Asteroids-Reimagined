#pragma once
#include "Globals.h"
#include "OnScreenText.h"

class TheHighScore : public OnScreenText
{
	struct ScoreList
	{
		std::string Name = "";
		int Score = 0;
	};

public:
	TheHighScore();
	virtual ~TheHighScore();

	bool Initialize(Utilities* utilities);
	bool BeginRun();
	void Load();

	void Input();
	void FixedUpdate();
	void Draw2D();

	void TheGameIsOver(int playerScore);
	void Reset();
	int GetHighScore();

private:
	size_t HighScoreListTimerID = 0;

	bool GameOver = false;
	bool NewHighScore = false;
	bool ShowHighScores = false;

	int NewHighScoreRank = 0;
	int HighScoreSelectedLetter = 0;
	int HighestScore = 0;

	static const size_t MaxScoreListLength = 50;
	static const size_t MaxHighScoreListNameLength = 5;

	Color color = WHITE;

	std::string HighScoreEntryText = "";
	std::string HighScoreListRaw = "";

	Font TheFontLarge = {};
	Font TheFontSmall = {};

	ScoreList HighScoreList[MaxScoreListLength];

	void DisplayHighScoreList();
	void NewHighScoreEntry();
	void MakeNewScoreList();
	void ConvertScoreListToString();
	void ConvertRawScoreListToArray();
	void CheckForNewHighScore(int score);
	void Save();
};

