#include "GameLogic.h"

GameLogic::GameLogic()
{
	TheManagers.EM.AddEntity(PlayerClear = DBG_NEW Entity());
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

	return true;
}

void GameLogic::Update()
{
	Common::Update();

	if (Player->GameOver)
	{
		State = MainMenu;
	}

	if (!Player->Enabled && State == InPlay)
	{
		PlayerClear->Enabled = true;

		if (CheckPlayerClear())
		{
			Player->Spawn();
			PlayerShipDisplay();
		}
	}
	else
	{
		PlayerClear->Enabled = false;

		if (Player->NewLife)
		{
			PlayerShipDisplay();
			Player->NewLife = false;
		}
	}
}

void GameLogic::GameInput()
{
	if (State == MainMenu)
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

	if (State == InPlay)
	{
		if (IsGamepadAvailable(0))
		{
			if (IsGamepadButtonPressed(0, 13)) //Menu Button
			{
				State = Pause;
			}

			if (IsGamepadButtonPressed(0, 8)) //X button
			{
				PlayBackgroundMusic = !PlayBackgroundMusic;
			}
		}

		if (IsKeyPressed(KEY_M))
		{
			PlayBackgroundMusic = !PlayBackgroundMusic;
		}


		if (IsKeyPressed(KEY_P))
		{
			State = Pause;
		}

		if (IsKeyPressed(KEY_B))
		{
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

void GameLogic::PlayerShipDisplay()
{
	Vector2 location = { (-GetScreenWidth() / 2.25f) + Player->Radius,
		(-GetScreenHeight() / 2) + Player->Radius * 2.0f + 30.0f };

	if (Player->Lives > PlayerShipModels.size())
	{
		PlayerShipModels.push_back(DBG_NEW LineModel());
		TheManagers.EM.AddLineModel(PlayerShipModels.back());
		PlayerShipModels.back()->SetModel(PlayerModel);
		PlayerShipModels.back()->Initialize(TheUtilities);
		PlayerShipModels.back()->RotationZ = PI / 2 + PI;
		PlayerShipModels.back()->Scale = 0.8f;
	}

	for (int i = 0; i < PlayerShipModels.size(); i++)
	{
		PlayerShipModels[i]->Enabled = false;
		PlayerShipModels[i]->Position = { location.x, location.y, 0.0f };
		location.x += Player->Radius * 2.0f;
	}

	if (Player->Lives > PlayerShipModels.size())
	{
		return;
	}

	for (int i = 0; i < Player->Lives; i++)
	{
		PlayerShipModels.at(i)->Enabled = true;
	}
}

void GameLogic::NewGame()
{
	Player->NewGame();
	Enemies->NewGame();

	State = InPlay;

	for (int i = 0; i < Player->Lives; i++)
	{
		PlayerShipDisplay();
	}
}

bool GameLogic::CheckPlayerClear()
{
	for (auto& rock : Enemies->Rocks)
	{
		if (rock->Enabled && rock->CirclesIntersect(*PlayerClear))
		{
			return false;
		}
	}

	for (auto& ufo : Enemies->UFOs)
	{
		if (ufo->Enabled)
		{
			return false;
		}

		for (auto& Shot : ufo->Shots)
		{
			if (Shot->Enabled)
			{
				return false;
			}
		}
	}

	if (Enemies->EnemyOne->Enabled || Enemies->EnemyTwo->Enabled
		|| Enemies->EnemyOne->Missile->Enabled)
	{
		return false;
	}

	if (Enemies->DeathStar->CirclesIntersect(*PlayerClear))
	{
		return false;
	}

	return true;
}
