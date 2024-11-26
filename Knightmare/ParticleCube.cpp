#include "ParticleCube.h"
ParticleCube::ParticleCube()
{
}

ParticleCube::~ParticleCube()
{
}

void ParticleCube::SetManagers(EntityManager* managers)
{
	EM = managers;

	LifeTimerID = EM->AddTimer();
}

bool ParticleCube::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	return false;
}

bool ParticleCube::BeginRun()
{
	Model3D::BeginRun();

	return false;
}

void ParticleCube::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

	if (EM->TimerElapsed(LifeTimerID))
	{
		Enabled = false;
	}
}

void ParticleCube::FixedUpdate(float deltaTime)
{
	Model3D::FixedUpdate(deltaTime);

}

void ParticleCube::Draw3D()
{
	Model3D::Draw3D();

}

void ParticleCube::Spawn(Vector3 position, Vector3 velocity, float radius,
	float speed, float time)
{
	Enabled = true;

	Vector3 spawnPos = position;
	spawnPos.x += GetRandomFloat(-radius, radius);
	spawnPos.y += GetRandomFloat(-radius, radius);
	Position = spawnPos;

	EM->ResetTimer(LifeTimerID, time);

	Vector3 AddedVel = GetRandomVelocity(GetRandomFloat(speed * 0.25f, speed));
	Velocity = Vector3Add(velocity, AddedVel);
}