#include "ParticleManager.h"

ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{
	Particles.clear();
}

bool ParticleManager::Initialize(Utilities* utilities)
{

	return false;
}

void ParticleManager::SetCubeModel(Model model)
{
	CubeModel = model;
}

void ParticleManager::SetManagers(Managers& managers)
{
	Man = &managers;
}

bool ParticleManager::BeginRun()
{
	Common::BeginRun();

	return false;
}

void ParticleManager::Update()
{
	Common::Update();
}

void ParticleManager::Spawn(Vector3 position, Vector3 velocity, float radius,
	float speed, int count, float time, Color color)
{
	for (int i = 0; i < count; i++)
	{
		Particles[SpawnPool(color)]->Spawn(position, velocity, radius, speed,  time);
	}

}

void ParticleManager::Reset()
{
	for (const auto& cube : Particles)
	{
		cube->Enabled = false;
	}
}
size_t ParticleManager::SpawnPool(Color color)
{
		bool spawnNew = true;
		size_t cubeSpawnNumber = Particles.size();
		int cubeNumber = 0;

		for (const auto& cube : Particles)
		{
			if (!cube->Enabled)
			{
				spawnNew = false;
				cubeSpawnNumber = cubeNumber;
				break;
			}

			cubeNumber++;
		}

		if (spawnNew)
		{
			Particles.push_back(DBG_NEW ParticleCube());
			Man->EM.AddModel3D(Particles[cubeSpawnNumber], CubeModel);
		}

		Particles[cubeSpawnNumber]->ModelColor = color;

	return cubeSpawnNumber;
}