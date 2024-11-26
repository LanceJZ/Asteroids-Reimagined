#pragma once
#include "LineModel.h"
#include "EntityManager.h"

class LineModelParticle : public LineModel
{
public:
	LineModelParticle();
	virtual ~LineModelParticle();

	void SetEntityManager(EntityManager *entityManager);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position, Vector3 velocity,
		float rotationZ, float maxSpeed,float maxTime);
	void Destroy();

private:
	size_t LifeTimerID = 0;

	EntityManager* EM = nullptr;
};
