#include "ExplosionControl.h"

ExplosionControl::ExplosionControl()
{
}

ExplosionControl::~ExplosionControl()
{
}

bool ExplosionControl::Initialize()
{
	ParticleManager::Initialize();

	return false;
}

bool ExplosionControl::BeginRun()
{
	ParticleManager::BeginRun();

	return false;
}

void ExplosionControl::FixedUpdate()
{
	ParticleManager::FixedUpdate();

}

void ExplosionControl::Spawn(Vector3 position, float radius, float speed,
	int count, float time, Color color)
{
	ParticleManager::SpawnCubes(position, {0, 0, 0}, radius, speed, count, time, color);
}
