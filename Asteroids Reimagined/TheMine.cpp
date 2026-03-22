#include "TheMine.h"

TheMine::TheMine()
{
	LifeTimerID = EM.AddTimer(10.0f);
}

TheMine::~TheMine()
{
}

bool TheMine::Initialize()
{
	LineModel::Initialize();

	return false;
}

bool TheMine::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void TheMine::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	CheckCollisions();
}

void TheMine::FixedUpdate(float deltaTime)
{
	LineModel::FixedUpdate(deltaTime);

	if (EM.TimerElapsed(LifeTimerID)) Destroy();
}

void TheMine::Draw3D()
{
	LineModel::Draw3D();
}

void TheMine::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	EM.ResetTimer(LifeTimerID);
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
