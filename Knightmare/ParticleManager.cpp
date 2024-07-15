#include "ParticleManager.h"

ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{
	CubeParticles.clear();
}

bool ParticleManager::Initialize(Utilities* utilities)
{

	return false;
}

void ParticleManager::SetCubeModel(Model model)
{
	CubeModel = model;
}

void ParticleManager::SetManagers(TheManagers& managers)
{
	Managers = &managers;
}

bool ParticleManager::BeginRun()
{
	Common::BeginRun();

	ParticleModel = Managers->CM.LoadAndGetLineModel("Dot"); //Replace with vertices in code.

	return false;
}

void ParticleManager::Update()
{
	Common::Update();
}

void ParticleManager::SpawnCubes(Vector3 position, Vector3 velocity, float radius,
	float speed, int count, float time, Color color)
{
	for (int i = 0; i < count; i++)
	{
		CubeParticles[SpawnCubePool(color)]->Spawn(position, velocity, radius, speed,  time);
	}

}

void ParticleManager::SpawnLineParticles(Vector3 position, Vector3 velocity, float radius,
	float speed, int count, float time, Color color)
{
	for (int i = 0; i < count; i++)
	{
		LineParticles[SpawnLinePool(color)]->Spawn(position, velocity, radius, speed, time);
	}
}

void ParticleManager::ResetCubes()
{
	for (const auto& cube : CubeParticles)
	{
		cube->Enabled = false;
	}
}
void ParticleManager::ResetLines()
{
	for (const auto& line : LineParticles)
	{
		line->Enabled = false;
	}
}
size_t ParticleManager::SpawnCubePool(Color color)
{
		bool spawnNew = true;
		size_t cubeSpawnNumber = CubeParticles.size();
		int cubeNumber = 0;

		for (const auto& cube : CubeParticles)
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
			CubeParticles.push_back(DBG_NEW ParticleCube());
			Managers->EM.AddModel3D(CubeParticles[cubeSpawnNumber], CubeModel);
		}

		CubeParticles[cubeSpawnNumber]->ModelColor = color;

	return cubeSpawnNumber;
}

size_t ParticleManager::SpawnLinePool(Color color)
{
	bool spawnNew = true;
	size_t lineSpawnNumber = LineParticles.size();
	int lineNumber = 0;

	for (const auto& line : LineParticles)
	{
		if (!line->Enabled)
		{
			spawnNew = false;
			lineSpawnNumber = lineNumber;
			break;
		}

		lineNumber++;
	}

	if (spawnNew)
	{
		LineParticles.push_back(DBG_NEW LineParticle());
		LineParticles.back()->SetManagers(Managers);
		LineParticles.back()->Initialize(TheUtilities);
		LineParticles.back()->BeginRun();
		Managers->EM.AddLineModel(LineParticles[lineSpawnNumber], ParticleModel);
	}

	LineParticles[lineSpawnNumber]->ModelColor = color;

	return lineSpawnNumber;
}
