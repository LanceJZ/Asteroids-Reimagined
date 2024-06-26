#pragma once
#include "Globals.h"

class Shot : public LineModel
{
public:
	Shot();
	virtual ~Shot();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Spawn(Vector3 position, Vector3 velocity);
	void Spawn(Vector3 position, Vector3 velocity, float lifetime);
	void Destroy();

private:
	size_t LifeTimerID = 0;
};
