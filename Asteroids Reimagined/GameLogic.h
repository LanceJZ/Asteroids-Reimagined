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
	size_t ExplodeTimerID = 0;
	Vector2 AdjustedFieldSize = {};

	LineModelPoints PlayerModel;

	std::vector <LineModel*> PlayerShipModels;

	Entity* PlayerClear;

	ThePlayer* Player = {};
	EnemyControl* Enemies = {};

	void PlayerShipDisplay();
	void NewGame();
	bool CheckPlayerClear();
};

