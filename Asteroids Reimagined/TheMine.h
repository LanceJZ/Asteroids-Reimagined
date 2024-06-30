#pragma once
#include "Globals.h"

class TheMine : public LineModel
{
public:
	TheMine();
	virtual ~TheMine();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:

	size_t LifeTimerID = 0;
};
