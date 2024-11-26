#pragma once
#include "Common.h"
#include "EntityManager.h"
#include "ParticleCube.h"
#include "LineParticle.h"
#include "LineModelParticle.h"

#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
	// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
	// allocations to be of _CLIENT_BLOCK type
#else
	#define DBG_NEW new
#endif

class ParticleManager : public Common
{
public:
	ParticleManager();
	virtual ~ParticleManager();

	void SetManagers(EntityManager& entityManager);
	void SetCubeModel(Model model);
	void SetLineModel(LineModelPoints model);

	virtual bool Initialize(Utilities* utilities);
	virtual bool BeginRun();

	virtual void FixedUpdate();

	void SpawnCubes(Vector3 position, Vector3 velocity, float radius,
		float maxSpeed,  int count, float maxTime, Color color);
	void SpawnLineDots(Vector3 position, Vector3 velocity, float radius,
		float maxSpeed, int count, float maxTime, Color color);
	void SpawnLineModelExplosion(LineModelPoints model, Vector3 position,
		Vector3 velocity, float rotationZ, float maxSpeed,
		float maxTime, Color color);

	void ResetCubes();
	void ResetLines();

	bool GetParticlesActive();
protected:
	Model CubeModel = {};
	LineModelPoints ParticleModel = {};
	EntityManager* EM = nullptr;

private:
	std::vector<ParticleCube*> CubeParticles;
	std::vector<LineParticle*> LineParticles;
	std::vector<LineModelParticle*> ExplosionLineModels;

	size_t SpawnCubePool(Color color);
	size_t SpawnLinePool(Color color);
	size_t SpawnLineModelPool(LineModelPoints modelPart, Color color);
};

