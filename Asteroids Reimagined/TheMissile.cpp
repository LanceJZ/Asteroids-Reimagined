#include "TheMissile.h"

TheMissile::TheMissile()
{
	LifeTimerID = TheManagers.EM.AddTimer(5.0f);
}

TheMissile::~TheMissile()
{
}

bool TheMissile::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	Enabled = false;

	return false;
}

bool TheMissile::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheMissile::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	if (TheManagers.EM.TimerElapsed(LifeTimerID))
	{
		Destroy();
	}

	ChasePlayer();
	CheckScreenEdge();
}

void TheMissile::Draw3D()
{
	LineModel::Draw3D();
}

void TheMissile::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	TheManagers.EM.ResetTimer(LifeTimerID);
}

void TheMissile::Destroy()
{
	Entity::Destroy();

}
