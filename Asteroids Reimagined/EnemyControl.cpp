#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	UFOSpawnTimerID = TheManagers.EM.AddTimer(10.0f);
	DeathStarSpawnTimerID = TheManagers.EM.AddTimer(5.0f);
	EnemyOneSpawnTimerID = TheManagers.EM.AddTimer(15.0f);
	EnemyTwoSpawnTimerID = TheManagers.EM.AddTimer(12.0f);

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

	for (const auto &ufo : UFOs)
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

void EnemyControl::SetRockExplodeSound(Sound sound)
{
	RockExplodeSound = sound;
}

void EnemyControl::SetUFOExplodeSound(Sound sound)
{
	UFOExplodeSound = sound;
}

void EnemyControl::SetUFOFireSound(Sound sound)
{
	UFOFireSound = sound;
}

void EnemyControl::SetUFOBigSound(Sound sound)
{
	UFOBigSound = sound;
}

void EnemyControl::SetUFOSmallSound(Sound sound)
{
	UFOSmallSound = sound;
}

void EnemyControl::SetDeathStarSpawnSound(Sound sound)
{
	DeathStar->SetSpawnSound(sound);
}

void EnemyControl::SetDeathStarExplodeSound(Sound sound)
{
	DeathStar->SetExplodeSound(sound);
}

void EnemyControl::SetEnemyOneSpawnSound(Sound sound)
{
	EnemyOne->SetSpawnSound(sound);
}

void EnemyControl::SetEnemyOneFireSound(Sound sound)
{
	EnemyOne->SetFireSound(sound);
}

void EnemyControl::SetEnemyOneExplodeSound(Sound sound)
{
	EnemyOne->SetExplodeSound(sound);
}

void EnemyControl::SetEnemyOneOnSound(Sound sound)
{
	EnemyOne->SetOnSound(sound);
}

void EnemyControl::SetEnemyOneMissileExplodeSound(Sound sound)
{
	EnemyOne->SetMissileExplodeSound(sound);
}

void EnemyControl::SetEnemyTwoSpawnSound(Sound sound)
{
	EnemyTwo->SetSpawnSound(sound);
}

void EnemyControl::SetEnemyTwoFireSound(Sound sound)
{
	EnemyTwo->SetFireSound(sound);
}

void EnemyControl::SetEnemyTwoExplodeSound(Sound sound)
{
	EnemyTwo->SetExplodeSound(sound);
}

void EnemyControl::SetEnemyTwoOnSound(Sound sound)
{
	EnemyTwo->SetOnSound(sound);
}

void EnemyControl::SetEnemyTwoMineExplodeSound(Sound sound)
{
	EnemyTwo->SetMineExplodeSound(sound);
}

void EnemyControl::SetParticleManager(ParticleManager* particles)
{
	Particles = particles;

	for (const auto& ufo : UFOs)
	{
		ufo->SetParticles(particles);
	}

	EnemyOne->SetParticleManager(particles);
	EnemyOne->Missile->SetParticleManager(particles);
	EnemyTwo->SetParticleManager(particles);
}

bool EnemyControl::Initialize(Utilities* utilities)
{
	Common::Initialize(TheUtilities);

	for (const auto &ufo : UFOs)
	{
		ufo->Initialize(TheUtilities);
	}

	DeathStar->Initialize(TheUtilities);

	return false;
}

bool EnemyControl::BeginRun()
{

	DeathStar->BeginRun();

	for (const auto& ufo : UFOs)
	{
		ufo->SetExplodeSound(UFOExplodeSound);
		ufo->SetFireSound(UFOFireSound);
		ufo->SetBigSound(UFOBigSound);
		ufo->SetSmallSound(UFOSmallSound);
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

	for (const auto& ufo : UFOs)
	{
		ufo->DeathStarActive = DeathStar->Enabled;
		ufo->DeathStarPosition = DeathStar->Position;
	}

	if (SpawnedDeathStar) CheckDeathStarStatus();
	else if (Wave > 1 && RockCount < 5)
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

		if (!EnemyOne->Enabled && Wave > 2)
		{
			EnemyOne->Spawn({ 0, 0, 0 });
		}
	}

	if (TheManagers.EM.TimerElapsed(EnemyTwoSpawnTimerID))
	{
		TheManagers.EM.ResetTimer(EnemyTwoSpawnTimerID);

		if (!Player->GameOver && !Player->Enabled) return;

		if (!EnemyTwo->Enabled && Wave > 3)
		{
			EnemyTwo->Spawn({ 0, 0, 0 });
		}
	}
}

