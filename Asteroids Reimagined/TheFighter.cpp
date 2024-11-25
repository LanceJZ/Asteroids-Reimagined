#include "TheFighter.h"

TheFighter::TheFighter()
{
}

TheFighter::~TheFighter()
{
}

void TheFighter::SetExplodeSound(Sound sound)
{
	Enemy::SetExplodeSound(sound);
}

bool TheFighter::Initialize(Utilities* utilities)
{
	LineModel::Initialize(TheUtilities);

	Enabled = false;

	return false;
}

bool TheFighter::BeginRun()
{
	LineModel::BeginRun();

	Points = 200;
	Speed = 150.0f;
	TurnSpeed = 0.45f;
	RotateMagnitude = PI / 2;

	return false;
}

void TheFighter::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	if (Separated)
	{
		CheckCollisions();
	}
}

void TheFighter::FixedUpdate(float deltaTime)
{
	Enemy::FixedUpdate(deltaTime);

	if (Separated)
	{
		if (!Player->Enabled)
		{
			if (CheckUFOActive())
			{
				ChaseUFO();
				return;
			}
			else if (CheckEnemyActive())
			{
				ChaseEnemy();
				return;
			}

			if (LeaveScreen()) Destroy();

			return;
		}
		else
		{
			ChasePlayer();
		}

		if (NewWave)
		{
			LeaveScreen();
		}
		else
		{
			CheckScreenEdge();
		}
	}
}

void TheFighter::Draw3D()
{
	LineModel::Draw3D();
}

void TheFighter::Separate()
{
	Separated = true;
	Position = GetWorldPosition();
	RotationZ = WorldRotation.z;
	IsChild = false;
}

void TheFighter::Reset()
{
	Enemy::Reset();

}

void TheFighter::Spawn(Vector3 position)
{
	Enabled = true;
	Separated = false;
	NewWave = false;

	Speed = 150.0f + ((float)Wave - 1) * 5.5f;
}

void TheFighter::Hit()
{
	Enemy::Hit();

}

void TheFighter::Destroy()
{
	Enemy::Destroy();

	ClearParents();
}

bool TheFighter::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;
}
