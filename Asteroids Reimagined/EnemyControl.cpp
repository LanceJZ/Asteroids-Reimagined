#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	UFOSpawnTimerID = TheManagers.EM.AddTimer();

	for (int i = 0; i < 2; i++)
	{
		UFOs[i] = DBG_NEW TheUFO();
		TheManagers.EM.AddLineModel(UFOs[i]);
	}
}

EnemyControl::~EnemyControl()
{
}

void EnemyControl::SetPlayer(ThePlayer* player)
{
	Player = player;

	for (auto &ufo : UFOs)
	{
		ufo->SetPlayer(player);
	}
}

void EnemyControl::SetRockModels(LineModelPoints rockModels[4])
{
	for (int i = 0; i < 4; i++)
	{
		RockModels[i] = rockModels[i];
	}
}

void EnemyControl::SetUFOModel(LineModelPoints model)
{
	for (auto &ufo : UFOs)
	{
		ufo->SetModel(model);
	}
}

void EnemyControl::SetShotModel(LineModelPoints model)
{
	for (auto &ufo : UFOs)
	{
		ufo->SetShotModel(model);
	}

	ShotModel = model;
}

bool EnemyControl::Initialize(Utilities* utilities)
{
	Common::Initialize(TheUtilities);

	TheManagers.EM.SetTimer(UFOSpawnTimerID, 5.0f);

	return false;
}

bool EnemyControl::BeginRun()
{
	SpawnRocks({ 0, 0, 0 }, 6, TheRock::Large);
	TheManagers.EM.ResetTimer(UFOSpawnTimerID);

	for (auto &ufo : UFOs)
	{
		ufo->Enabled = false;
	}

	return false;
}

void EnemyControl::Update()
{
	Common::Update();

	if (TheManagers.EM.TimerElapsed(UFOSpawnTimerID)) SpawnUFO();

}

void EnemyControl::SpawnRocks(Vector3 position, int count, TheRock::RockSize size)
{
	for (int rock = 0; rock < count; rock++)
	{
		bool spawnNewRock = true;
		size_t rockNumber = Rocks.size();

		for (size_t rockCheck = 0; rockCheck < rockNumber; rockCheck++)
		{
			if (!Rocks[rockCheck]->Enabled)
			{
				spawnNewRock = false;
				rockNumber = rockCheck;
				break;
			}
		}

		if (spawnNewRock)
		{
			size_t rockType = GetRandomValue(0, 3);
			Rocks.push_back(DBG_NEW TheRock());
			TheManagers.EM.AddLineModel(Rocks[rockNumber]);
			Rocks[rockNumber]->Initialize();
			Rocks[rockNumber]->BeginRun();
			Rocks[rockNumber]->SetModel((RockModels[rockType]));
			Rocks[rockNumber]->SetPlayer(Player);
		}

		Rocks[rockNumber]->Spawn(position, size);
	}
}

void EnemyControl::SpawnUFO()
{
	TheManagers.EM.ResetTimer(UFOSpawnTimerID);

	if (!Player->GameOver && !Player->Enabled) return;

	for (auto &ufo : UFOs)
	{
		if (!ufo->Enabled)
		{
			ufo->Spawn(UFOSpawnCount++);
			break;
		}
	}
}

void EnemyControl::Reset()
{
	UFOSpawnCount = 0;
}
