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
}

void TheRock::Draw3D()
{
	LineModel::Draw3D();
}

void TheRock::Spawn(Vector3 position, RockSize size)
{
	Entity::Spawn(position);

	float magnitude = GetRandomFloat(30.1f, 50.1f);
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
		magnitude = GetRandomFloat(40.1f, 60.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);
		break;
	case Medium:
		change = 1.85f;
		Scale =  scale / change;
		Radius = radius / change;
		maxVS = 2;
		magnitude = GetRandomFloat(30.1f, 50.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);
		break;
	case Large:
		Y(GetRandomFloat(-WindowHeight, WindowHeight));
		Scale = scale;
		Radius = radius;
		maxVS = 1;

		magnitude = GetRandomFloat(20.1f, 40.1f);
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