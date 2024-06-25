#include "Enemy1.h"

Enemy1::Enemy1()
{
}

Enemy1::~Enemy1()
{
}

bool Enemy1::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	Enabled = false;

	return false;
}

bool Enemy1::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void Enemy1::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	SetRotateVelocity(Destination, TurnSpeed, Speed);

	if (CheckWentOffScreen()) Enabled = false;

	DestinationTarget();
}

void Enemy1::Draw3D()
{
	LineModel::Draw3D();
}

void Enemy1::Spawn(Vector3 position)
{
	MaxSpeed = 133.666f;

	int width = WindowWidth / 1.25f;
	int height = WindowHeight / 1.25f;

	if (GetRandomValue(1, 10) < 5)
	{
		if (GetRandomValue(1, 10) < 5)
		{
			// Top
			EdgeSpawnedFrom = Edge::Top;
			position.y = -WindowHeight;
			position.x = GetRandomValue(-width, width);
			Velocity.y = MaxSpeed;
			Destination = { position.x, WindowHeight, 0 };
		}
		else
		{
			//Bottom
			EdgeSpawnedFrom = Edge::Bottom;
			position.y = WindowHeight;
			position.x = GetRandomValue(-width, width);
			Velocity.y = -MaxSpeed;
			Destination = { position.x, -WindowHeight, 0 };
		}

	}
	else
	{
		if (GetRandomValue(1, 10) < 5)
		{
			//Left
			EdgeSpawnedFrom = Edge::Left;
			position.x = -WindowWidth;
			position.y = GetRandomValue(-height, height);
			Velocity.x = MaxSpeed;
			Destination = { WindowWidth, position.y, 0 };
		}
		else
		{
			//Right
			EdgeSpawnedFrom = Edge::Right;
			position.x = WindowWidth;
			position.y = GetRandomValue(-height, height);
			Velocity.x = -MaxSpeed;
			Destination = { -WindowWidth, position.y, 0 };
		}
	}

	Enemy::Spawn(position);
	RotationZ = AngleFromVectorZ(Destination);

}

void Enemy1::Destroy()
{
	Entity::Destroy();

}

bool Enemy1::CheckWentOffScreen()
{
	if (X() < -WindowWidth)
	{
		return true;
	}
	if (X() > WindowWidth)
	{
		return true;
	}
	if (Y() < -WindowHeight)
	{
		return true;
	}
	if (Y() > WindowHeight)
	{
		return true;
	}

	return false;
}

void Enemy1::DestinationTarget()
{
	switch (EdgeSpawnedFrom)
	{
	case Edge::Top:
		DestinationBottom();
		break;
	case Edge::Bottom:
		DestinationTop();
		break;
	case Edge::Left:
		DestinationRight();
		break;
	case Edge::Right:
		DestinationLeft();
		break;
	}
}

void Enemy1::DestinationTop()
{
	if (Player->X() > X())
	{
		Destination.x = (-WindowWidth * 0.75f);
	}
	else
	{
		Destination.x = (WindowWidth * 0.75f);
	}
}

void Enemy1::DestinationBottom()
{
	if (Player->X() > X())
	{
		Destination.x = (-WindowWidth * 0.75f);
	}
	else
	{
		Destination.x = (WindowWidth * 0.75f);
	}
}

void Enemy1::DestinationLeft()
{
	if (Player->Y() > Y())
	{
		Destination.y = (-WindowHeight * 0.75f);
	}
	else
	{
		Destination.y = (WindowHeight * 0.75f);
	}
}

void Enemy1::DestinationRight()
{
	if (Player->Y() > Y())
	{
		Destination.y = (-WindowHeight * 1.75f);
	}
	else
	{
		Destination.y = (WindowHeight * 1.75f);
	}
}
