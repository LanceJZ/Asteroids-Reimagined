#pragma once
#include "Common.h"
#include "ParticleManager.h"

#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
	// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
	// allocations to be of _CLIENT_BLOCK type
#else
	#define DBG_NEW new
#endif

class ThrustControl : public ParticleManager
{
public:
	ThrustControl();
	virtual ~ThrustControl();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

	void Spawn(Vector3 position, Vector3 velocity, float endWidth,
		float length,  int count, float time, Color color);

private:

};

