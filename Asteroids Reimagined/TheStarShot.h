#pragma once
#include "Globals.h"
#include "LineModel.h"

class TheStarShot : public LineModel
{
public:
	TheStarShot();
	virtual ~TheStarShot();

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:


};
