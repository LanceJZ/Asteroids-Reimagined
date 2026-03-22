#include "Shot.h"

Shot::Shot()
{
	LifeTimerID = EM.AddTimer();
}

Shot::~Shot()
{
}

bool Shot::Initialize()
{
	LineModel::Initialize();

	return false;
}

bool Shot::BeginRun()
{
	Enabled = false;

	return false;
}

void Shot::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

}

void Shot::FixedUpdate(float deltaTime)
{
	LineModel::FixedUpdate(deltaTime);

	if (EM.TimerElapsed(LifeTimerID)) Destroy();

	CheckScreenEdge();
}

void Shot::Draw3D()
{
	LineModel::Draw3D();
}

void Shot::Spawn(Vector3 position)
{
	Entity::Spawn(position);
}

void Shot::Spawn(Vector3 position, Vector3 velocity)
{
	Spawn(position);
	Velocity = velocity;
}

void Shot::Spawn(Vector3 position, Vector3 velocity, float lifetime)
{
	EM.ResetTimer(LifeTimerID, lifetime);
	Spawn(position, velocity);
}

void Shot::Destroy()
{
	Entity::Destroy();
}