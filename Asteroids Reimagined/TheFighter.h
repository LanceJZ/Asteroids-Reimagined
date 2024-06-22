#pragma once
#include "Globals.h"

class TheFighter : public LineModel
{
public:
	TheFighter();
	virtual ~TheFighter();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:


};
