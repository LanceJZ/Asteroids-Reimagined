#pragma once
#include "Globals.h"

class TheBoss : public LineModel
{
public:
	TheBoss();
	virtual ~TheBoss();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:


};
