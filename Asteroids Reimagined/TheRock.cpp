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

bool TheRock::Initialize()
{
	LineModel::Initialize(TheUtilities);

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

	CheckScreenEdge();
	CheckCollisions();
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
	float radius = 47.25f;
	float change = 0;
	Enabled = true;
	BeenHit = false;

	switch (size)
	{
	case Small:
		change = 4.5f;
		Scale = scale / change;
		Radius = radius / change;
		maxVS = 3;
		magnitude = GetRandomFloat(52.3f, 74.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);
		break;
	case Medium:
		change = 2.75f;
		Scale =  scale / change;
		Radius = radius / change;
		maxVS = 2;
		magnitude = GetRandomFloat(43.2f, 63.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);
		break;
	case MediumLarge:
		change = 1.85f;
		Scale =  scale / change;
		Radius = radius / change;
		maxVS = 2;
		magnitude = GetRandomFloat(36.1f, 56.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);
		break;
	case Large:
		Y(GetRandomFloat(-WindowHeight, WindowHeight));
		Scale = scale;
		Radius = radius;
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

bool TheRock::CheckCollisions()
{
	if (Player->Enabled && CirclesIntersect(*Player))
	{
		if (!Player->Shield->Enabled) Destroy();

		Player->Hit(Position, Velocity);

		return true;
	}

	for (auto& shot : Player->Shots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			return true;
		}
	}

	return false;
}
