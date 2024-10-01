#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	UFOSpawnTimerID = Managers.EM.AddTimer(20.0f);
	DeathStarSpawnTimerID = Managers.EM.AddTimer(5.0f);
	EnemyOneSpawnTimerID = Managers.EM.AddTimer(15.0f);
	EnemyTwoSpawnTimerID = Managers.EM.AddTimer(12.0f);
	BossExplodingTimerID = Managers.EM.AddTimer(5.0f);

	Managers.EM.AddEntity(DeathStar = DBG_NEW TheDeathStar());
	Managers.EM.AddLineModel(EnemyTwo = DBG_NEW Enemy2());
	Managers.EM.AddLineModel(Boss = DBG_NEW TheBoss());
}

EnemyControl::~EnemyControl()
{
}

void EnemyControl::SetPlayer(ThePlayer* player)
{
	Player = player;

	DeathStar->SetPlayer(player);
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
	UFOModel = model;
}

void EnemyControl::SetShotModel(LineModelPoints model)
{
	Boss->SetShotModel(model);

	ShotModel = model;
}

void EnemyControl::SetWedgeModel(LineModelPoints model)
{
	DeathStar->SetWedgeModel(model);
}

void EnemyControl::SetEnemy1Model(LineModelPoints model)
{
	EnemyOneModel = model;
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

void EnemyControl::SetLeftSpineMountModel(LineModelPoints model)
{
	Boss->SetLeftSpineMountModel(model);
}

void EnemyControl::SetRightSpineMountModel(LineModelPoints model)
{
	Boss->SetRightSpineMountModel(model);
}

void EnemyControl::SetEnemyMissileModel(LineModelPoints model)
{
	MissileModel = model;
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
	EnemyOneSpawnSound = sound;
}

void EnemyControl::SetEnemyOneFireSound(Sound sound)
{
	EnemyOneFireSound = sound;
}

void EnemyControl::SetEnemyOneMissileOnSound(Sound sound)
{
	EnemyOneMissileOnSound = sound;
}

void EnemyControl::SetEnemyOneExplodeSound(Sound sound)
{
	EnemyOneExplodeSound = sound;
}

void EnemyControl::SetEnemyOneOnSound(Sound sound)
{
	EnemyOneOnSound = sound;
}

void EnemyControl::SetEnemyOneMissileExplodeSound(Sound sound)
{
	EnemyOneMissileExplodeSound = sound;
}

void EnemyControl::SetEnemyTwoSpawnSound(Sound sound)
{
	EnemyTwo->SetSpawnSound(sound);
	EnemyTwoSpawnSound = sound;
}

void EnemyControl::SetEnemyTwoFireSound(Sound sound)
{
	EnemyTwo->SetFireSound(sound);
	EnemyTwoFireSound = sound;
}

void EnemyControl::SetEnemyTwoExplodeSound(Sound sound)
{
	EnemyTwo->SetExplodeSound(sound);
	EnemyTwoExplodeSound = sound;
}

void EnemyControl::SetEnemyTwoOnSound(Sound sound)
{
	EnemyTwo->SetOnSound(sound);
	EnemyTwoOnSound = sound;
}

void EnemyControl::SetEnemyTwoMineExplodeSound(Sound sound)
{
	EnemyTwo->SetMineExplodeSound(sound);
	EnemyTwoMineExplodeSound = sound;
}

void EnemyControl::SetBossExplodeSound(Sound sound)
{
	Boss->SetExplodeSound(sound);
}

void EnemyControl::SetBossShieldHitSound(Sound sound)
{
	Boss->SetShieldHitSound(sound);
}

void EnemyControl::SetBossShieldDownSound(Sound sound)
{
	Boss->SetShieldDownSound(sound);
}

void EnemyControl::SetBossHitSound(Sound sound)
{
	Boss->SetHitSound(sound);
}

void EnemyControl::SetBossTurretFireSound(Sound sound)
{
	Boss->SetTurretFireSound(sound);
}

void EnemyControl::SetBossTurretExplodeSound(Sound sound)
{
	Boss->SetTurretExplodeSound(sound);
}

void EnemyControl::SetBossSpineFireSound(Sound sound)
{
	Boss->SetSpineFireSound(sound);
}

void EnemyControl::SetParticleManager(ParticleManager* particles)
{
	Particles = particles;

	EnemyTwo->SetParticleManager(particles);
	DeathStar->SetParticleManager(particles);
	Boss->SetParticleManager(particles);
}

bool EnemyControl::Initialize(Utilities* utilities)
{
	Common::Initialize(TheUtilities);

	EnemyTwo->Initialize(TheUtilities);
	DeathStar->Initialize(TheUtilities);
	Boss->Initialize(TheUtilities);

	return false;
}

bool EnemyControl::BeginRun()
{
	Common::BeginRun();

	//DeathStar->SetEnemies(EnemyOne, EnemyTwo); TODO: Change for EnemyOne
	EnemyTwo->BeginRun();
	DeathStar->BeginRun();
	Boss->BeginRun();

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

	CheckRockCollisions();

	if (NoMoreRocks)
	{
		Wave++;

		if (SpawnedDeathStar) DeathStar->NewWaveStart();

		if (Wave % 5 == 0 && !Player->GameOver)
		{
			ClearForBossWave = true;
			return;
		}

		if (Wave > 5) RockSpawnCount += 2;
		else RockSpawnCount++;

		if (Wave > 10) RockSpawnCount += 3;
		if (Wave > 15) RockSpawnCount += 4;
		if (Wave > 20) RockSpawnCount += 5;
		if (Wave > 25) RockSpawnCount += 6;

		SpawnRocks({ 0, 0, 0 }, RockSpawnCount, TheRock::Large);
	}

	if (SpawnedDeathStar)
	{
		CheckDeathStarStatus();

		for (const auto& ufo : UFOs)
		{
			ufo->DeathStarActive = DeathStar->Enabled;
			ufo->DeathStarPosition = DeathStar->Position;
		}
	}
	else if (Wave > 1 && RockCount < 6)
	{
		if (Managers.EM.TimerElapsed(DeathStarSpawnTimerID))
		{
			SpawnDeathStar();
		}
	}

	HaveHomingMineChaseEnemy();

	if (Managers.EM.TimerElapsed(EnemyOneSpawnTimerID))
	{
		Managers.EM.ResetTimer(EnemyOneSpawnTimerID, EnemyOneSpawnTimeAmount);

		if (!Player->GameOver && !Player->Enabled) return;

		if (Wave > 2)
		{
			SpawnEnemyOne();
		}
	}

	if (Managers.EM.TimerElapsed(EnemyTwoSpawnTimerID))
	{
		Managers.EM.ResetTimer(EnemyTwoSpawnTimerID);

		if (!Player->GameOver && !Player->Enabled) return;

		if (!EnemyTwo->Enabled && Wave > 3)
		{
			EnemyTwo->Spawn({ 0, 0, 0 });
			EnemyTwo->Wave = Wave;

			EnemyTwo->UFORefs.clear();

			for (const auto& ufo : UFOs)
			{
				EnemyTwo->UFORefs.push_back(ufo);
			}
		}
	}

	if (Managers.EM.TimerElapsed(UFOSpawnTimerID)) SpawnUFO();

	for (const auto& ufo : UFOs)
	{
		ufo->CheckShotsHitPlayer();
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
			Rocks.back()->Initialize(TheUtilities);
			Rocks.back()->BeginRun();

			for (const auto& ufo : UFOs)
			{
				ufo->Rocks.push_back(Rocks.back());
			}
		}

		Rocks.at(rockNumber)->Spawn(position, size);
	}
}

