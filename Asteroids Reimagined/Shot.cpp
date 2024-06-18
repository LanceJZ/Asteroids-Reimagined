#include "Shot.h"

Shot::Shot()
{
	TheManagers.EM.AddTimer(LifeTimerID = TheManagers.EM.AddTimer());
	Enabled = false;
}

Shot::~Shot()
{
}

bool Shot::Initialize()
{
	LineModel::Initialize(TheUtilities);

	return false;
}

bool Shot::BeginRun()
{
	LineModel::BeginRun();

	return false;
}

void Shot::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	if (TheManagers.EM.TimerElapsed(LifeTimerID))
	{
		Destroy();
	}

	CheckScreenEdge();
}

void Shot::Draw()
{
	LineModel::Draw();
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
	TheManagers.EM.ResetTimer(LifeTimerID, lifetime);
	Spawn(position, velocity);
}

void Shot::Destroy()
{
	Entity::Destroy();
}