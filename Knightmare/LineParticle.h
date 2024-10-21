#pragma once
#include "LineModel.h"
#include "TheManagers.h"

class LineParticle : public LineModel
{
public:
	LineParticle();
	virtual ~LineParticle();

	void SetManagers(TheManagers* managers);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(double deltaTime);
	void Draw3D();

	void Spawn(Vector3 position, Vector3 velocity, float radius, float speed, float time);
	void Destroy();

private:
	size_t LifeTimerID = 0;

	TheManagers* Managers = nullptr;
};
