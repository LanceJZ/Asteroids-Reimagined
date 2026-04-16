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
	Enemy::Initialize();

	return false;
}

bool TheMine::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheMine::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

}

void TheMine::FixedUpdate(float deltaTime)
{
	Enemy::FixedUpdate(deltaTime);

	if (EM.TimerElapsed(LifeTimerID)) Destroy();

	CheckCollisions();
}

void TheMine::Draw3D()
{
	Enemy::Draw3D();
}

void TheMine::Spawn(Vector3 position)
{
	Enemy::Spawn(position);

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
