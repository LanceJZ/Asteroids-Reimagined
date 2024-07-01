#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	UFOSpawnTimerID = TheManagers.EM.AddTimer(10.0f);
	DeathStarSpawnTimerID = TheManagers.EM.AddTimer(5.0f);
	EnemyOneSpawnTimerID = TheManagers.EM.AddTimer(13.0f);
	EnemyTwoSpawnTimerID = TheManagers.EM.AddTimer(10.0f);

	for (int i = 0; i < 2; i++)
	{
		TheManagers.EM.AddLineModel(UFOs[i] = DBG_NEW TheUFO());
	}

	TheManagers.EM.AddEntity(DeathStar = DBG_NEW TheDeathStar());
	TheManagers.EM.AddLineModel(EnemyOne = DBG_NEW Enemy1());
	TheManagers.EM.AddLineModel(EnemyTwo = DBG_NEW Enemy2());
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
	EnemyTwo->SetPlayer(player);
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

void EnemyControl::SetEnemy2Model(LineModelPoints model)
{
	EnemyTwo->SetModel(model);
}

void EnemyControl::SetEnemyMissileModel(LineModelPoints model)
{
	EnemyOne->SetMissileModel(model);
}

void EnemyControl::SetEnemyMineModel(LineModelPoints model)
{
	EnemyTwo->SetMineModel(model);
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

	EnemyOne->SetUFO(UFOs);
	EnemyTwo->SetUFO(UFOs);

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

		if (!Player->GameOver && !Player->Enabled) return;

		if (!EnemyOne->Enabled)
		{
			EnemyOne->Spawn({ 0, 0, 0 });
		}
	}

	if (TheManagers.EM.TimerElapsed(EnemyTwoSpawnTimerID))
	{
		TheManagers.EM.ResetTimer(EnemyTwoSpawnTimerID);

		if (!Player->GameOver && !Player->Enabled) return;

		if (!EnemyTwo->Enabled)
		{
			EnemyTwo->Spawn({ 0, 0, 0 });
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
			if (!Rocks.at(rockCheck)->Enabled)
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
			TheManagers.EM.AddLineModel(Rocks.at(rockNumber));
			Rocks.at(rockNumber)->Initialize();
			Rocks.at(rockNumber)->BeginRun();
			Rocks.at(rockNumber)->SetModel((RockModels[rockType]));
			Rocks.at(rockNumber)->SetPlayer(Player);
		}

		Rocks.at(rockNumber)->Spawn(position, size);
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
	if (!Player->GameOver && !Player->Enabled) return;

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

void EnemyControl::CheckRockCollisions()
{
	NoMoreRocks = true;
	RockCount = 0;

	for (int i = 0; i < Rocks.size(); i++)
	{
		if (Rocks.at(i)->Enabled)
		{
			NoMoreRocks = false;
			RockCount++;

			CheckUFOCollisions(Rocks.at(i));
			CheckEnemyCollisions(Rocks.at(i));

			if (Rocks.at(i)->BeenHit)
			{
				Rocks.at(i)->Destroy();
				TheManagers.EM.ResetTimer(DeathStarSpawnTimerID);

				if (Rocks.at(i)->Size == TheRock::Large)
				{
					SpawnRocks(Rocks.at(i)->Position, 2, TheRock::MediumLarge);
					continue;
				}

				if (Rocks.at(i)->Size == TheRock::MediumLarge)
				{
					SpawnRocks(Rocks.at(i)->Position, 3, TheRock::Medium);
					continue;
				}

				if (Rocks.at(i)->Size == TheRock::Medium)
				{
					SpawnRocks(Rocks.at(i)->Position, 4, TheRock::Small);
					continue;
				}
			}
		}
	}
}

void EnemyControl::CheckUFOCollisions(TheRock* rock)
{
	for (auto& ufo : UFOs)
	{
		ufo->CheckCollisions(rock);

		if (!ufo->Enabled) return;

		if (EnemyOne->Enabled && ufo->CirclesIntersect(*EnemyOne))
		{
			ufo->Hit();
			ufo->Destroy();
			EnemyOne->Hit();
			EnemyOne->Destroy();
		}

		if (EnemyTwo->Enabled && ufo->CirclesIntersect(*EnemyTwo))
		{
			ufo->Hit();
			ufo->Destroy();
			EnemyTwo->Hit();
			EnemyTwo->Destroy();
		}
	}
}

void EnemyControl::CheckEnemyCollisions(TheRock* rock)
{
	if (EnemyOne->Enabled && EnemyOne->CirclesIntersect(*rock))
	{
		EnemyOne->Hit();
		EnemyOne->Destroy();
		rock->Hit();
		return;
	}

	if (EnemyOne->Missile->Enabled && EnemyOne->Missile->CirclesIntersect(*rock))
	{
		EnemyOne->Missile->Destroy();
		rock->Hit();
		return;
	}

	if (EnemyTwo->Enabled && EnemyTwo->CirclesIntersect(*rock))
	{
		EnemyTwo->Hit();
		EnemyTwo->Destroy();
		rock->Hit();
		return;
	}

	if (EnemyOne->Enabled && EnemyTwo->Enabled && EnemyOne->CirclesIntersect(*EnemyTwo))
	{
		EnemyOne->Hit();
		EnemyOne->Destroy();
		EnemyTwo->Hit();
		EnemyTwo->Destroy();
		return;
	}

	if (EnemyTwo->Enabled && EnemyOne->Enabled && EnemyTwo->CirclesIntersect(*EnemyOne))
	{
		EnemyTwo->Hit();
		EnemyTwo->Destroy();
		EnemyOne->Hit();
		EnemyOne->Destroy();
		return;
	}

	if (EnemyOne->Missile->Enabled && EnemyTwo->Enabled
		&& EnemyOne->Missile->CirclesIntersect(*EnemyTwo))
	{
		EnemyOne->Missile->Destroy();
		EnemyTwo->Hit();
		EnemyTwo->Destroy();
		return;
	}
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