void EnemyControl::SpawnUFO()
{
	float ufoTimeAmountAdjust = ((float)Wave * 0.1f) + ((float)UFOSpawnCount * 0.01f);

	if (UFOSpawnTimeAmount < ufoTimeAmountAdjust - 0.5f)
		ufoTimeAmountAdjust = UFOSpawnTimeAmount - 0.5f;

	float min = UFOSpawnTimeAmount / (((float)Wave * 0.1f) + 1.0f);
	float max = UFOSpawnTimeAmount - ufoTimeAmountAdjust;

	float ufoSpawnTime = GetRandomFloat(min, max);

	Managers.EM.ResetTimer(UFOSpawnTimerID, ufoSpawnTime);

	if (!Player->GameOver && !Player->Enabled) return;

	bool spawnUFO = true;
	size_t ufoNumber = UFOs.size();

	for (size_t check = 0; check < ufoNumber; check++)
	{
		if (!UFOs.at(check)->Enabled)
		{
			spawnUFO = false;
			ufoNumber = check;
			break;
		}
	}

	if (spawnUFO)
	{
		UFOs.push_back(DBG_NEW TheUFO());
		Managers.EM.AddLineModel(UFOs.back());
		UFOs.back()->SetModel(UFOModel);
		UFOs.back()->SetShotModel(ShotModel);
		UFOs.back()->SetExplodeSound(UFOExplodeSound);
		UFOs.back()->SetFireSound(UFOFireSound);
		UFOs.back()->SetBigSound(UFOBigSound);
		UFOs.back()->SetSmallSound(UFOSmallSound);
		UFOs.back()->SetPlayer(Player);
		UFOs.back()->SetParticleManager(Particles);
		UFOs.back()->Initialize(TheUtilities);
		UFOs.back()->BeginRun();

		for (const auto& enemy : EnemyOnes)
		{
			enemy->UFORefs.push_back(UFOs.back());
		}

		DeathStar->UFORefs.push_back(UFOs.back());
		DeathStar->SetUFO();
	}

	UFOs.at(ufoNumber)->Spawn(UFOSpawnCount++);
}

