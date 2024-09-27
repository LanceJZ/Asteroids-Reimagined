#include "TheFighter.h"

TheFighter::TheFighter()
{
}

TheFighter::~TheFighter()
{
}

void TheFighter::SetEnemies(Enemy* enemyOne, Enemy* enemyTwo)
{
	EnemyOne = enemyOne;
	EnemyTwo = enemyTwo;
}

void TheFighter::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
}

void TheFighter::SetParticleManager(ParticleManager* particleManager)
{
	Particles = particleManager;
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

		if (CheckUFOActive())
		{
			ChaseUFO();
		}
		else if (EnemyOne->Enabled || EnemyTwo->Enabled)
		{
			ChaseEnemy();
		}
		else if (!Player->Enabled)
		{
			if (LeaveScreen()) Destroy();

			return;
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
}

void TheFighter::Hit()
{
	Enemy::Hit();

}

void TheFighter::Destroy()
{
	Entity::Destroy();

	ClearParents();
}

void TheFighter::ChasePlayer()
{
	SetRotateVelocity(Player->Position, TurnSpeed, Speed);
}

bool TheFighter::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;
}
