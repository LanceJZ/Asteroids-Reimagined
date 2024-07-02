#include "Enemy1.h"

Enemy1::Enemy1()
{
	TheManagers.EM.AddLineModel(Missile = DBG_NEW TheMissile());
	FireMissileTimerID = TheManagers.EM.AddTimer(2.0f);
}

Enemy1::~Enemy1()
{
}

void Enemy1::SetPlayer(ThePlayer* player)
{
	Enemy::SetPlayer(player);

	Missile->SetPlayer(player);
}

void Enemy1::SetMissileModel(LineModelPoints model)
{
	Missile->SetModel(model);
}

bool Enemy1::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	Enabled = false;
	Points = 1250;

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

	if (TheManagers.EM.TimerElapsed(FireMissileTimerID))
	{
		if (!Missile->Enabled && Player->Enabled) FireMissile();

		TheManagers.EM.ResetTimer(FireMissileTimerID);
	}

	DestinationTarget();
	CheckCollisions();
	if (CheckWentOffScreen()) Enabled = false;
}

void Enemy1::Draw3D()
{
	LineModel::Draw3D();
}

void Enemy1::Spawn(Vector3 position)
{
	TheManagers.EM.ResetTimer(FireMissileTimerID);

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
	Missile->SetUFO(UFOs);
}

void Enemy1::Destroy()
{
	Entity::Destroy();

}

bool Enemy1::CheckWentOffScreen()
{
	if (EdgeSpawnedFrom == Edge::Right || EdgeSpawnedFrom == Edge::Left)
	{
		if (X() < -WindowWidth)
		{
			return true;
		}
		if (X() > WindowWidth)
		{
			return true;
		}
	}
	else if (EdgeSpawnedFrom == Edge::Top || EdgeSpawnedFrom == Edge::Bottom)
	{
		if (Y() < -WindowHeight)
		{
			return true;
		}
		if (Y() > WindowHeight)
		{
			return true;
		}
	}

	CheckScreenEdge();

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
		Destination.x = (-WindowWidth * 0.5f) + (Player->X() - (WindowWidth * 0.25f));
	}
	else
	{
		Destination.x = (WindowWidth * 0.5f) - (Player->X() - (WindowWidth * 0.25f));
	}
}

void Enemy1::DestinationBottom()
{
	DestinationTop();
}

void Enemy1::DestinationLeft()
{
	if (Player->Y() > Y())
	{
		Destination.y = (-WindowHeight * 0.5f) + (Player->Y() - (WindowHeight * 0.25f));
	}
	else
	{
		Destination.y = (WindowHeight * 0.5f) - (Player->Y() - (WindowHeight * 0.25f));
	}
}

void Enemy1::DestinationRight()
{
	DestinationLeft();
}

void Enemy1::FireMissile()
{
	Missile->Spawn(Position);
	Missile->RotationZ = RotationZ;
}

bool Enemy1::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;
}