void EnemyControl::SpawnDeathStar()
{
	if (!Player->GameOver && !Player->Enabled) return;

	Vector3 position = { 0.0f, 0.0f, 0.0f };
	int width = (int)(GetScreenWidth() / 1.25f);
	int height = (int)(GetScreenHeight() / 1.25f);
	int windowHeight = GetScreenHeight();
	int windowWidth = GetScreenWidth();
	float maxSpeed = 20.0f;

	if (GetRandomValue(1, 10) < 5)
	{
		if (GetRandomValue(1, 10) < 5)
		{
			// Top
			position.y = (float)-windowHeight;
			position.x = (float)GetRandomValue(-width, width);
			DeathStar->Velocity.y = maxSpeed;
			DeathStar->Velocity.x = maxSpeed;
		}
		else
		{
			//Bottom
			position.y = (float)windowHeight;
			position.x = (float)GetRandomValue(-width, width);
			DeathStar->Velocity.y = -maxSpeed;
			DeathStar->Velocity.x = -maxSpeed;
		}

	}
	else
	{
		if (GetRandomValue(1, 10) < 5)
		{
			//Left
			position.x = (float)-windowWidth;
			position.y = (float)GetRandomValue(-height, height);
			DeathStar->Velocity.x = maxSpeed;
			DeathStar->Velocity.y = maxSpeed;
		}
		else
		{
			//Right
			position.x = (float)windowWidth;
			position.y = (float)GetRandomValue(-height, height);
			DeathStar->Velocity.x = -maxSpeed;
			DeathStar->Velocity.y = -maxSpeed;
		}
	}

	DeathStar->Spawn(position);
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
	bool ufoHitRock = false;
	NoMoreRocks = true;
	RockCount = 0;

	for (int i = 0; i < Rocks.size(); i++)
	{
		if (Rocks.at(i)->Enabled)
		{
			NoMoreRocks = false;
			RockCount++;

			if (CheckUFOCollisions(Rocks.at(i))) ufoHitRock = true;

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

				int count = 0;

				int max = (int)(Wave * 0.5f) + 5;

				if (ufoHitRock) count = GetRandomValue(4, max);
				else count = GetRandomValue(1, 4);

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

bool EnemyControl::CheckUFOCollisions(TheRock* rock)
{
	bool ufoHitRock = false;

	for (const auto& ufo : UFOs)
	{
		if (!ufo->Enabled) continue;

		if (rock->Size == TheRock::Small) continue;

		if (ufo->CheckShotCollisions(rock)) ufoHitRock = true;

		if (ufo->CirclesIntersect(*rock))
		{
			ufo->Hit();
			rock->Hit();
		}
	}

	return ufoHitRock;
}

void EnemyControl::CheckEnemyCollisions(TheRock* rock)
{
	if (rock->Size == TheRock::Small) return;

	for (const auto& enemy : EnemyOnes)
	{
		if (enemy->Enabled && enemy->CirclesIntersect(*rock))
		{
			enemy->Hit();
			rock->Hit();
			return;
		}
	}

	if (EnemyTwo->Enabled && EnemyTwo->CirclesIntersect(*rock))
	{
		EnemyTwo->Hit();
		rock->Hit();
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

	for (const auto& enemy : EnemyOnes)
	{
		allClear = false;
	}

	if (EnemyTwo->Enabled) allClear = false;

	for (const auto& mine : EnemyTwo->Mines)
	{
		if (mine->Enabled) allClear = false;
	}

	if (DeathStar->Enabled) allClear = false;

	for (auto& fighterPair : DeathStar->FighterPairs)
	{
		if (fighterPair->Enabled) allClear = false;

		for (const auto& fighter : fighterPair->Fighters)
		{
			if (fighter->Enabled) allClear = false;
		}
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

	Managers.EM.ResetTimer(DeathStarSpawnTimerID);
}

void EnemyControl::SpawnBoss()
{
	Vector3 position = {0.0f, 0.0f, 0.0f };
	float rotation = 0;
	float width = Boss->WindowWidth * 0.75f;
	float height = Boss->WindowHeight * 0.75f;

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

void EnemyControl::SpawnEnemyOne()
{
	float enemyTimeAmountAdjust = ((float)Wave * 0.1f) +
		(((float)(EnemyOneSpawnCount++)) * 0.01f);

	if (EnemyOneSpawnTimeAmount < enemyTimeAmountAdjust - 0.5f)
		enemyTimeAmountAdjust = EnemyOneSpawnTimeAmount - 0.5f;

	float min = EnemyOneSpawnTimeAmount / (((float)(Wave - 3) * 0.1f) + 1.0f);
	float max = EnemyOneSpawnTimeAmount - enemyTimeAmountAdjust;

	float enemyTime = GetRandomFloat(min, max);

	Managers.EM.ResetTimer(EnemyOneSpawnTimerID, enemyTime);

	bool spawnOne = true;
	size_t enemyNumber = EnemyOnes.size();

	for (size_t i = 0; i < enemyNumber; i++)
	{
		if (!EnemyOnes.at(i)->Enabled)
		{
			spawnOne = false;
			enemyNumber = i;
			break;
		}
	}

	if (spawnOne)
	{
		EnemyOnes.push_back(DBG_NEW Enemy1());
		Managers.EM.AddLineModel(EnemyOnes.back());
		EnemyOnes.back()->SetModel(EnemyOneModel);
		EnemyOnes.back()->SetShotModel(ShotModel);
		EnemyOnes.back()->SetMissileModel(MissileModel);
		EnemyOnes.back()->SetSpawnSound(EnemyOneSpawnSound);
		EnemyOnes.back()->SetFireSound(EnemyOneFireSound);
		EnemyOnes.back()->SetExplodeSound(EnemyOneExplodeSound);
		EnemyOnes.back()->SetMissileExplodeSound(EnemyOneMissileExplodeSound);
		EnemyOnes.back()->SetOnSound(EnemyOneOnSound);
		EnemyOnes.back()->SetMissileOnSound(EnemyOneMissileOnSound);
		EnemyOnes.back()->SetPlayer(Player);
		EnemyOnes.back()->SetParticleManager(Particles);
		EnemyOnes.back()->Initialize(TheUtilities);
		EnemyOnes.back()->BeginRun();
	}

	EnemyOnes.at(enemyNumber)->Wave = Wave;
	EnemyOnes.at(enemyNumber)->UFORefs.clear();

	for (const auto& ufo : UFOs)
	{
		EnemyOnes.at(enemyNumber)->UFORefs.push_back(ufo);
	}

	EnemyOnes.at(enemyNumber)->Spawn();
}

void EnemyControl::HaveHomingMineChaseEnemy()
{
	for (const auto& mine : Player->Mines)
	{
		if (!mine->Enabled) continue;

		bool enemyToChase = false;
		float distance = 450.0f;
		Vector3 enemyPosition = { 0.0f, 0.0f, 0.0f };

		for (const auto& ufo : UFOs)
		{
			if (!ufo->Enabled) continue;

			float ufoDistance = Vector3Distance(ufo->Position, mine->Position);

			if (ufoDistance < distance)
			{
				distance = ufoDistance;
				enemyToChase = true;
				enemyPosition = ufo->Position;
				break;
			}
		}

		for (const auto& enemy : EnemyOnes)
		{
			if (enemy->Enabled && !enemyToChase)
			{
				float enemyOneDistance = Vector3Distance(enemy->Position, mine->Position);

				if (enemyOneDistance < distance)
				{
					distance = enemyOneDistance;
					enemyToChase = true;
					enemyPosition = enemy->Position;
					break;
				}
			}
		}

		if (EnemyTwo->Enabled && !enemyToChase)
		{
			float enemyTwoDistance = Vector3Distance(EnemyTwo->Position, mine->Position);

			if (enemyTwoDistance < distance)
			{
				distance = enemyTwoDistance;
				enemyToChase = true;
				enemyPosition = EnemyTwo->Position;
			}
		}

		if (DeathStar->Enabled)
		{
			float deathStarDistance =
				Vector3Distance(DeathStar->GetWorldPosition(), mine->Position);

			if (deathStarDistance < distance)
			{
				enemyPosition = DeathStar->GetWorldPosition();
				enemyToChase = true;
			}

		}

		if (enemyToChase)
		{
			mine->ChaseEnemy(enemyPosition);
		}
		else
		{
			mine->LostEnemy();
		}
	}
}

void EnemyControl::Reset()
{
	BossWave = false;
	ClearForBossWave = false;
	Wave = 0;
	UFOSpawnCount = 0;
	RockSpawnCount = StartRockCount;

	Managers.EM.ResetTimer(UFOSpawnTimerID, 35.0f);
	Managers.EM.ResetTimer(DeathStarSpawnTimerID);

	UFOSpawnTimeAmount = 30.0f;
	EnemyOneSpawnTimeAmount = 15.0f;

	for (auto& ufo : UFOs)
	{
		ufo->Reset();
	}

	for (const auto& enemy : EnemyOnes)
	{
		enemy->Reset();
	}

	EnemyTwo->Reset();

	for (const auto& ufo : UFOs)
	{
		ufo->Reset();
	}
}
