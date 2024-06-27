#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	UFOSpawnTimerID = TheManagers.EM.AddTimer(10.0f);
	DeathStarSpawnTimerID = TheManagers.EM.AddTimer(5.0f);
	EnemyOneSpawnTimerID = TheManagers.EM.AddTimer(3.0f);

	for (int i = 0; i < 2; i++)
	{
		TheManagers.EM.AddLineModel(UFOs[i] = DBG_NEW TheUFO());
	}

	TheManagers.EM.AddEntity(DeathStar = DBG_NEW TheDeathStar());
	TheManagers.EM.AddLineModel(EnemyOne = DBG_NEW Enemy1());
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

	DeathStar->SetPlayer(player);
	EnemyOne->SetPlayer(player);
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

void EnemyControl::SetWedgeModel(LineModelPoints model)
{
	DeathStar->SetWedgeModel(model);
}

void EnemyControl::SetEnemy1Model(LineModelPoints model)
{
	EnemyOne->SetModel(model);
}

void EnemyControl::SetEnemyMissileModel(LineModelPoints model)
{
	EnemyOne->SetMissileModel(model);
}

bool EnemyControl::Initialize(Utilities* utilities)
{
	Common::Initialize(TheUtilities);

	for (auto &ufo : UFOs)
	{
		ufo->Initialize(TheUtilities);
	}

	DeathStar->Initialize(TheUtilities);

	return false;
}

bool EnemyControl::BeginRun()
{

	DeathStar->BeginRun();

	for (auto& ufo : UFOs)
	{
		ufo->SetRocks(Rocks);
	}

	Reset();

	return false;
}

void EnemyControl::Update()
{
	Common::Update();

	if (TheManagers.EM.TimerElapsed(UFOSpawnTimerID)) SpawnUFO();

	if (NoMoreRocks)
	{
		SpawnRocks({ 0, 0, 0 }, RockSpawnCount++, TheRock::Large);
		if (SpawnedDeathStar) DeathStar->NewWaveStart();
		Wave++;
	}

	for (auto& ufo : UFOs)
	{
		ufo->DeathStarActive = DeathStar->Enabled;
	}

	if (SpawnedDeathStar) CheckDeathStarStatus();
	else if (Wave > 2 && RockCount < 6)
	{
		if (TheManagers.EM.TimerElapsed(DeathStarSpawnTimerID))
		{
			SpawnDeathStar();
		}
	}

	CheckRockCollisions();

	if (TheManagers.EM.TimerElapsed(EnemyOneSpawnTimerID))
	{
		TheManagers.EM.ResetTimer(EnemyOneSpawnTimerID);

		if (!EnemyOne->Enabled)
		{
			EnemyOne->Spawn({ 0, 0, 0 });
		}
	}

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

void EnemyControl::SpawnDeathStar()
{
	DeathStar->Spawn({ -500, -400, 0 });
	DeathStar->SetUFO(UFOs);
	SpawnedDeathStar = true;
}

void EnemyControl::CheckDeathStarStatus()
{
	if (DeathStar->Enabled)
	{
		return;
	}

	bool deadJim = true;

	for (auto& pairs : DeathStar->FighterPairs)
	{
		if (pairs->Enabled)
		{
			deadJim = false;
			break;
		}

		for (auto& fighter : pairs->Fighters)
		{
			if (fighter->Enabled)
			{
				deadJim = false;
				break;
			}
		}
	}

	if (deadJim)
	{
		SpawnedDeathStar = false;
		TheManagers.EM.ResetTimer(DeathStarSpawnTimerID);
		return;
	}
}

bool EnemyControl::CheckRockCollisions()
{
	NoMoreRocks = true;
	RockCount = 0;

	for (int i = 0; i < Rocks.size(); i++)
	{
		if (Rocks[i]->Enabled)
		{
			NoMoreRocks = false;
			RockCount++;

			CheckUFOCollisions(Rocks[i]);
			CheckEnemyCollisions(Rocks[i]);

			if (Rocks[i]->BeenHit)
			{
				Rocks[i]->Destroy();
				TheManagers.EM.ResetTimer(DeathStarSpawnTimerID);

				if (Rocks[i]->Size == TheRock::Large)
				{
					SpawnRocks(Rocks[i]->Position, 2, TheRock::MediumLarge);
					continue;
				}

				if (Rocks[i]->Size == TheRock::MediumLarge)
				{
					SpawnRocks(Rocks[i]->Position, 3, TheRock::Medium);
					continue;
				}

				if (Rocks[i]->Size == TheRock::Medium)
				{
					SpawnRocks(Rocks[i]->Position, 4, TheRock::Small);
					continue;
				}

			}
		}
	}

	return false;
}

bool EnemyControl::CheckUFOCollisions(TheRock* rock)
{
	for (auto& ufo : UFOs)
	{
		for (auto& shot : ufo->Shots)
		{
			if (shot->Enabled && shot->CirclesIntersect(*rock))
			{
				shot->Destroy();
				rock->Hit();
				return true;
			}

			if (shot->Enabled && shot->CirclesIntersect(*Player))
			{
				shot->Destroy();
				Player->Hit();
				return true;
			}
		}

		if (ufo->Enabled && ufo->CirclesIntersect(*rock))
		{
			ufo->Hit();
			ufo->Destroy();
			rock->Hit();
			return true;
		}
	}

	return false;
}

bool EnemyControl::CheckEnemyCollisions(TheRock* rock)
{
	if (EnemyOne->Enabled && EnemyOne->CirclesIntersect(*rock))
	{
		EnemyOne->Hit();
		EnemyOne->Destroy();
		rock->Hit();
		return true;
	}

	if (EnemyOne->Missile->Enabled && EnemyOne->Missile->CirclesIntersect(*rock))
	{
		EnemyOne->Missile->Destroy();
		rock->Hit();
		return true;
	}

	return false;
}

void EnemyControl::Reset()
{
	UFOSpawnCount = 0;
	RockSpawnCount = StarRockCount;
	TheManagers.EM.ResetTimer(UFOSpawnTimerID);
	TheManagers.EM.ResetTimer(DeathStarSpawnTimerID);

	for (auto &ufo : UFOs)
	{
		ufo->Enabled = false;
	}
}
