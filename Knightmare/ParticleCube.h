#pragma once
#include "Model3D.h"
#include "Timer.h"

class ParticleCube : public Model3D
{
public:
	ParticleCube();
	virtual ~ParticleCube();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw();

	void Spawn(Vector3 position, Vector3 velocity, float radius, float speed, float time);

private:
	Timer LifeTimer;
};
