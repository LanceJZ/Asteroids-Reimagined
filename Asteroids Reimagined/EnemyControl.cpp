#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	UFOSpawnTimerID = Managers.EM.AddTimer(10.0f);
	DeathStarSpawnTimerID = Managers.EM.AddTimer(5.0f);
	EnemyOneSpawnTimerID = Managers.EM.AddTimer(15.0f);
	EnemyTwoSpawnTimerID = Managers.EM.AddTimer(12.0f);
	BossExplodingTimerID = Managers.EM.AddTimer(5.0f);

	for (int i = 0; i < 2; i++)
	{
		Managers.EM.AddLineModel(UFOs[i] = DBG_NEW TheUFO());
	}

	Managers.EM.AddEntity(DeathStar = DBG_NEW TheDeathStar());
	Managers.EM.AddLineModel(EnemyOne = DBG_NEW Enemy1());
	Managers.EM.AddLineModel(EnemyTwo = DBG_NEW Enemy2());
	Managers.EM.AddLineModel(Boss = DBG_NEW TheBoss());
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
	Boss->SetPlayer(player);
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

	Boss->SetShotModel(model);

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

void EnemyControl::SetBossModel(LineModelPoints model)
{
	Boss->SetModel(model);
}

void EnemyControl::SetBossShieldModel(LineModelPoints model)
{
	Boss->SetShieldModel(model);
}

void EnemyControl::SetBossTurretModel(LineModelPoints model)
{
	Boss->SetTurretModel(model);
}

void EnemyControl::SetEnemyMissileModel(LineModelPoints model)
{
	EnemyOne->SetMissileModel(model);
	Boss->SetMissileModel(model);
}

