#include "ParticleCube.h"
ParticleCube::ParticleCube()
{
}

ParticleCube::~ParticleCube()
{
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
	if (Enabled)
	{
		Model3D::Update(deltaTime);

		LifeTimer.Update(deltaTime);

		if (LifeTimer.Elapsed())
		{
			Enabled = false;
		}
	}
}

void ParticleCube::Draw()
{
	Model3D::Draw();

}

void ParticleCube::Spawn(Vector3 position, Vector3 velocity, float radius, float speed, float time)
{
	Enabled = true;

	Vector3 spawnPos = position;
	spawnPos.x += GetRandomFloat(-radius, radius);
	spawnPos.y += GetRandomFloat(-radius, radius);
	Position = spawnPos;

	LifeTimer.Reset(time);

	Vector3 AddedVel = GetRandomVelocity(GetRandomFloat(speed * 0.25f, speed));
	Velocity = Vector3Add(velocity, AddedVel);
}