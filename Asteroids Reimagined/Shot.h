#pragma once
#include "Globals.h"

class Shot : public Model3D
{
public:
	Shot();
	virtual ~Shot();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw();

	void Spawn(Vector3 position);
	void Spawn(Vector3 position, Vector3 velocity);
	void SpawnPlayerShot(Vector3 position);
	void Destroy();

private:

};
