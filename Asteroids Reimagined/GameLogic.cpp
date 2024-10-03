#include "GameLogic.h"

GameLogic::GameLogic()
{
	Managers.EM.AddEntity(PlayerClear = DBG_NEW Entity());
	Managers.EM.AddOnScreenText(HighScores = DBG_NEW TheHighScore());

	ExplodeTimerID = Managers.EM.AddTimer(3.1f);
}

GameLogic::~GameLogic()
{
}

void GameLogic::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void GameLogic::SetEnemies(EnemyControl* enemies)
{
	Enemies = enemies;
}

void GameLogic::SetPowerUpModel(LineModelPoints model)
{
	PowerUpModel = model;
}

void GameLogic::SetPowerUpSound(Sound sound)
{
	PowerUpSound = sound;
}

void GameLogic::SetPickUpSound(Sound sound)
{
	PickUpSound = sound;
}

bool GameLogic::Initialize(Utilities* utilities)
{
	Common::Initialize(TheUtilities);

	AdjustedFieldSize = Vector2Multiply(FieldSize, { 0.5f, 0.5f });

	PlayerClear->Radius = 140.0f;

	State = MainMenu;

	return false;
}

bool GameLogic::BeginRun()
{
	Common::BeginRun();

	PlayerModel = Player->GetLineModel();
	PlayerClear->Enabled = false;
	GameEnded = true;
	Player->SetHighScore(HighScores->GetHighScore());

	return true;
}

void GameLogic::Update()
{
	Common::Update();

	if (State == MainMenu)
	{
		if (!GameEnded)
		{
			HighScores->TheGameIsOver(Player->GetScore());
			GameEnded = true;
		}

		return;
	}

	if (Player->GameOver)
	{
		State = MainMenu;
		PlayerClear->Enabled = false;
		Player->Destroy();
		return;
	}

	if (State == InPlay)
	{
		if (Player->GetBeenHit())
		{
			Managers.EM.ResetTimer(ExplodeTimerID);
			Player->Destroy();
		}

		if (!Player->Enabled && !Player->GetBeenHit() &&
			Managers.EM.TimerElapsed(ExplodeTimerID))
		{
			PlayerClear->Enabled = true;

			if (CheckPlayerClear() || IsKeyPressed(KEY_ENTER) ||
				(IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)))
			{
				Player->Spawn();
				PlayerClear->Enabled = false;
				PlayerShipDisplay();
			}
		}

		if (Player->NewLife)
		{
			PlayerShipDisplay();
			Player->NewLife = false;
		}
	}
	else
	{
		PlayerClear->Enabled = false;
	}

	if (Player->Enabled)
	{
		if (Enemies->SpawnPowerUp)
		{
			Enemies->SpawnPowerUp = false;
			SpawnPowerUp(Enemies->PowerUpSpawnPosition);
		}
	}
}

void GameLogic::GameInput()
{
	if (State == Pause)
	{
		if (IsKeyPressed(KEY_P) || (IsGamepadAvailable(0)
			&& IsGamepadButtonPressed(0, 13)))
		{
			State = InPlay;
			Player->Paused = false;
		}

		return;
	}
	else if (State == MainMenu)
	{
		if (IsGamepadAvailable(0))
		{
			if (IsGamepadButtonPressed(0, 15))//Start button
			{
				NewGame();
			}
		}

		if (IsKeyPressed(KEY_N))
		{
			NewGame();
		}

		if (IsKeyPressed(KEY_D))
		{

		}
	}
	else if (State == InPlay)
	{
		if (!Player->Enabled)
		{
		}

		if (IsKeyPressed(KEY_M) || (IsGamepadAvailable(0) &&
			IsGamepadButtonPressed(0, 8)))
		{
			PlayBackgroundMusic = !PlayBackgroundMusic;
		}


		if (IsKeyPressed(KEY_P) || (IsGamepadAvailable(0) &&
			IsGamepadButtonPressed(0, 13)))
		{
			State = Pause;
			Player->Paused = true;
		}

#if DEBUG
		if (IsKeyPressed(KEY_F5))
		{
			Enemies->NextWave();
		}

		if (IsKeyPressed(KEY_F6))
		{
			Player->ExtraLife();
		}
#endif
	}
}

