#include "GameLogic.h"

GameLogic::GameLogic()
{

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
	Common::Initialize(utilities);

	AdjustedFieldSize = Vector2Multiply(FieldSize, { 0.5f, 0.5f });

	return false;
}

bool GameLogic::BeginRun()
{
	Common::BeginRun();

	return false;
}

void GameLogic::Update()
{
	Common::Update();

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
	}
}

void GameLogic::NewGame()
{
}
