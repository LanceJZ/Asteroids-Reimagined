#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "EnemyControl.h"
#include "PowerUp.h"
#include "TheHighScore.h"

enum GameState
{
	PlayerHit,
	Ended,
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

	float GameVolume = 1.0f;

	GameState State = MainMenu;

	void SetPlayer(ThePlayer* player);
	void SetAntiPlayer(TheAntiPlayer* player);
	void SetEnemies(EnemyControl* enemies);

	void SetPowerUpModel(std::vector<Vector3> model);

	void SetPowerUpSound(Sound sound);
	void SetPickUpSound(Sound sound);

	bool Initialize();
	bool BeginRun();
	bool Load();

	void FixedUpdate();
	void GameInput();

	void SpawnPowerUp(Vector3 position);

private:
	bool GameEnded = false;
	size_t ExplodeTimerID = 0;
	Vector2 AdjustedFieldSize = {};

	std::vector<Vector3> PlayerModel = {};
	std::vector<Vector3> PowerUpModel = {};

	Sound PowerUpSound = {};
	Sound PickUpSound = {};

	std::vector <LineModel*> PlayerShipModels;
	std::vector <PowerUp*> PowerUps;

	Entity* PlayerClear = {};

	ThePlayer* Player = nullptr;
	TheAntiPlayer* AntiPlayer = nullptr;
	EnemyControl* Enemies = nullptr;
	TheHighScore* HighScores = nullptr;

	void PlayerShipDisplay();
	void AddPlayerShipModels(int number);
	void NewGame();
	bool CheckPlayerClear();
};