void EnemyControl::SetEnemyMineModel(LineModelPoints model)
{
	EnemyTwo->SetMineModel(model);
	Boss->SetMineModel(model);
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

	if (ClearForBossWave && !Player->GameOver)
	{
		MakeReadyForBossWave();

		return;
	}

	if (BossWave)
	{
		DoBossStuff();

		return;
	}

	if (NoMoreRocks)
	{
		Wave++;

		if (SpawnedDeathStar) DeathStar->NewWaveStart();

		if (Wave == 1 && !Player->GameOver)
		{
			ClearForBossWave = true;
			return;
		}

		SpawnRocks({ 0, 0, 0 }, RockSpawnCount++, TheRock::Large);
	}

	for (const auto& ufo : UFOs)
	{
		ufo->DeathStarActive = DeathStar->Enabled;
		ufo->DeathStarPosition = DeathStar->Position;
	}

	if (SpawnedDeathStar) CheckDeathStarStatus();
	else if (Wave > 1 && RockCount < 5)
	{
		if (Managers.EM.TimerElapsed(DeathStarSpawnTimerID))
		{
			SpawnDeathStar();
		}
	}

	CheckRockCollisions();

	if (Managers.EM.TimerElapsed(EnemyOneSpawnTimerID))
	{
		Managers.EM.ResetTimer(EnemyOneSpawnTimerID);

		if (!Player->GameOver && !Player->Enabled) return;

		if (!EnemyOne->Enabled && Wave > 2)
		{
			EnemyOne->Spawn({ 0, 0, 0 });
		}
	}

	if (Managers.EM.TimerElapsed(EnemyTwoSpawnTimerID))
	{
		Managers.EM.ResetTimer(EnemyTwoSpawnTimerID);

		if (!Player->GameOver && !Player->Enabled) return;

		if (!EnemyTwo->Enabled && Wave > 3)
		{
			EnemyTwo->Spawn({ 0, 0, 0 });
		}
	}

	if (Managers.EM.TimerElapsed(UFOSpawnTimerID)) SpawnUFO();
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

		for (size_t check = 0; check < rockNumber; check++)
		{
			if (!Rocks.at(check)->Enabled)
			{
				spawnNewRock = false;
				rockNumber = check;
				break;
			}
		}

		if (spawnNewRock)
		{
			size_t rockType = GetRandomValue(0, 3);
			Rocks.push_back(DBG_NEW TheRock());
			Managers.EM.AddLineModel(Rocks.back());
			Rocks.back()->SetModel((RockModels[rockType]));
			Rocks.back()->SetPlayer(Player);
			Rocks.back()->SetExplodeSound(RockExplodeSound);
			Rocks.back()->Initialize();
			Rocks.back()->BeginRun();
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
	Managers.EM.ResetTimer(UFOSpawnTimerID);

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
		Managers.EM.ResetTimer(DeathStarSpawnTimerID);
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

			if (Rocks.at(i)->GetBeenHit())
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
				Managers.EM.ResetTimer(DeathStarSpawnTimerID);
				Particles->SpawnLineParticles(Rocks.at(i)->Position,
					Vector3Multiply(Rocks.at(i)->Velocity, { 0.25f }),
					Rocks.at(i)->Radius * 0.25f, 15.0f, 15, 1.5f, WHITE);

				int count = GetRandomValue(1, 4);

				if (Rocks.at(i)->Size == TheRock::Large)
				{
					SpawnRocks(Rocks.at(i)->Position, count, TheRock::MediumLarge);
					continue;
				}
				else if (Rocks.at(i)->Size == TheRock::MediumLarge)
				{
					SpawnRocks(Rocks.at(i)->Position, count, TheRock::Medium);
					continue;
				}
				else if (Rocks.at(i)->Size == TheRock::Medium)
				{
					SpawnRocks(Rocks.at(i)->Position, count, TheRock::Small);
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

void EnemyControl::MakeReadyForBossWave()
{
	bool allClear = true;

	for (const auto& ufo : UFOs)
	{
		if (ufo->Enabled) allClear = false;

		for (const auto& shot : ufo->Shots)
		{
			if (shot->Enabled) allClear = false;
		}
	}

	if (EnemyOne->Enabled || EnemyTwo->Enabled) allClear = false;

	if (EnemyOne->Missile->Enabled) allClear = false;

	for (const auto& mine : EnemyTwo->Mines)
	{
		if (mine->Enabled) allClear = false;
	}

	if (allClear)
	{
		ClearForBossWave = false;
		BossWave = true;
		SpawnBoss();
	}
}

void EnemyControl::DoBossStuff()
{
	if (!Boss->Enabled && Managers.EM.TimerElapsed(BossExplodingTimerID))
	{
		BossWave = false;
		Managers.EM.ResetTimer(UFOSpawnTimerID);
		Managers.EM.ResetTimer(EnemyOneSpawnTimerID);
		Managers.EM.ResetTimer(EnemyTwoSpawnTimerID);
	}

	if (Boss->GetBeenHit())
	{
		Boss->Destroy();
		Managers.EM.ResetTimer(BossExplodingTimerID);
	}

	if (Player->GameOver)
	{
		Boss->Reset();
		BossWave = false;
	}
}

void EnemyControl::SpawnBoss()
{
	Vector3 position = {0.0f, 0.0f, 0.0f };
	float rotation = 0;
	float width = Boss->WindowWidth * 0.75;
	float height = Boss->WindowHeight * 0.75;

	int option = GetRandomValue(0, 3);

	if (option == 0)
	{
		position.x = GetRandomFloat(-width, width);
		position.y = -Boss->WindowHeight;

		rotation = PI / 2.0f;
	}
	else if (option == 1)
	{
		position.x = GetRandomFloat(-width, width);
		position.y = Boss->WindowHeight;

		rotation = PI + PI / 2.0f;
	}
	else if (option == 2)
	{
		position.x = -Boss->WindowWidth;
		position.y = GetRandomFloat(-height, height);

		rotation = 0;
	}
	else if (option == 3)
	{
		position.x = Boss->WindowWidth;
		position.y = GetRandomFloat(-height, height);

		rotation = PI;
	}

	Boss->Spawn(position, rotation);
}

void EnemyControl::Reset()
{
	BossWave = false;
	ClearForBossWave = false;
	Wave = 0;
	UFOSpawnCount = 0;
	RockSpawnCount = StartRockCount;
	Managers.EM.ResetTimer(UFOSpawnTimerID);
	Managers.EM.ResetTimer(DeathStarSpawnTimerID);

	for (auto& ufo : UFOs)
	{
		ufo->Reset();
	}

	EnemyOne->Reset();
	EnemyTwo->Reset();
}
