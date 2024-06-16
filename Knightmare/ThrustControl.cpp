#include "ThrustControl.h"

ThrustControl::ThrustControl()
{

}

ThrustControl::~ThrustControl()
{
}

bool ThrustControl::Initialize(Utilities* utilities)
{
	//ParticleManager::Initialize(utilities);

	return false;
}

bool ThrustControl::BeginRun()
{
	ParticleManager::BeginRun();

	return false;
}

void ThrustControl::Update()
{
	ParticleManager::Update();

}

void ThrustControl::SpawnExplosion(Vector3 position, Vector3 velocity, float endWidth,
	float length, int count, float time, Color color)
{
	float size = 3.1f;
	float speed = 5.1f;

	float width = GetRandomFloat(-endWidth, endWidth);

	velocity = Vector3Add(GetVelocityFromAngleZ(width, length), velocity);

	ParticleManager::SpawnExplosion(position, velocity, size, speed, count, time, color);
}
