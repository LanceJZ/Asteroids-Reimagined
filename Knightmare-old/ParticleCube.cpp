#include "ParticleCube.h"
ParticleCube::ParticleCube()
{
}

ParticleCube::~ParticleCube()
{
}

void ParticleCube::SetManagers(EntityManager* entityManager)
{
	EM = entityManager;

	LifeTimerID = EM->AddTimer();
}

bool ParticleCube::Initialize()
{
	Model3D::Initialize();

	return false;
}

bool ParticleCube::BeginRun()
{
	Model3D::BeginRun();

	Enabled = false;

	return false;
}

void ParticleCube::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

}

void ParticleCube::FixedUpdate(float deltaTime)
{
	Model3D::FixedUpdate(deltaTime);

	if (EM->TimerElapsed(LifeTimerID))
	{
		Enabled = false;
	}
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
	spawnPos.x += M.GetRandomFloat(-radius, radius);
	spawnPos.y += M.GetRandomFloat(-radius, radius);
	Position = spawnPos;

	EM->ResetTimer(LifeTimerID, time);

	Vector3 AddedVel = M.GetRandomVelocity(M.GetRandomFloat(speed * 0.25f, speed));
	Velocity = Vector3Add(velocity, AddedVel);
}