void EnemyControl::NewGame()
{
	DeathStar->NewGame();
	Reset();

	for (const auto& rock : Rocks)
	{
		rock->Destroy();
	}
}

void EnemyControl::NextWave()
{
	Wave++;

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
			TheManagers.EM.AddLineModel(Rocks.back());
			Rocks.back()->Initialize();
			Rocks.back()->BeginRun();
			Rocks.back()->SetModel((RockModels[rockType]));
			Rocks.back()->SetPlayer(Player);
			Rocks.back()->SetExplodeSound(RockExplodeSound);
		}

		Rocks.at(rockNumber)->Spawn(position, size);

		for (const auto& ufo : UFOs)
		{
			ufo->SetRocks(Rocks);
		}
	}
}

void EnemyControl::SpawnUFO()
{
	TheManagers.EM.ResetTimer(UFOSpawnTimerID);

	if (!Player->GameOver && !Player->Enabled) return;

	for (const auto &ufo : UFOs)
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

	for (const auto& pairs : DeathStar->FighterPairs)
	{
		if (pairs->Enabled)
		{
			deadJim = false;
			break;
		}

		for (const auto& fighter : pairs->Fighters)
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
				if (Player->Enabled)
				{
					if (GetRandomValue(0, 100) < 10)
					{
						SpawnPowerUp = true;
						PowerUpSpawnPosition = Rocks.at(i)->Position;
					}
				}

				Rocks.at(i)->Destroy();
				TheManagers.EM.ResetTimer(DeathStarSpawnTimerID);
				Particles->SpawnLineParticles(Rocks.at(i)->Position,
					Vector3Multiply(Rocks.at(i)->Velocity, { 0.25f }),
					Rocks.at(i)->Radius * 0.25f, 15.0f, 15, 1.5f, WHITE);

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
	for (const auto& ufo : UFOs)
	{
		ufo->CheckCollisions(rock);

		if (!ufo->Enabled) continue;

		if (EnemyOne->Enabled && ufo->CirclesIntersect(*EnemyOne))
		{
			ufo->Hit();
			EnemyOne->Hit();
		}

		if (EnemyTwo->Enabled && ufo->CirclesIntersect(*EnemyTwo))
		{
			ufo->Hit();
			EnemyTwo->Hit();
		}
	}
}

void EnemyControl::CheckEnemyCollisions(TheRock* rock)
{
	if (EnemyOne->Enabled && EnemyOne->CirclesIntersect(*rock))
	{
		EnemyOne->Hit();
		rock->Hit();
		return;
	}

	if (EnemyOne->Missile->Enabled && EnemyOne->Missile->CirclesIntersect(*rock))
	{
		EnemyOne->Missile->Hit();
		rock->Hit();
		return;
	}

	if (EnemyTwo->Enabled && EnemyTwo->CirclesIntersect(*rock))
	{
		EnemyTwo->Hit();
		rock->Hit();
		return;
	}

	if (EnemyOne->Enabled && EnemyTwo->Enabled && EnemyOne->CirclesIntersect(*EnemyTwo))
	{
		EnemyOne->Hit();
		EnemyTwo->Hit();
		return;
	}

	if (EnemyTwo->Enabled && EnemyOne->Enabled && EnemyTwo->CirclesIntersect(*EnemyOne))
	{
		EnemyTwo->Hit();
		EnemyOne->Hit();
		return;
	}

	if (EnemyOne->Missile->Enabled && EnemyTwo->Enabled
		&& EnemyOne->Missile->CirclesIntersect(*EnemyTwo))
	{
		EnemyOne->Missile->Hit();
		EnemyTwo->Hit();
		return;
	}
}

void EnemyControl::Reset()
{
	Wave = 0;
	UFOSpawnCount = 0;
	RockSpawnCount = StartRockCount;
	TheManagers.EM.ResetTimer(UFOSpawnTimerID);
	TheManagers.EM.ResetTimer(DeathStarSpawnTimerID);

	for (auto& ufo : UFOs)
	{
		ufo->Reset();
	}

	EnemyOne->Reset();
	EnemyTwo->Reset();
}
