#include "TheUFO.h"

TheUFO::TheUFO()
{
	FireTimerID = EM.AddTimer(1.75f);
	ChangeVectorTimerID = EM.AddTimer(5.50f);
}

TheUFO::~TheUFO()
{
}

bool TheUFO::Initialize()
{
	Enemy::Initialize();

	ShotTimerAmount = 1.75f;

	return false;
}

bool TheUFO::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheUFO::SetBigSound(Sound sound)
{
	BigSound = sound;
	SetSoundVolume(BigSound, 0.5f);
}

void TheUFO::SetSmallSound(Sound sound)
{
	SmallSound = sound;
	SetSoundVolume(SmallSound, 0.5f);
}

void TheUFO::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	CheckCollisions();
}

void TheUFO::FixedUpdate(float deltaTime)
{
	Enemy::FixedUpdate(deltaTime);

	CheckScreenEdgeY();

	if (CheckReachedSide())
	{
		Enabled = false;
	}

	if (EM.TimerElapsed(FireTimerID))
	{
		FireShot();
	}

	if (EM.TimerElapsed(ChangeVectorTimerID))
	{
		ChangeVector();
	}

	//if (!Player->GameOver) PlayOnSound();

	if (!Player->Enabled && !Player->GameOver) EM.ResetTimer(FireTimerID);
}

void TheUFO::AlwaysUpdate(float deltaTime)
{
	Enemy::AlwaysUpdate(deltaTime);

	//CheckShotsHitPlayer();
}

void TheUFO::Draw3D()
{
	Enemy::Draw3D();
}

void TheUFO::Spawn(int spawnCount)
{
	Vector3 position = { 0, 0, 0 };
	int height = (int)(WindowHalfHeight / 1.25f);

	position.y = (float)GetRandomValue(-height, height);

	EM.ResetTimer(FireTimerID);
	EM.ResetTimer(ChangeVectorTimerID);

	float fullScale = 1.0f;
	float fullRadius = 18.5f;
	float fullSpeed = 128.666f;
	float spawnPercent = (float)spawnCount * 0.1f;
	TraceLog(LOG_INFO, "UFO Spawn Count: %i", spawnCount);
	spawnPercent += (float)(Player->GetScore() * 0.0001f);
	spawnPercent += (float)(Wave) * 0.25f;
	TraceLog(LOG_INFO, "Wave: %i", Wave);


	if (spawnPercent > 90) spawnPercent = 90;
	if (spawnPercent < 1) spawnPercent = 1;
	//TODO:Needs testing.
	int randvalue = GetRandomValue(1, 100);

	TraceLog(LOG_INFO, "UFO SpawnPercent: %i", (int)spawnPercent);
	TraceLog(LOG_INFO, "RandValue: %i", randvalue);

	if (randvalue > (int)spawnPercent)
	{
		TheSize = Large;
		Scale = fullScale;
		MaxSpeed = fullSpeed / 1.333f;
		Radius = fullRadius;
		Points = 200;
	}
	else
	{
		TheSize = Small;
		Scale = fullScale / 2;
		MaxSpeed = fullSpeed;
		Radius = fullRadius / 2;
		Points = 1000;
	}

	if (GetRandomValue(1, 10) < 5)
	{
		position.x = (float)WindowHalfWidth;
		Velocity.x = -MaxSpeed;
	}
	else
	{
		position.x = (float)-WindowHalfWidth;
		Velocity.x = MaxSpeed;
	}

	switch(TheSize)
	{
	case Large:
		if (!IsSoundPlaying(BigSound)) OnSound = BigSound;
		break;
	case Small:
		if (!IsSoundPlaying(SmallSound)) SmallSound;
		break;
	}

	ShotTimerAmount = 1.75f - (spawnCount * 0.01f) - (Wave * 0.1f);

	if (ShotTimerAmount < 0.2f) ShotTimerAmount = 0.2f;

	Entity::Spawn(position);
}

void TheUFO::Destroy()
{
	Enemy::Destroy();

	EM.ResetTimer(FireTimerID, 1.75f);
	EM.ResetTimer(ChangeVectorTimerID, 5.50f);
}

void TheUFO::Hit()
{
	Enemy::Hit();

	Destroy();
}

void TheUFO::Reset()
{
	Enemy::Reset();

	Destroy();
}

