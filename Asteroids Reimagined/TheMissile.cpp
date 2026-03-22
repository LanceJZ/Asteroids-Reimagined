#include "TheMissile.h"

TheMissile::TheMissile()
{
	LifeTimerID = EM.AddTimer(5.0f);
}

TheMissile::~TheMissile()
{
}

void TheMissile::SetOnSound(Sound sound)
{
	OnSound = sound;
}

bool TheMissile::Initialize()
{
	Enemy::Initialize();

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

	CheckCollisions();
}

void TheMissile::FixedUpdate(float deltaTime)
{
	Enemy::FixedUpdate(deltaTime);

	if (EM.TimerElapsed(LifeTimerID))
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

	EM.ResetTimer(LifeTimerID);
}

void TheMissile::Hit()
{
	Enemy::Hit();

	Destroy();
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
