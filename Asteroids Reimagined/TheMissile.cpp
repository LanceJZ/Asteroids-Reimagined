#include "TheMissile.h"

TheMissile::TheMissile()
{
	LifeTimerID = Managers.EM.AddTimer(5.0f);
}

TheMissile::~TheMissile()
{
}

bool TheMissile::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	Enabled = false;
	Points = 500;

	return false;
}

bool TheMissile::BeginRun()
{
	Enemy::BeginRun();

	SetSoundVolume(ExplodeSound, 0.5f);

	return false;
}

void TheMissile::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	if (Managers.EM.TimerElapsed(LifeTimerID))
	{
		Destroy();
	}

	ChasePlayer();
	CheckScreenEdge();
	CheckCollisions();
}

void TheMissile::Draw3D()
{
	LineModel::Draw3D();
}

void TheMissile::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	Managers.EM.ResetTimer(LifeTimerID);
}

void TheMissile::Hit()
{
	Enemy::Hit();
}

void TheMissile::Destroy()
{
	Entity::Destroy();

}

bool TheMissile::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;
}
