#pragma once
#include "Globals.h"
#include "LineModel.h"

class ThePlasmaShot : public LineModel
{
public:
	ThePlasmaShot();
	virtual ~ThePlasmaShot();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(double deltaTime);
	void FixedUpdate(double deltaTime);
	void Draw3D();

	void Spawn(Vector3 position, Vector3 velocity);
	void Destroy();

private:
	size_t LifeTimerID = 0;
	float GrowthRate = 1.666f;
};
