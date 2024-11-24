#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	UFOSpawnTimerID = Managers.EM.AddTimer(20.0f);
	DeathStarSpawnTimerID = Managers.EM.AddTimer(5.0f);
	EnemyOneSpawnTimerID = Managers.EM.AddTimer(15.0f);
	EnemyTwoSpawnTimerID = Managers.EM.AddTimer(12.0f);
	BossExplodingTimerID = Managers.EM.AddTimer(5.0f);

	Managers.EM.AddEntity(DeathStar = DBG_NEW TheDeathStar());
	Managers.EM.AddLineModel(Boss = DBG_NEW TheBoss());
}

EnemyControl::~EnemyControl()
{
}

void EnemyControl::SetPlayer(ThePlayer* player)
{
	Player = player;

	DeathStar->SetPlayer(player);
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
	EnemyTwoModel = model;
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
	MineModel = model;
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
	Boss->SetMissileFireSound(sound);
}

void EnemyControl::SetEnemyOneMissileOnSound(Sound sound)
{
	EnemyOneMissileOnSound = sound;
	Boss->SetMissileOnSound(sound);
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
	Boss->SetMissileExplodeSound(sound);
}

void EnemyControl::SetEnemyTwoSpawnSound(Sound sound)
{
	EnemyTwoSpawnSound = sound;
}

void EnemyControl::SetEnemyTwoFireSound(Sound sound)
{
	EnemyTwoFireSound = sound;
	Boss->SetMineDropSound(sound);
}

void EnemyControl::SetEnemyTwoExplodeSound(Sound sound)
{
	EnemyTwoExplodeSound = sound;
}

void EnemyControl::SetEnemyTwoOnSound(Sound sound)
{
	EnemyTwoOnSound = sound;
}

void EnemyControl::SetEnemyTwoMineExplodeSound(Sound sound)
{
	EnemyTwoMineExplodeSound = sound;
	Boss->SetMineExplodeSound(sound);
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

	DeathStar->SetParticleManager(particles);
	Boss->SetParticleManager(particles);
}

bool EnemyControl::Initialize(Utilities* utilities)
{
	Common::Initialize(TheUtilities);

	DeathStar->Initialize(TheUtilities);
	Boss->Initialize(TheUtilities);

	return false;
}

bool EnemyControl::BeginRun()
{
	Common::BeginRun();

	//DeathStar->SetEnemies(EnemyOne, EnemyTwo);
	// TODO: Change for EnemyOne and EnemyTwo info to DeathStar.
	DeathStar->BeginRun();
	Boss->BeginRun();

	Reset();

	return false;
}

void EnemyControl::Update()
{
	Common::Update();

	for (const auto& ufo : UFOs)
	{
		ufo->CheckShotsHitPlayer();
	}

	for (const auto& enemy : EnemyOnes)
	{
		enemy->CheckShotsHitPlayer();
	}

	for (const auto& enemy : EnemyTwos)
	{
		enemy->CheckShotsHitPlayer();
	}

	CheckRockCollisions();
}

void EnemyControl::FixedUpdate()
{
	Common::FixedUpdate();

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
		if (Wave > 30) RockSpawnCount += 7;

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
	else if (Wave > 5 && RockCount < 10)
	{
		if (Managers.EM.TimerElapsed(DeathStarSpawnTimerID))
		{
			SpawnDeathStar();
		}
	}
	else if (Wave > 1 && RockCount < 6)
	{
		if (Managers.EM.TimerElapsed(DeathStarSpawnTimerID))
		{
			SpawnDeathStar();
		}
	}
	else if (RockCount < 3)
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
		Managers.EM.ResetTimer(EnemyTwoSpawnTimerID, EnemyTwoSpawnTimeAmount);

		if (!Player->GameOver && !Player->Enabled) return;

		if (Wave > 3)
		{
			SpawnEnemyTwo();
		}
	}

	if (Managers.EM.TimerElapsed(UFOSpawnTimerID)) SpawnUFO();
}

void EnemyControl::NextWave()
{
	Wave++;
}

