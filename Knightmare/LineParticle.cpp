#include "LineParticle.h"

LineParticle::LineParticle()
{
}

LineParticle::~LineParticle()
{
}

void LineParticle::SetManagers(TheManagers* managers)
{
	Managers = managers;
}

bool LineParticle::Initialize(Utilities* utilities)
{
	LineModel::Initialize(utilities);

	return false;
}

bool LineParticle::BeginRun()
{
	LineModel::BeginRun();

	LifeTimerID = Managers->EM.AddTimer();

	return false;
}

void LineParticle::Update(double deltaTime)
{
	LineModel::Update(deltaTime);

	if (Managers->EM.TimerElapsed(LifeTimerID))
	{
		Enabled = false;
	}
}

void LineParticle::Draw3D()
{
	LineModel::Draw3D();
}

void LineParticle::Spawn(Vector3 position, Vector3 velocity, float radius, float speed, float time)
{
	Enabled = true;
	Vector3 spawnPosition = position;
	spawnPosition.x += GetRandomFloat(-radius, radius);
	spawnPosition.y += GetRandomFloat(-radius, radius);
	Position = spawnPosition;

	Vector3 addedVelocity = GetRandomVelocity(GetRandomFloat(speed * 0.25f, speed));
	Velocity = Vector3Add(velocity, addedVelocity);

	Managers->EM.ResetTimer(LifeTimerID, time);
}

void LineParticle::Destroy()
{
	Entity::Destroy();

}
