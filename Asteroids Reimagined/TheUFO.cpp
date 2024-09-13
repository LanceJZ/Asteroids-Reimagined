#include "TheUFO.h"

TheUFO::TheUFO()
{
	FireTimerID = Managers.EM.AddTimer(1.25f);
	ChangeVectorTimerID = Managers.EM.AddTimer(5.50f);

	for (int i = 0; i < 2; i++)
	{
		Shots[i] = DBG_NEW Shot();
		Managers.EM.AddLineModel(Shots[i]);
	}
}

TheUFO::~TheUFO()
{
}

void TheUFO::SetRocks(std::vector<TheRock*> &rocks)
{
	Rocks = &rocks;
}

bool TheUFO::Initialize(Utilities* utilities)
{
	LineModel::Initialize(TheUtilities);

	return false;
}

bool TheUFO::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheUFO::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void TheUFO::SetShotModel(LineModelPoints model)
{
	for (const auto& shot : Shots)
	{
		shot->SetModel(model);
	}
}

void TheUFO::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
	SetSoundVolume(ExplodeSound, 0.5f);
}

void TheUFO::SetFireSound(Sound sound)
{
	FireSound = sound;
	SetSoundVolume(FireSound, 0.5f);
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

void TheUFO::SetParticles(ParticleManager* particles)
{
	Particles = particles;
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
}

void TheUFO::Draw3D()
{
	LineModel::Draw3D();
}

bool TheUFO::CheckShotCollisions(TheRock* rock)
{
	bool shotHit = false;

	for (const auto& shot : Shots)
	{
		if (shot->Enabled && shot->CirclesIntersect(*rock))
		{
			shot->Destroy();
			rock->Hit();
			shotHit = true;
			continue;
		}
	}

	return shotHit;
}

void TheUFO::CheckShotPlayerCollisions() //Move to Enemy class.
{
	if (!Player->Enabled) return;

	for (const auto& shot : Shots)
	{
		if (shot->Enabled && shot->CirclesIntersect(*Player))
		{
			shot->Destroy();
			Player->Hit(Position, Velocity);
			break;
		}
	}
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

	Entity::Spawn(position);
}

void TheUFO::Destroy()
{
	Entity::Destroy();

}

void TheUFO::Hit()
{
	Entity::Hit();

	if (!Player->GameOver) PlaySound(ExplodeSound);

	Particles->SpawnLineParticles(Position, Vector3Multiply(Velocity, { 0.25f }),
		Radius * 0.25f, 20.0f, 30, 2.0f, WHITE);

	Destroy();
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
	Managers.EM.ResetTimer(FireTimerID);

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

	for (const auto& shot : Shots)
	{
		if (!shot->Enabled)
		{
			if (!Player->GameOver) PlaySound(FireSound);

			Vector3 position = Vector3Add(VelocityFromAngleZ(Radius), Position);
			shot->Spawn(position, GetVelocityFromAngleZ(angle, shotSpeed), 2.5f);

			break;
		}
	}
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

	return AngleFromVectorZ(Player->Position) +
		GetRandomFloat(-percentChance, percentChance);
}

float TheUFO::AimedShotAtDeathStar()
{
	return AngleFromVectorZ(DeathStarPosition);
}

float TheUFO::AimedShotAtRock()
{
	bool noRocks = true;

	//Vector3 closestRockPosition = { GetRandomFloat(-WindowWidth, WindowWidth),
	//	GetRandomFloat(-WindowHeight, WindowHeight), 0 };

	Vector3 closestRockPosition = { 0, 0, 0 };
	Vector3 closestRockVelocity = { 0, 0, 0 };
	float shortestDistance = 1000.0f;

	for (const auto &rock : *Rocks)
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

	Vector3 compensation = GetVelocityFromAngleZ(AngleFromVectorZ(closestRockVelocity),
		shortestDistance);

	return AngleFromVectorZ(Vector3Add(closestRockPosition,
		Vector3Add(closestRockVelocity, compensation)));
}

void TheUFO::ChangeVector()
{
	Managers.EM.ResetTimer(ChangeVectorTimerID, GetRandomFloat(3.1f, 7.5f));

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