void EnemyControl::NewGame()
{
	DeathStar->NewGame();

	for (const auto& rock : Rocks)
	{
		rock->Destroy();
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
	EnemyTwoSpawnTimeAmount = 20.0f;

	for (const auto& enemy : EnemyOnes)
	{
		enemy->Reset();
	}

	for (const auto& enemy : EnemyTwos)
	{
		enemy->Reset();
	}

	for (const auto& ufo : UFOs)
	{
		ufo->Reset();
	}
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
			Managers.EM.AddLineModel(Rocks.back(), (RockModels[rockType]));
			Rocks.back()->SetPlayer(Player);
			Rocks.back()->SetExplodeSound(RockExplodeSound);
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
	float ufoTimeAmountAdjust = ((float)Wave * 0.3f) + ((float)UFOSpawnCount * 0.03f);

	if (UFOSpawnTimeAmount < ufoTimeAmountAdjust - 0.5f)
		ufoTimeAmountAdjust = UFOSpawnTimeAmount - 0.5f;

	float min = (UFOSpawnTimeAmount - ufoTimeAmountAdjust) /
		(((float)Wave * 0.1f) + 1.0f);
	float max = UFOSpawnTimeAmount - ufoTimeAmountAdjust;

	if (max < min) min = max;

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
		Managers.EM.AddLineModel(UFOs.back(), UFOModel);
		UFOs.back()->SetShotModel(ShotModel);
		UFOs.back()->SetExplodeSound(UFOExplodeSound);
		UFOs.back()->SetFireSound(UFOFireSound);
		UFOs.back()->SetBigSound(UFOBigSound);
		UFOs.back()->SetSmallSound(UFOSmallSound);
		UFOs.back()->SetPlayer(Player);
		UFOs.back()->SetParticleManager(Particles);
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

void EnemyControl::SpawnEnemyOne()
{
	float enemyTimeAmountAdjust = ((float)(Wave - 3) * 0.5f) +
		(((float)(EnemyOneSpawnCount++)) * 0.05f);

	if (EnemyOneSpawnTimeAmount < enemyTimeAmountAdjust - 0.5f)
		enemyTimeAmountAdjust = EnemyOneSpawnTimeAmount - 0.5f;

	float min = (EnemyOneSpawnTimeAmount - enemyTimeAmountAdjust) /
		(((float)(Wave - 3) * 0.1f) + 1.0f);
	float max = EnemyOneSpawnTimeAmount - enemyTimeAmountAdjust;

	if (min < 0.15f) min = 0.15f;

	if (min > max) max = min * 2.0f;

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
		Managers.EM.AddLineModel(EnemyOnes.back(), EnemyOneModel);
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

void EnemyControl::SpawnEnemyTwo()
{
	float enemyTimeAmountAdjust = ((float)(Wave - 4) * 0.5f) +
		(((float)(EnemyTwoSpawnCount++)) * 0.05f);

	if (EnemyTwoSpawnTimeAmount < enemyTimeAmountAdjust - 0.5f)
		enemyTimeAmountAdjust = EnemyTwoSpawnTimeAmount - 0.5f;

	float min = (EnemyTwoSpawnTimeAmount - enemyTimeAmountAdjust) /
		(((float)(Wave - 4) * 0.1f) + 1.0f);
	float max = EnemyTwoSpawnTimeAmount - enemyTimeAmountAdjust;

	if (min < 0.15f) min = 0.15f;

	if (min > max) max = min * 2.0f;

	float enemyTime = GetRandomFloat(min, max);

	Managers.EM.ResetTimer(EnemyTwoSpawnTimerID, enemyTime);

	bool spawnOne = true;
	size_t enemyNumber = EnemyTwos.size();

	for (size_t i = 0; i < enemyNumber; i++)
	{
		if (!EnemyTwos.at(i)->Enabled)
		{
			spawnOne = false;
			enemyNumber = i;
			break;
		}
	}

	if (spawnOne)
	{
		EnemyTwos.push_back(DBG_NEW Enemy2());
		Managers.EM.AddLineModel(EnemyTwos.back(), EnemyTwoModel);
		EnemyTwos.back()->SetShotModel(ShotModel);
		EnemyTwos.back()->SetMineModel(MineModel);
		EnemyTwos.back()->SetSpawnSound(EnemyTwoSpawnSound);
		EnemyTwos.back()->SetFireSound(EnemyTwoFireSound);
		EnemyTwos.back()->SetExplodeSound(EnemyTwoExplodeSound);
		EnemyTwos.back()->SetOnSound(EnemyTwoOnSound);
		EnemyTwos.back()->SetPlayer(Player);
		EnemyTwos.back()->SetParticleManager(Particles);
		EnemyTwos.back()->BeginRun();
	}

	EnemyTwos.at(enemyNumber)->Wave = Wave;
	EnemyTwos.at(enemyNumber)->UFORefs.clear();

	for (const auto& ufo : UFOs)
	{
		EnemyTwos.at(enemyNumber)->UFORefs.push_back(ufo);
	}

	EnemyTwos.at(enemyNumber)->Spawn();
}

void EnemyControl::SpawnDeathStar()
{
	if (!Player->GameOver && !Player->Enabled) return;

	Vector3 position = { 0.0f, 0.0f, 0.0f };
	int width = (int)(GetScreenWidth() / 2.25f);
	int height = (int)(GetScreenHeight() / 2.25f);
	int windowHeight = GetScreenHeight() / 2;
	int windowWidth = GetScreenWidth() / 2;
	float maxSpeed = 20.0f;

	if (GetRandomValue(1, 10) < 5)
	{
		position.x = (float)GetRandomValue(-width, width);

		if (GetRandomValue(1, 10) < 5)
		{
			// Top
			position.y = (float)-windowHeight;
			DeathStar->Velocity.y = maxSpeed;
			DeathStar->Velocity.x = maxSpeed;
		}
		else
		{
			//Bottom
			position.y = (float)windowHeight;
			DeathStar->Velocity.y = -maxSpeed;
			DeathStar->Velocity.x = -maxSpeed;
		}

	}
	else
	{
		position.y = (float)GetRandomValue(-height, height);

		if (GetRandomValue(1, 10) < 5)
		{
			//Left
			position.x = (float)-windowWidth;
			DeathStar->Velocity.x = maxSpeed;
			DeathStar->Velocity.y = maxSpeed;
		}
		else
		{
			//Right
			position.x = (float)windowWidth;
			DeathStar->Velocity.x = -maxSpeed;
			DeathStar->Velocity.y = -maxSpeed;
		}
	}

	DeathStar->Wave = Wave;
	DeathStar->Spawn(position);
	SpawnedDeathStar = true;
}

void EnemyControl::SpawnBoss()
{
	Vector3 position = {0.0f, 0.0f, 0.0f };
	float rotation = 0;
	float width = Boss->WindowWidth * 0.95f;
	float height = Boss->WindowHeight * 0.95f;

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

	Boss->Wave = Wave;

	Boss->Spawn(position, rotation);
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
	bool enemyHitRock = false;
	NoMoreRocks = true;
	RockCount = 0;

	for (int i = 0; i < Rocks.size(); i++)
	{
		if (Rocks.at(i)->Enabled)
		{
			NoMoreRocks = false;
			RockCount++;

			if (CheckUFOCollisions(Rocks.at(i))) ufoHitRock = true;

			if (CheckEnemyCollisions(Rocks.at(i))) enemyHitRock = true;

			if (Rocks.at(i)->GetBeenHit())
			{
				if (!Player->GameOver)
				{
					int chance = 30;

					chance -= (int)((Wave + 1) * 3.5f);

					if (chance < 1) chance = 1;

					if ((float)GetRandomValue(0, 100) < chance)
					{
						SpawnPowerUp = true;
						PowerUpSpawnPosition = Rocks.at(i)->Position;
					}
				}

				Rocks.at(i)->Destroy();
				Managers.EM.ResetTimer(DeathStarSpawnTimerID);
				Particles->SpawnLineParticles(Rocks.at(i)->Position,
					Vector3Multiply(Rocks.at(i)->Velocity, { 0.25f, 0.25f }),
					Rocks.at(i)->Radius * 0.25f, 15.0f, 15, 1.5f, WHITE);

				int count = 0;

				int max = (int)(Wave * 0.5f) + 5;

				if (ufoHitRock) count = GetRandomValue(4, max);
				else if (enemyHitRock) count = GetRandomValue(3, max);
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

		if (ufo->CheckRockCollisions(rock)) ufoHitRock = true;

		if (rock->Size == TheRock::Small) continue;

		if (ufo->CirclesIntersect(*rock))
		{
			ufo->Hit();
			rock->Hit();
		}
	}

	return ufoHitRock;
}

bool EnemyControl::CheckEnemyCollisions(TheRock* rock)
{
	bool enemyHitRock = false;

	for (const auto& enemy : EnemyOnes)
	{
		if (enemy->Enabled && enemy->CirclesIntersect(*rock))
		{
			enemy->Hit();
			rock->Hit();
		}

		if (enemy->CheckRockCollisions(rock)) enemyHitRock = true;
	}

	for (const auto& enemy : EnemyTwos)
	{
		if (enemy->Enabled && enemy->CirclesIntersect(*rock))
		{
			enemy->Hit();
			rock->Hit();
		}

		if (enemy->CheckRockCollisions(rock)) enemyHitRock = true;
	}

	return enemyHitRock;
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
		if (enemy->Enabled) allClear = false;

		for (const auto& shot : enemy->Shots)
		{
			if (shot->Enabled) allClear = false;
		}

		for (const auto& missile : enemy->Missiles)
		{
			if (missile->Enabled) allClear = false;
		}
	}

	for (const auto& enemy : EnemyTwos)
	{
		if (enemy->Enabled) allClear = false;

		for (const auto& mine : enemy->Mines)
		{
			if (mine->Enabled) allClear = false;
		}

		for (const auto& shot : enemy->Shots)
		{
			if (shot->Enabled) allClear = false;
		}
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

void EnemyControl::HaveHomingMineChaseEnemy()
{
	for (const auto& mine : Player->Mines)
	{
		if (!mine->Enabled) continue;

		float distance = 450.0f;
		Entity *closestEnemy = nullptr;

		for (const auto& ufo : UFOs)
		{
			if (!ufo->Enabled) continue;

			float ufoDistance = Vector3Distance(ufo->Position, mine->Position);

			if (ufoDistance < distance)
			{
				distance = ufoDistance;
				closestEnemy = ufo;
				break;
			}
		}

		for (const auto& enemy : EnemyOnes)
		{
			if (enemy->Enabled)
			{
				float enemyOneDistance = Vector3Distance(enemy->Position, mine->Position);

				if (enemyOneDistance < distance)
				{
					distance = enemyOneDistance;
					closestEnemy = enemy;
					break;
				}
			}
		}

		for (const auto& enemy : EnemyTwos)
		{
			if (enemy->Enabled)
			{
				float enemyTwoDistance = Vector3Distance(enemy->Position, mine->Position);

				if (enemyTwoDistance < distance)
				{
					distance = enemyTwoDistance;
					closestEnemy = enemy;
					break;
				}
			}
		}

		mine->ChaseEnemy(closestEnemy);
	}
}