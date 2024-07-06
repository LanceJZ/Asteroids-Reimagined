#pragma once
#include "Globals.h"
#include "Utilities.h"
#include "ThePlayer.h"
#include "EnemyControl.h"
#include "PowerUp.h"

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

	void SetPowerUpModel(LineModelPoints model);

	void SetPowerUpSound(Sound sound);
	void SetPickUpSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();
	void GameInput();

	void SpawnPowerUp(Vector3 position);

private:
	size_t ExplodeTimerID = 0;
	Vector2 AdjustedFieldSize = {};

	LineModelPoints PlayerModel = {};
	LineModelPoints PowerUpModel = {};

	Sound PowerUpSound = {};
	Sound PickUpSound = {};

	std::vector <LineModel*> PlayerShipModels;
	std::vector <PowerUp*> PowerUps;

	Entity* PlayerClear = {};

	ThePlayer* Player = {};
	EnemyControl* Enemies = {};

	void PlayerShipDisplay();
	void NewGame();
	bool CheckPlayerClear();
};

