#pragma once
#include "Globals.h"
#include "Shot.h"

class TheBossTurret : public LineModel
{
public:
	TheBossTurret();
	virtual ~TheBossTurret();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn();
	void Destroy();

private:

	std::vector<Shot*> Shots;
};