void TheUFO::FireShot()
{
	float angle = 0;
	float shotSpeed = 325;
	bool shootRocks = false;
	EM.ResetTimer(FireTimerID, ShotTimerAmount);

	if (DeathStarActive)
	{
		angle = AimedShotAtDeathStar();
		//printf("UFO shot at Death Star\n");
	}
	else
	{
		if (GetRandomValue(1, 10) < 5 || !Player->Enabled)
		{
			angle = AimedShotAtRock();
			//printf("UFO shot at rock\n");
		}
		else
		{
			angle = AimedShot();
			//printf("UFO shot at player\n");
		}
	}

	if (TheUFO::Large)
	{
		if (GetRandomValue(1, 10) < 3)
		{
			angle = M.GetRandomRadian();
		}
	}

	EM.ResetTimer(ShotTimerID);

	bool spawnNew = true;
	size_t spawnNumber = Shots.size();

	for (size_t check = 0; check < spawnNumber; check++)
	{
		if (!Shots[check]->Enabled)
		{
			spawnNew = false;
			spawnNumber = check;
			break;
		}
	}

	if (spawnNew)
	{
		Shots.push_back(DBG_NEW Shot());
		EM.AddLineModel(Shots[spawnNumber], ShotModel);
		Shots[spawnNumber]->BeginRun();
	}

	if (!Player->GameOver) PlaySound(FireSound);

	Vector3 position = Vector3Add(GetVelocityFromAngleZ(Radius), Position);
	Shots[spawnNumber]->Spawn(position, GetVelocityFromAngleZ(angle, shotSpeed), 2.5f);
}

float TheUFO::AimedShot()
{
	if (!Player->Enabled)
	{
		return M.GetRandomRadian();
	}

	float percentChance = 0.2f - (Player->GetScore() * 0.00001f);

	if (percentChance < 0)
	{
		percentChance = 0;
	}

	float minP = 0.015f - (0.01f * 0.0025f);

	if (minP < 0)
		minP = 0;

	float maxP = 0.02f + minP;

	percentChance += M.GetRandomFloat(minP, maxP);

	return GetAngleFromVectorZ(Player->Position) +
		M.GetRandomFloat(-percentChance, percentChance);
}

float TheUFO::AimedShotAtDeathStar()
{
	return GetAngleFromVectorZ(DeathStarPosition);
}

float TheUFO::AimedShotAtRock()
{
	bool noRocks = true;

	Vector3 closestRockPosition = { 0, 0, 0 };
	Vector3 closestRockVelocity = { 0, 0, 0 };
	float shortestDistance = 1000.0f;

	for (const auto &rock : Rocks)
	{
		if (rock->Enabled)
		{
			noRocks = false;
			float distance = Vector3Distance(rock->Position, Position);

			if (distance < shortestDistance)
			{
				shortestDistance = distance;
				closestRockPosition = rock->Position;
				closestRockVelocity = rock->Velocity;
			}
		}
	}

	if (noRocks)
	{
		return M.GetRandomRadian();
	}

	Vector3 compensation = GetVelocityFromAngleZ(GetAngleFromVectorZ(closestRockVelocity),
		shortestDistance);

	Vector3 closestRock = Vector3Add(closestRockPosition,
		Vector3Add(closestRockVelocity, compensation));

	return GetAngleFromVectorZ(closestRock);
}

void TheUFO::ChangeVector()
{
	float vectorTimer = M.GetRandomFloat(3.1f - (Wave * 0.1f), 7.5f);

	if (vectorTimer < 0.25f) vectorTimer = 0.25f;

	EM.ResetTimer(ChangeVectorTimerID,	vectorTimer);

	if (GetRandomValue(1, 10) > 2)
	{
		if ((int)Velocity.y == 0)
		{
			if (GetRandomValue(1, 10) < 5)
			{
				Velocity.y = MaxSpeed;
			}
			else
			{
				Velocity.y = -MaxSpeed;
			}
		}
		else
		{
			if (Position.y < WindowHalfHeight - (Radius * 3) &&
				Position.y > -WindowHalfHeight - (Radius * 3))
			{
				Velocity.y = 0;
			}
		}
	}
}

bool TheUFO::CheckReachedSide()
{
	if (X() < -WindowHalfWidth)
	{
		return true;
	}
	if (X() > WindowHalfWidth)
	{
		return true;
	}

	return false;
}

bool TheUFO::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;
}

void TheUFO::SendScoreToPlayer()
{
	switch (TheSize)
	{
	case Large:
		Player->ScoreUpdate(200);
		break;
	case Small:
		Player->ScoreUpdate(1000);
		break;
	}
}

void TheUFO::PlayOnSound()
{
	switch(TheSize)
	{
	case Large:
		if (!IsSoundPlaying(BigSound)) PlaySound(BigSound);
		break;
	case Small:
		if (!IsSoundPlaying(SmallSound)) PlaySound(SmallSound);
		break;
	}
}
