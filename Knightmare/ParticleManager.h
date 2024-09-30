#pragma once
#include "Common.h"
#include "TheManagers.h"
#include "ParticleCube.h"
#include "LineParticle.h"

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

	virtual void SetManagers(TheManagers& managers);
	virtual void SetCubeModel(Model model);

	virtual bool Initialize(Utilities* utilities);
	virtual bool BeginRun();

	virtual void Update();

	virtual void SpawnCubes(Vector3 position, Vector3 velocity, float radius,
		float speed,  int count, float time, Color color);
	virtual void SpawnLineParticles(Vector3 position, Vector3 velocity, float radius,
		float speed, int count, float time, Color color);
	virtual void ResetCubes();
	virtual void ResetLines();
protected:
	Model CubeModel = {};
	LineModelPoints ParticleModel = {};
	TheManagers* Managers = {};

private:
	std::vector<ParticleCube*> CubeParticles;
	std::vector<LineParticle*> LineParticles;
	size_t SpawnCubePool(Color color);
	size_t SpawnLinePool(Color color);
};

