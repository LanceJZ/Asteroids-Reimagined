#pragma once
#include "ParticleManager.h"
#include "Managers.h"

#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
	// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
	// allocations to be of _CLIENT_BLOCK type
#else
	#define DBG_NEW new
#endif

class ExplosionControl : public ParticleManager
{
public:
	ExplosionControl();
	virtual ~ExplosionControl();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

	void Spawn(Vector3 position, float radius,
		float speed,  int count, float time, Color color);

private:

};

