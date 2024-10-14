#include "TheUFO.h"

TheUFO::TheUFO()
{
	FireTimerID = Managers.EM.AddTimer(1.75f);
	ChangeVectorTimerID = Managers.EM.AddTimer(5.50f);
}

TheUFO::~TheUFO()
{
}

bool TheUFO::Initialize(Utilities* utilities)
{
	LineModel::Initialize(TheUtilities);

	ShotTimerAmount = 1.75f;

	return false;
}

bool TheUFO::BeginRun()
{
	LineModel::BeginRun();

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
	LineModel::Update(deltaTime);

	if (CheckReachedSide())
	{
		Enabled = false;
	}

	if (Managers.EM.TimerElapsed(FireTimerID))
	{
		FireShot();
	}

	if (Managers.EM.TimerElapsed(ChangeVectorTimerID))
	{
		ChangeVector();
	}

	CheckScreenEdgeY();
	CheckCollisions();

	if (!Player->GameOver) PlayOnSound();

	if (!Player->Enabled && !Player->GameOver) Managers.EM.ResetTimer(FireTimerID);
}

void TheUFO::Draw3D()
{
	LineModel::Draw3D();
}

void TheUFO::Spawn(int spawnCount)
{
	Vector3 position = { 0, 0, 0 };
	int height = (int)(WindowHeight / 1.25f);

	position.y = (float)GetRandomValue(-height, height);

	Managers.EM.ResetTimer(FireTimerID);
	Managers.EM.ResetTimer(ChangeVectorTimerID);

	float fullScale = 1.0f;
	float fullRadius = 18.5f;
	float fullSpeed = 128.666f;
	float spawnPercent = (float)spawnCount * 0.5f;
	spawnPercent += (Player->GetScore() * 0.0001f);
	spawnPercent += (Wave);

	if (GetRandomFloat(0, 300) > spawnPercent)
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
		position.x = WindowWidth;
		Velocity.x = -MaxSpeed;
	}
	else
	{
		position.x = -WindowWidth;
		Velocity.x = MaxSpeed;
	}

	ShotTimerAmount = 1.75f - (spawnCount * 0.01f) - (Wave * 0.1f);

	if (ShotTimerAmount < 0.2f) ShotTimerAmount = 0.2f;

	Entity::Spawn(position);
}

void TheUFO::Destroy()
{
	Entity::Destroy();

	Managers.EM.ResetTimer(FireTimerID, 1.75f);
	Managers.EM.ResetTimer(ChangeVectorTimerID, 5.50f);
}

void TheUFO::Hit()
{
	Enemy::Hit();

}

void TheUFO::Reset()
{
	Destroy();

	for (const auto& shot : Shots)
	{
		shot->Destroy();
	}
}

void TheUFO::FireShot()
{
	float angle = 0;
	float shotSpeed = 325;
	bool shootRocks = false;
	Managers.EM.ResetTimer(FireTimerID, ShotTimerAmount);

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
			angle = GetRandomRadian();
		}
	}

	Managers.EM.ResetTimer(ShotTimerID);

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
		Managers.EM.AddLineModel(Shots[spawnNumber], ShotModel);
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
		return GetRandomRadian();
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

	percentChance += GetRandomFloat(minP, maxP);

	return GetAngleFromVectorZ(Player->Position) +
		GetRandomFloat(-percentChance, percentChance);
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
		return GetRandomRadian();
	}

	Vector3 compensation = GetVelocityFromAngleZ(GetAngleFromVectorZ(closestRockVelocity),
		shortestDistance);

	return GetAngleFromVectorZ(Vector3Add(closestRockPosition,
		Vector3Add(closestRockVelocity, compensation)));
}

void TheUFO::ChangeVector()
{
	float vectorTimer = GetRandomFloat(3.1f - (Wave * 0.1f), 7.5f);

	if (vectorTimer < 0.25f) vectorTimer = 0.25f;

	Managers.EM.ResetTimer(ChangeVectorTimerID,	vectorTimer);

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
			if (Position.y < WindowHeight - (Radius * 3) &&
				Position.y > -WindowHeight - (Radius * 3))
			{
				Velocity.y = 0;
			}
		}
	}
}

bool TheUFO::CheckReachedSide()
{
	if (X() < -WindowWidth)
	{
		return true;
	}
	if (X() > WindowWidth)
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
