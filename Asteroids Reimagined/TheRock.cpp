#include "TheRock.h"

TheRock::TheRock()
{
}

TheRock::~TheRock()
{
}

void TheRock::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void TheRock::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
	SetSoundVolume(ExplodeSound, 0.333f);
}

bool TheRock::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool TheRock::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheRock::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	CheckCollisions();
}

void TheRock::FixedUpdate(float deltaTime)
{
	LineModel::FixedUpdate(deltaTime);

	CheckScreenEdge();
}

void TheRock::Draw3D()
{
	LineModel::Draw3D();
}

void TheRock::Spawn(Vector3 position, RockSize size)
{
	Entity::Spawn(position);

	float magnitude = 0;
	float angle = GetRandomRadian();
	float maxVS = 0;

	TheRock::Size = size;
	float scale = 25;
	float change = 0;

	switch (size)
	{
	case Small:
		change = 4.5f;
		Scale = scale / change;
		maxVS = 3;
		magnitude = GetRandomFloat(52.3f, 74.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);
		break;
	case Medium:
		change = 2.75f;
		Scale =  scale / change;
		maxVS = 2;
		magnitude = GetRandomFloat(43.2f, 63.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);
		break;
	case MediumLarge:
		change = 1.85f;
		Scale =  scale / change;
		maxVS = 2;
		magnitude = GetRandomFloat(36.1f, 56.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);
		break;
	case Large:
		Y(GetRandomFloat(-WindowHeight, WindowHeight));
		Scale = scale;
		maxVS = 1;

		magnitude = GetRandomFloat(20.35f, 40.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);

		if (Velocity.x > 0)
		{
			X(-WindowWidth);
		}
		else
		{
			X(WindowWidth);
		}

		break;
	}

	float rVel = GetRandomFloat(-maxVS, maxVS);

	if (rVel < 0.1)
	{
		rVel++;
	}
	else if (rVel > -0.1)
	{
		rVel--;
	}

	RotationVelocityZ = rVel;
}

void TheRock::Destroy()
{
	Entity::Destroy();

}

void TheRock::Hit()
{
	Entity::Hit();

	if (!Player->GameOver) PlaySound(ExplodeSound);
}

bool TheRock::CheckCollisions()
{
	if (Player->Enabled && CirclesIntersect(*Player))
	{
		if (!Player->Shield->Enabled) Hit();

		Player->Hit(Position, Velocity);
		SendScoreToPlayer();

		return true;
	}

	for (const auto& shot : Player->Shots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			SendScoreToPlayer();
			return true;
		}
	}

	for (const auto& shot : Player->DoubleShots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			SendScoreToPlayer();
			return true;
		}
	}

	for (const auto& bigShot : Player->BigShots)
	{
		if (bigShot->Enabled && CirclesIntersect(*bigShot))
		{
			bigShot->HitPoints -= 20;

			if (bigShot->HitPoints <= 0) bigShot->Destroy();

			Hit();
			SendScoreToPlayer();
			return true;
		}
	}

	for (const auto& plasmaShot : Player->PlasmaShots)
	{
		if (plasmaShot->Enabled && CirclesIntersect(*plasmaShot))
		{
			Hit();
			SendScoreToPlayer();
			return true;
		}
	}

	return false;
}

void TheRock::SendScoreToPlayer()
{
	switch (Size)
	{
	case Large:
		Player->ScoreUpdate(20);
		break;
	case Medium:
		Player->ScoreUpdate(50);
		break;
	case Small:
		Player->ScoreUpdate(100);
		break;
	}
}
