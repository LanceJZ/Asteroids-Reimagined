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
	ParticleModel.linePoints.push_back({ -0.250f, 0.250f, 0 });
	ParticleModel.linePoints.push_back({ 0.250f, 0.250f, 0 });
	ParticleModel.linePoints.push_back({ 0.250f, -0.250f, 0 });
	ParticleModel.linePoints.push_back({ -0.250f, -0.250f, 0 });

	return false;
}

void ParticleManager::SetCubeModel(Model model)
{
	CubeModel = model;
}

void ParticleManager::SetLineModel(LineModelPoints model)
{
	ParticleModel = model;
}

void ParticleManager::SetManagers(EntityManager& managers)
{
	EM = &managers;
}

bool ParticleManager::BeginRun()
{
	Common::BeginRun();

	return false;
}

void ParticleManager::FixedUpdate()
{
	Common::FixedUpdate();
}

void ParticleManager::SpawnCubes(Vector3 position, Vector3 velocity, float radius,
	float maxSpeed, int count, float maxTime, Color color)
{
	for (int i = 0; i < count; i++)
	{
		CubeParticles[SpawnCubePool(color)]->Spawn(position, velocity,
			radius, maxSpeed,  maxTime);
	}

}

void ParticleManager::SpawnLineDots(Vector3 position, Vector3 velocity,
	float radius, float maxSpeed, int count, float maxTime, Color color)
{
	for (int i = 0; i < count; i++)
	{
		LineParticles[SpawnLinePool(color)]->Spawn(position, velocity,
			radius, maxSpeed, maxTime);
	}
}

void ParticleManager::SpawnLineModelExplosion(LineModelPoints model,
	Vector3 position, Vector3 velocity, float rotationZ,
	float maxSpeed, float maxTime, Color color)
{
	std::vector<LineModelPoints> lineModelParts = {};

	for (int i = 0; i < model.linePoints.size() - 1; i++)
	{
		LineModelPoints lineModelPart = {};

		lineModelPart.linePoints.push_back(model.linePoints[i]);
		lineModelPart.linePoints.push_back(model.linePoints[i + 1]);

		lineModelParts.push_back(lineModelPart);
	}

	for (const auto& partModel : lineModelParts)
	{
		ExplosionLineModels[SpawnLineModelPool(partModel, color)]->Spawn(position,
			velocity, rotationZ, maxSpeed, maxTime);
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
bool ParticleManager::GetParticlesActive()
{
	for (const auto& cube : CubeParticles)
	{
		if (cube->Enabled)
		{
			return true;
		}
	}

	for (const auto& line : LineParticles)
	{
		if (line->Enabled)
		{
			return true;
		}
	}

	for (const auto& lineModel : ExplosionLineModels)
	{
		if (lineModel->Enabled)
		{
			return true;
		}
	}

	return false;
}
size_t ParticleManager::SpawnCubePool(Color color)
{
		bool spawnNew = true;
		size_t cubeSpawnNumber = CubeParticles.size();

		for (size_t check = 0; check < cubeSpawnNumber; check++)
		{
			if (!CubeParticles[check]->Enabled)
			{
				spawnNew = false;
				cubeSpawnNumber = check;
				break;
			}
		}

		if (spawnNew)
		{
			CubeParticles.push_back(DBG_NEW ParticleCube());
			EM->AddModel3D(CubeParticles[cubeSpawnNumber], CubeModel);
			CubeParticles[cubeSpawnNumber]->SetManagers(EM);
			CubeParticles[cubeSpawnNumber]->Initialize(TheUtilities);
			CubeParticles[cubeSpawnNumber]->BeginRun();
		}

		CubeParticles[cubeSpawnNumber]->ModelColor = color;

	return cubeSpawnNumber;
}

size_t ParticleManager::SpawnLinePool(Color color)
{
	bool spawnNew = true;
	size_t lineSpawnNumber = LineParticles.size();

	for (size_t check = 0; check < lineSpawnNumber; check++)
	{
		if (!LineParticles[check]->Enabled)
		{
			spawnNew = false;
			lineSpawnNumber = check;
			break;
		}
	}

	if (spawnNew)
	{
		LineParticles.push_back(DBG_NEW LineParticle());
		EM->AddLineModel(LineParticles[lineSpawnNumber], ParticleModel);
		LineParticles.back()->SetEntityManager(EM);
		LineParticles.back()->Initialize(TheUtilities);
		LineParticles.back()->BeginRun();
	}

	LineParticles[lineSpawnNumber]->ModelColor = color;

	return lineSpawnNumber;
}

size_t ParticleManager::SpawnLineModelPool(LineModelPoints modelPart,
	Color color)
{
	bool spawnNew = true;
	size_t lineModelSpawnNumber = ExplosionLineModels.size();

	for (size_t check = 0; check < lineModelSpawnNumber; check++)
	{
		if (!ExplosionLineModels[check]->Enabled)
		{
			spawnNew = false;
			lineModelSpawnNumber = check;
			break;
		}
	}

	if (spawnNew)
	{
		ExplosionLineModels.push_back(DBG_NEW LineModelParticle());
		EM->AddLineModel(ExplosionLineModels[lineModelSpawnNumber], modelPart);
		ExplosionLineModels[lineModelSpawnNumber]->SetEntityManager(EM);
		ExplosionLineModels[lineModelSpawnNumber]->Initialize(TheUtilities);
		ExplosionLineModels[lineModelSpawnNumber]->BeginRun();
	}

	ExplosionLineModels[lineModelSpawnNumber]->ModelColor = color;

	return lineModelSpawnNumber;
}
