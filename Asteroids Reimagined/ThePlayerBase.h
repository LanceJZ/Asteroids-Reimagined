#pragma once
#include "Globals.h"
#include "Shot.h"

class ThePlayerBase : public LineModel
{
public:
	ThePlayerBase();
	virtual ~ThePlayerBase();

	std::vector<Shot*> Shots = {};

	void SetShotModel(std::vector<Vector3> model);

	void SetFireSound(Sound sound);
	void SetExplodeSound(Sound sound);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

protected:

	size_t MagazineSize = 0;
	size_t FireRateTimerID = 0;

	Sound FireSound = {};
	Sound ExplodeSound = {};

	std::vector<Vector3> ShotModel;

private:


};
