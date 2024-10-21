#include "TheMine.h"

TheMine::TheMine()
{
	LifeTimerID = Managers.EM.AddTimer(10.0f);
}

TheMine::~TheMine()
{
}

bool TheMine::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool TheMine::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheMine::Update(double deltaTime)
{
	LineModel::Update(deltaTime);

	CheckCollisions();
}

void TheMine::FixedUpdate(double deltaTime)
{
	LineModel::FixedUpdate(deltaTime);

	if (Managers.EM.TimerElapsed(LifeTimerID)) Destroy();
}

void TheMine::Draw3D()
{
	LineModel::Draw3D();
}

void TheMine::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	Managers.EM.ResetTimer(LifeTimerID);
}

void TheMine::Hit()
{
	Enemy::Hit();

	Destroy();
}

void TheMine::Destroy()
{
	Entity::Destroy();

}

bool TheMine::CheckCollisions()
{
	Enemy::CheckCollisions();

	return false;
}
