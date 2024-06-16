#pragma once
#include "Common.h"
#include "Managers.h"
#include "ParticleCube.h"

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

	virtual void SetManagers(Managers& managers);
	virtual void SetCubeModel(Model model);

	virtual bool Initialize(Utilities* utilities);
	virtual bool BeginRun();

	virtual void Update();

	virtual void Spawn(Vector3 position, Vector3 velocity, float radius,
		float speed,  int count, float time, Color color);
	virtual void Reset();
protected:
	Model CubeModel = {};
	Managers* Man = {};

private:
	std::vector<ParticleCube*> Particles;
	size_t SpawnPool(Color color);

};

