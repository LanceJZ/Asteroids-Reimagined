#include "Shot.h"

Shot::Shot()
{
	LifeTimerID = Managers.EM.AddTimer();
}

Shot::~Shot()
{
}

bool Shot::Initialize(Utilities* utilities)
{
	LineModel::Initialize(TheUtilities);

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

	if (Managers.EM.TimerElapsed(LifeTimerID))
	{
		Destroy();
	}

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
	Managers.EM.ResetTimer(LifeTimerID, lifetime);
	Spawn(position, velocity);
}

void Shot::Destroy()
{
	Entity::Destroy();
}