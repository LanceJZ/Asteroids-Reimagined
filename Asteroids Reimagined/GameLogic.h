#pragma once
#include "Globals.h"
#include "Utilities.h"
#include "ThePlayer.h"
#include "EnemyControl.h"

enum GameState
{
	PlayerHit,
	Over,
	InPlay,
	Pause,
	HighScores,
	MainMenu
};

class GameLogic : public Common
{
public:
	GameLogic();
	virtual ~GameLogic();

	bool PlayBackgroundMusic = true;
	GameState State = MainMenu;

	void SetPlayer(ThePlayer* player);
	void SetEnemies(EnemyControl* enemies);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();
	void GameInput();

private:
	Vector2 AdjustedFieldSize = {};

	ThePlayer* Player = {};
	EnemyControl* Enemies = {};

	void NewGame();
};