void GameLogic::SpawnPowerUp(Vector3 position)
{
	if (!Player->Enabled)
	{
		PlaySound(PowerUpSound);
	}

	bool spawnNewPowerUp = true;
	size_t powerUpNumber = PowerUps.size();

	for (size_t powerUpCheck = 0; powerUpCheck < powerUpNumber; powerUpCheck++)
	{
		if (!PowerUps.at(powerUpCheck)->Enabled)
		{
			spawnNewPowerUp = false;
			powerUpNumber = powerUpCheck;
			break;
		}
	}

	if (spawnNewPowerUp)
	{
		PowerUps.push_back(DBG_NEW PowerUp());
		Managers.EM.AddEntity(PowerUps.back());
		PowerUps.back()->SetPlayer(Player);
		PowerUps.back()->SetEnemy(Enemies);
		PowerUps.back()->SetModel(PowerUpModel);
		PowerUps.back()->SetPickUpSound(PickUpSound);
		PowerUps.back()->Initialize(TheUtilities);
		PowerUps.back()->BeginRun();
	}

	PowerUps.at(powerUpNumber)->Spawn(position);
}

void GameLogic::PlayerShipDisplay()
{
	Vector2 location = { (-GetScreenWidth() / 2.05f) + Player->Radius,
		(-GetScreenHeight() / 2) + Player->Radius * 2.0f + 30.0f };

	if (Player->Lives > (int)PlayerShipModels.size())
	{
		AddPlayerShipModels(Player->Lives - (int)PlayerShipModels.size());
	}

	if (Player->Lives > (int)PlayerShipModels.size())
	{
		return;
	}

	for (const auto& model : PlayerShipModels)
	{
		model->Enabled = false;
		model->Position = { location.x, location.y, 0.0f };
		location.x += Player->Radius * 2.0f;
	}

	for (int i = 0; i < Player->Lives; i++)
	{
		PlayerShipModels.at(i)->Enabled = true;
	}
}

void GameLogic::AddPlayerShipModels(int number)
{
	for (int i = 0; i < number; i++)
	{
		PlayerShipModels.push_back(DBG_NEW LineModel());
		Managers.EM.AddLineModel(PlayerShipModels.back());
		PlayerShipModels.back()->SetModel(PlayerModel);
		PlayerShipModels.back()->Initialize(TheUtilities);
		PlayerShipModels.back()->RotationZ = PI / 2 + PI;
		PlayerShipModels.back()->Scale = 0.8f;
		PlayerShipModels.back()->Radius = 0.0f;
	}
}

void GameLogic::NewGame()
{
	Player->NewGame();
	Enemies->NewGame();
	HighScores->Reset();

	State = InPlay;
	GameEnded = false;

	PlayerShipDisplay();

	for (const auto& powerUp : PowerUps)
	{
		powerUp->Enabled = false;
	}
}

bool GameLogic::CheckPlayerClear()
{
	for (const auto& rock : Enemies->Rocks)
	{
		if (rock->Enabled && rock->CirclesIntersect(*PlayerClear))
		{
			return false;
		}
	}

	for (const auto& ufo : Enemies->UFOs)
	{
		if (ufo->Enabled && ufo->CirclesIntersect(*PlayerClear))
		{
			return false;
		}

		for (const auto& Shot : ufo->Shots)
		{
			if (Shot->Enabled && Shot->CirclesIntersect(*PlayerClear))
			{
				return false;
			}
		}
	}

	if (Enemies->Boss->Enabled)
	{
		if (Enemies->Boss->CirclesIntersect(*PlayerClear)) return false;

		if (PlayerClear->CirclesIntersect(
			Enemies->Boss->FireShotAtPlayerArea->GetWorldPosition(),
				Enemies->Boss->FireShotAtPlayerArea->Radius))
		{
			return false;
		}

		for (const auto& shot : Enemies->Boss->Shots)
		{
			if (shot->Enabled && shot->CirclesIntersect(*PlayerClear))
			{
				return false;
			}
		}

		for (const auto& turret : Enemies->Boss->Turrets)
		{
			for (const auto& shot : turret->Shots)
			{
				if (shot->Enabled && shot->CirclesIntersect(*PlayerClear))
				{
					return false;
				}
			}
		}
	}

	if (Enemies->DeathStar->CirclesIntersect(*PlayerClear))
	{
		return false;

	}

	for (const auto& fighterPair : Enemies->DeathStar->FighterPairs)
	{
		if (fighterPair->CirclesIntersect(*PlayerClear))
		{
			return false;
		}

		for (const auto& fighter : fighterPair->Fighters)
		{
			if (fighter->CirclesIntersect(*PlayerClear))
			{
				return false;
			}
		}
	}

	for (const auto& enemy : Enemies->EnemyOnes)
	{
		if (enemy->CirclesIntersect(*PlayerClear))
		{
			return false;
		}

		for (const auto& missile : enemy->Missiles)
		{
			if (missile->CirclesIntersect(*PlayerClear))
			{
				return false;
			}
		}
	}

	for (const auto& enemy : Enemies->EnemyTwos)
	{
		if (enemy->CirclesIntersect(*PlayerClear))
		{
			return false;
		}

		for (const auto& mine : enemy->Mines)
		{
			if (mine->CirclesIntersect(*PlayerClear))
			{
				return false;
			}
		}
	}

	return true;
}
